import numpy as np

def getFrequencyBuckets(music, sampling_rate, N):
	y = np.ftt.fft(music, N)
	Y = np.fft.fftshift(y)
	Y_ = np.abs(Y)/N
	f = np.fft.fftfreq(N)
	Y_pos = Y_[int(Y_.shape[0]/2):]
	f_pos = f[int(f.shape[0]/2):]
	