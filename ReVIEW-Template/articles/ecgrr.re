= 心拍数計算
これまで学んだ技術を使って、心電図から心拍数を計算してみよう。


== データ準備
今回はScipyにある心電図を使う。公式ドキュメント@<fn>{scipy.datasets.electrocardiogram}によると、サンプリング周波数が360Hzで、5分間の心電図とある。この情報をもとにまずは描画してみる。
//footnote[scipy.datasets.electrocardiogram][https://docs.scipy.org/doc/scipy/reference/generated/scipy.datasets.electrocardiogram.html]

//list[07ecg.py][心電図を描画する]{
import numpy as np
from scipy.datasets import electrocardiogram
import matplotlib.pyplot as plt

# データロード
ecg = electrocardiogram()
fs = 360  # Hz
t = np.arange(ecg.size) / fs

# 描画
plt.plot(t, ecg)
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[07ecg][心電図を描画][scale=0.8]

//noindent
帯状の波形しか見えず、見通しが悪い。よって、今回は最初の10秒のみを扱う。

//list[08ecg10s.py][最初の10秒を描画する]{
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
plt.plot(t, ecg)
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[08ecg10s][最初の10秒を描画]

//noindent
心電図らしい波形が見えてきた。この波形から心拍数を求めてみる。


== 心拍数算出
さて、分析を始めてみよう。まず、想像する心電図と違って大きく揺らいでいるので、移動平均フィルタによるハイパスフィルタで基線変動を除去する。

//list[09ecg10srmbaseline.py][心電図の基線変動を除去する]{
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
plt.plot(t, ecg)
plt.plot(t[delay:-delay], ecg_hp)
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[09ecg10srmbaseline][心電図の基線変動を除去]

//noindent
黒線を見ると、きれいに基線変動が除去できている。このままでもよいが、高周波ノイズが確認できる。そこで、再び移動平均フィルタを使って、この高周波ノイズを除去する。

//list[10ecg10s_rmbaseline_trimnoise.py][心電図の高周波ノイズを除去する]{
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

# 描画
plt.plot(t[delay:-delay], ecg_hp)
plt.plot(t[delay2:-delay2], ecg_hp_ma)
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[10ecg10s_rmbaseline_trimnoise][心電図の高周波ノイズを除去]

//noindent
まだ少しギザギザしているが、先に進もう。心拍数を計算するためには、R波と言われる一番高い山の位置を取る必要があるので、ピーク検出する。

//list[11ecg10s_rmbaseline_trimnoise_findpeak.py][心電図のピークを検出する]{
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
plt.plot(t[delay2:-delay2], ecg_hp_ma)
plt.plot(t[delay2:-delay2][index], ecg_hp_ma[index], 'o')
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[11ecg10s_rmbaseline_trimnoise_findpeak][心電図のピークを検出]

//noindent
直前で高周波ノイズを除去したものの、R波以外のピークも検出してしまった。対策方法は色々あるが、今回は高さの閾値を設けて簡単に検出する。

//list[12ecg10s_rmbaseline_trimnoise_findpeak2.py][高さの閾値を設けて心電図のピークを検出する]{
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
        if (ecg_hp_ma[i+1] > 0.1):  # 高さの閾値
            index.append(i+1)

# 描画
plt.plot(t[delay2:-delay2], ecg_hp_ma)
plt.plot(t[delay2:-delay2][index], ecg_hp_ma[index], 'o')
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//image[12ecg10s_rmbaseline_trimnoise_findpeak2][高さの閾値を設けて心電図のピークを検出]

きれいにR波を検出できたので、心拍数を求めよう。心拍数の単位はbpm（beats per minuteの略）であるため、1分間に何回拍動するかを計算すればよい。

//list[13ecg10s_rmbaseline_trimnoise_findpeak2_calchr.py][心拍数を計算する]{
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
        if (ecg_hp_ma[i+1] > 0.1):  # 高さの閾値
            index.append(i+1)

# 心拍数計算
HR = 60 * (len(index)/10)
print(f'HR: {HR} [bpm]')

# 描画
plt.plot(t[delay2:-delay2], ecg_hp_ma)
plt.plot(t[delay2:-delay2][index], ecg_hp_ma[index], 'o')
plt.xlabel("Time [s]")
plt.ylabel("ECG [mV]")
plt.show()
//}

//noindent
実行結果は以下の通り。

//list[][]{
HR: 114.0 [bpm]
//}

//noindent
これまで学んだ移動平均フィルタと微分フィルタだけで、心電図から心拍数を求めることができた。


== 練習問題
以下の2つを考えてもらいたい。

 1. 10秒後以降の心電図でもうまく検出できるか
 2. もし基線変動を除去しない場合、うまくピーク検出できるか

上記はいずれも答えがあるものではなく、読者の理解を深めるために設けた。自分の手でぜひ試してみてほしい。
