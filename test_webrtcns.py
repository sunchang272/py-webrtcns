import scipy.io.wavfile as wavfile
import webrtcns
import numpy as np
import struct

mic = './wav/1-car.wav'
out = './wav/out.wav'

seg_len = 160

ns = webrtcns.Ns()
# 0, 1, 2, 3
ns.set_mode(3)

def wave_header(sample_array, sample_rate):
    byte_count = (len(sample_array)) * 2 # short
    # write the header
    hdr = struct.pack(b'<ccccIccccccccIHHIIHH',
        b'R', b'I', b'F', b'F',
        byte_count + 0x2c - 8,  # header size
        b'W', b'A', b'V', b'E', b'f', b'm', b't', b' ',
        0x10,  # size of 'fmt ' header
        1,  # format 1
        1,  # channels
        sample_rate,  # samples / second
        sample_rate * 2,  # bytes / second
        2,  # block alignment
        16)  # bits / sample
    hdr += struct.pack(b'<ccccI',
        b'd', b'a', b't', b'a', byte_count)
    return hdr

def test_webrtcns():
    fs, sig = wavfile.read(mic)
    fout = open(out,'wb')
    head = wave_header(sig, fs)
    fout.write(head)
    for seg_index in range(0, len(sig), seg_len):
        if seg_index + seg_len >= len(sig):
            break
        else:
            nn = ns.process(sig[seg_index:seg_index+seg_len])
            fout.write(nn)
    fout.close()

if __name__ == '__main__':
    test_webrtcns()
