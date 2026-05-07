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

# 高周波ノイズ除去
ma = np.ones(11) / 11
ecg_hp_ma = np.convolve(ecg_hp, ma, 'valid')  # 移動平均フィルタ
delay2 = int( (len(ma) - 1) / 2 ) + delay

# ピーク検出
ecg_hp_ma_diff = np.diff(ecg_hp_ma) / (1.0/fs)  # 微分フィルタ
index = []  # 検出したピークのインデックス
for i in range(len(ecg_hp_ma_diff)-1):
    if (ecg_hp_ma_diff[i] >= 0) and (ecg_hp_ma_diff[i+1] < 0):
        index.append(i+1)

# 描画
#plt.plot(t, ecg)
#plt.plot(t[delay:-delay], ecg_hp)
plt.plot(t[delay2:-delay2], ecg_hp_ma, color="gray")
plt.plot(t[delay2:-delay2][index], ecg_hp_ma[index], 'o', color="black")
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
