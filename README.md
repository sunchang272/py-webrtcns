# py-webrtcns
This is a python interface to the WebRTC NS

# How to use it

0. Install the webrtcns module::
2. Create a ``Ns`` object::
3. Set mode
4. Give it a short segment ("frame") of audio. It only accepts 16-bit mono PCM audio, sampled at 16000Hz. A frame must be 160 samples::
