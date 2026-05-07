import numpy as np
import matplotlib.pyplot as plt

# 2次関数
dt = 0.1  # 刻み幅
t = np.arange(-5, 5, dt)
x = np.pow(t, 2)

# 微分
dxdt = np.diff(x) / dt

# 描画
plt.plot(t, x, color="gray")
plt.plot(t[1:], dxdt, color="black")
plt.xlim(-3, 3)  # x軸範囲を指定
plt.ylim(-1, 5)  # y軸範囲を指定
plt.show()
