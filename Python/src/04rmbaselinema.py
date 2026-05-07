import numpy as np
import matplotlib.pyplot as plt

# 正弦波
t = np.arange(0.0, 3.0, 1.0/125)
sin = 1 * np.sin(2.0 * np.pi * 10 * t)

# 基線変動を重畳
baseline = 0.5 * t
wave = sin + baseline

# 移動平均フィルタ
ma = np.ones(11) / 11
wave_ma = np.convolve(wave, ma, 'valid')
delay = int( (len(ma) - 1) / 2 )

# ハイパスフィルタ
sin_hp = wave[delay:-delay] - wave_ma

# 描画
plt.plot(t, wave, color="gray")
plt.plot(t[delay:-delay], sin_hp, color="black")
plt.show()
