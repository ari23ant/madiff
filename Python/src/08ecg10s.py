import numpy as np
from scipy.datasets import electrocardiogram
import matplotlib.pyplot as plt

# データロード
ecg = electrocardiogram()
fs = 360  # Hz

# 最初の10秒で切り取る
ecg = ecg[(0*fs):(10*fs)]
t = np.arange(0, 10, 1/fs)

# 描画
plt.plot(t, ecg, color="gray")
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
