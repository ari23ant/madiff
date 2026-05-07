import numpy as np
import matplotlib.pyplot as plt

# 正弦波
t = np.arange(0.0, 3.0, 1.0/125)
sin = 1 * np.sin(2.0 * np.pi * 1 * t)

# 高周波ノイズを重畳
noise = 0.1 * np.sin(2.0 * np.pi * 10 * t)
wave = sin + noise

# 移動平均フィルタ
ma = np.ones(11) / 11
wave_ma = np.convolve(wave, ma, 'valid')
delay = int( (len(ma) - 1) / 2 )

# ハイパスフィルタ
noise_hp = wave[delay:-delay] - wave_ma

# 描画
plt.plot(t, wave, color="gray")
plt.plot(t[delay:-delay], noise_hp, color="black")
plt.show()
