= ピーク検出
本書でのピークとは波形の山の頂点であり、これの検出方法を考える。@<img>{findpeak_max_image}に示すようなデータがあるとする。頂点付近だけ注目すれば、閾値を設けて最大値を取ればよい気もするが、頂点の高さが変位する例には対応が難しい。

//image[findpeak_max_image][最大値でピーク検出するときの問題点][scale=0.8]

そこで隣り合うデータ点の変位、または差を考えてみる。頂点になる前までは変位は増え、頂点を超えると変位は減る。つまり、変位が正から負になる場所がわかれば、ピーク検出できる。この方法であれば、頂点の高さが時間経過で変化する例にも対応できる。

//image[findpeak_diff_image][変位の増減でピーク検出する][scale=0.8]

変位の増減でピーク検出するということは、差分を取る必要がある。つまり、微分フィルタに通してやればいい。


== 例題
今回は、ゆっくり動く正弦波を重畳させた波形を題材とする。

//texequation[twosinwaves][@<eq>{ma|twosinwaves}再掲]{
x(t) = A_{1} \sin 2 \pi f_{1} \ t + A_{2} \sin 2 \pi f_{2} \ t
//}

//noindent
設定値は以下の通り。便宜上、第2項の正弦波を基線変動と書く。

//table[simwaveparams][疑似波形の設定値]{
設定項目	値など
------------------
波形の長さ[s]	3.0
サンプリング周波数[Hz]	125
ピーク検出したい波形[]	正弦波
波形の振幅[]	1
波形の周波数[Hz]	1
基線変動	正弦波
基線変動の振幅[]	1
基線変動の周波数[Hz]	0.25
//}

//noindent
以下にPythonスクリプトと、実行結果のグラフを示す。

//list[06findpeak.py][微分フィルタでピーク検出する]{
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
plt.plot(t, wave)
plt.plot(t[index], wave[index], 'o')
plt.show()
//}

//image[06findpeak][ピーク検出結果][scale=0.8]

//noindent
@<img>{06findpeak}より、変位の増減で波形の頂点を検出できることがわかる。

@<list>{06findpeak.py}において、以下のピーク検出部分について解説する。

//list[06findpeak.py_diff][ピーク検出するPythonスクリプト]{
# ピーク検出
wave_diff = np.diff(wave) / (1.0/125)
index = []  # 検出したピークのインデックス
for i in range(len(wave_diff)-1):
    if (wave_diff[i] >= 0) and (wave_diff[i+1] < 0):
        index.append(i+1)
//}

//noindent
@<code>{for}文を見ると、探索範囲が@<code>{range(len(wave_diff)}ではなく、@<code>{range(len(wave_diff)-1)}と1だけ小さくしている。これは@<code>{if}文にある、@<code>{wave_diff[i+1]}が探索範囲の外となることを防ぐためである。
また、ピークを検出した際に@<code>{index}にピーク位置を格納するが、@<code>{index.append(i)}ではなく、@<code>{index.append(i+1)}と1だけ右にずらしている。これは元のデータと微分したデータ間でインデックスが1ズレているため、それを補正している。


== 練習問題
以下の2つを考えてもらいたい。

 1. 波形の谷はどうすれば検出できるか
 2. 微分の際に@<m>{dt}（スクリプト上では@<code>{(1.0/125)}）で除算しないとどうなるか

1.は複数の手法がある。よくある考え方なので、ぜひ検討してほしい。
