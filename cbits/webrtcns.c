/*
 * webrtcns.c
 *
 *  Created on: 2021Äê4ÔÂ26ÈÕ
 *      Author: Ëï³©
 */

#define SAMPLE_RATE 16000
#define BUFFER_SIZE  160
#include <Python.h>
#include <stdlib.h>
#include "webrtcns/noise_suppression.h"
#include "webrtcns/ns_core.h"

#if PY_MAJOR_VERSION >= 3
#define PY3
#endif

static PyObject *NsError;
const char WebRtcNsDoc[] = "hello.";


static void ns_free(PyObject* nsptr)
{
  NsHandle *handle = PyCapsule_GetPointer(nsptr, "WebRtcNsPtr");
  WebRtcNs_Free(handle);
}

static PyObject* ns_create(PyObject *self, PyObject *args)
{
  NsHandle *handle = WebRtcNs_Create();
  PyObject *nsptr;
  nsptr = PyCapsule_New(handle, "WebRtcNsPtr", ns_free);
  return Py_BuildValue("O", nsptr);
}

static PyObject* ns_init(PyObject *self, PyObject *nsptr)
{
  NsHandle* handle = PyCapsule_GetPointer(nsptr, "WebRtcNsPtr");
  WebRtcNs_Init(handle, SAMPLE_RATE);
  Py_RETURN_NONE;
}

static PyObject* ns_set_mode(PyObject *self, PyObject *args)
{
  PyObject *nsptr;
  long mode;
  if (!PyArg_ParseTuple(args, "Ol", &nsptr, &mode)) {
    return NULL;
  }
  if (mode < 0) {
    return NULL;
  } else if (mode > 3) {
    PyErr_Format(PyExc_ValueError,
                 "%ld is an invalid mode, must be 0-3",
                 mode);
    return NULL;
  }
  WebRtcNs_set_policy(PyCapsule_GetPointer(nsptr, "WebRtcNsPtr"), mode);
  Py_RETURN_NONE;
}

static PyObject* ns_process(PyObject *self, PyObject *args)
{
  PyObject *nsptr;
  Py_buffer audio_frame = {NULL, NULL};
  PyObject *result;
  long frame_length;
#ifdef PY3
  if (!PyArg_ParseTuple(args, "Oy*", &nsptr, &audio_frame)) {
#else
  if (!PyArg_ParseTuple(args, "Os*", &nsptr, &audio_frame)) {
#endif
    return NULL;
  }
  short *pdatain = (short *)calloc(BUFFER_SIZE, sizeof(short));
  short *pdata = (short *)calloc(BUFFER_SIZE, sizeof(short));
  float *spframe = (float *)calloc(BUFFER_SIZE, sizeof(float));
  float *outframe = (float *)calloc(BUFFER_SIZE, sizeof(float));
  pdatain = (short *)audio_frame.buf;
  for (int i = 0; i < BUFFER_SIZE; i++)
	  spframe[i] = (float) pdatain[i];
  WebRtcNs_Analyze(PyCapsule_GetPointer(nsptr, "WebRtcNsPtr"), spframe);
  WebRtcNs_Process(PyCapsule_GetPointer(nsptr, "WebRtcNsPtr"), &spframe, 1, &outframe);
  for (int i = 0; i < BUFFER_SIZE; i++)
	  pdata[i] = (short)outframe[i];
#ifdef PY3
  result = Py_BuildValue("y#", pdata, BUFFER_SIZE * 2);
#else
  result = Py_BuildValue("s#", pdata, BUFFER_SIZE * 2);
#endif
  PyBuffer_Release(&audio_frame);
  return result;
}

static PyMethodDef WebRtcNsMethods[] = {
    {"create",  ns_create, METH_NOARGS,
     "Create a ns."},
    {"init",  ns_init, METH_O,
     "Init a ns."},
    {"set_mode",  ns_set_mode, METH_VARARGS,
     "Set mode."},
    {"process",  ns_process, METH_VARARGS,
     "Process."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

#ifdef PY3
static struct PyModuleDef WebRtcNsModule = {
   PyModuleDef_HEAD_INIT,
   "_webrtcns",   /* name of module */
   WebRtcNsDoc, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   WebRtcNsMethods
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit__webrtcns(void)

#else

#define INITERROR return

void
init_webrtcns(void)

#endif

{
#ifdef PY3
    PyObject *module = PyModule_Create(&WebRtcNsModule);
#else
    PyObject *module = Py_InitModule3("_webrtcns", WebRtcNsMethods, WebRtcNsDoc);
#endif
    if (module == NULL) {
      INITERROR;
    }

    NsError = PyErr_NewException("webrtcns.Error", NULL, NULL);
    Py_INCREF(NsError);
    PyModule_AddObject(module, "Error", NsError);

#ifdef PY3
    return module;
#endif
}
