import pkg_resources

import _webrtcns

__author__ = "Chang Sun"
__copyright__ = "Copyright (C) 2021 Chang Sun"
__license__ = ""
__version__ = pkg_resources.get_distribution('webrtcns').version


class Ns(object):
    def __init__(self, mode=None):
        self._ns = _webrtcns.create()
        _webrtcns.init(self._ns)
        if mode is not None:
            self.set_mode(mode)

    def set_mode(self, mode):
        _webrtcns.set_mode(self._ns, mode)

    def process(self, buf):
        return _webrtcns.process(self._ns, buf)
