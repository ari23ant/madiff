import numpy as np
from scipy.datasets import electrocardiogram
import matplotlib.pyplot as plt

# データロード
ecg = electrocardiogram()
fs = 360  # Hz
t = np.arange(ecg.size) / fs

# 描画
plt.plot(t, ecg, color="gray")
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
