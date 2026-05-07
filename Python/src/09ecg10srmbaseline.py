import numpy as np
from scipy.datasets import electrocardiogram
import matplotlib.pyplot as plt

# データロード
ecg = electrocardiogram()
fs = 360  # Hz

# 最初の10秒で切り取る
ecg = ecg[(0*fs):(10*fs)]
t = np.arange(0, 10, 1/fs)

# 基線変動除去
ma = np.ones(11) / 11
ecg_ma = np.convolve(ecg, ma, 'valid')  # 移動平均フィルタ
delay = int( (len(ma) - 1) / 2 )
ecg_hp = ecg[delay:-delay] - ecg_ma  # ハイパスフィルタ

# 描画
plt.plot(t, ecg, color="gray")
plt.plot(t[delay:-delay], ecg_hp, color="black")
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
