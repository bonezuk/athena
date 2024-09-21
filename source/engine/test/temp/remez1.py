import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

fs = 22050   # Sample rate, Hz

def plot_response(w, h, title):
    "Utility function to plot response functions"
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(w, 20*np.log10(np.abs(h)))
    ax.set_ylim(-40, 5)
    ax.grid(True)
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Gain (dB)')
    ax.set_title(title)
    
cutoff = 8000.0    # Desired cutoff frequency, Hz
trans_width = 100  # Width of transition from pass to stop, Hz
numtaps = 325      # Size of the FIR filter.
taps = signal.remez(numtaps, [0, cutoff, cutoff + trans_width, 0.5*fs],
                    [1, 0], fs=fs)
w, h = signal.freqz(taps, [1], worN=2000, fs=fs)
plot_response(w, h, "Low-pass Filter")
plt.show()
