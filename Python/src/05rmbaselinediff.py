import numpy as np
import matplotlib.pyplot as plt

# 正弦波
t = np.arange(0.0, 3.0, 1.0/125)
sin = 1 * np.sin(2.0 * np.pi * 1 * t)

# 基線変動を重畳
baseline = 2 * t
wave = sin + baseline

# ハイパスフィルタ（微分フィルタと同じ）
wave_diff = np.diff(wave) / (1.0/125)

# 描画
plt.plot(t, wave, color="gray")
plt.plot(t[1:], wave_diff, color="black")
plt.show()
