# py-webrtcns
This is a python interface to the WebRTC noise suppression (NS).

# How to use it

1. Install the webrtcns module:

```shell
git clone https://github.com/sunchang272/py-webrtcns.git
cd py-webrtcns
python setup.py install
```

2. Create a ``Ns`` object:

```python
ns = webrtcns.Ns()
```

3. Set mode (0, 1, 2, 3):

```python
ns.set_mode(3)
```

4. Give it a short segment ("frame") of audio. It only accepts 16-bit mono PCM audio, sampled at 16000Hz. A frame must be 160 samples:

```python
ns.process(frame)
```
