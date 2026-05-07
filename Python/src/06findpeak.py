import numpy as np
import matplotlib.pyplot as plt

# 正弦波
t = np.arange(0.0, 3.0, 1.0/125)
sin = 1 * np.sin(2.0 * np.pi * 1 * t)

# 基線変動を重畳
baseline = 1 * np.sin(2.0 * np.pi * 0.25 * t)
wave = sin + baseline

# ピーク検出
wave_diff = np.diff(wave) / (1.0/125)
index = []  # 検出したピークのインデックス
for i in range(len(wave_diff)-1):
    if (wave_diff[i] >= 0) and (wave_diff[i+1] < 0):
        index.append(i+1)

# 描画
plt.plot(t, wave, color="gray")
plt.plot(t[index], wave[index], 'o', color="black")
plt.show()
