= 基線変動除去
基線変動とは、その名の通り波形の基線（ベースライン）が動く状態を示す。よくセンサの不具合や、実験環境の不備が原因で生じる現象である。しかし、そもそもの信号が基線変動する特性を持つときは、ソフトウェアで解決してもよい。様々な解決方法があるが、本書ではこれまで学んだ移動平均フィルタを使う方法と微分フィルタを使う方法を紹介する。


== 移動平均フィルタで基線変動を除去
一般に、基線変動は取得したい波形よりも低周波である。したがって、移動平均フィルタを使ったハイパスフィルタで除去できる。ここでは、以下の波形を考えてみる。

//texequation[sinwavebaseline]{
x(t) = A_{1} \sin 2 \pi f_{1} \ t + a t
//}

第1項が抽出したい正弦波で、第2項が重畳する基線変動である。簡単のため基線変動は直線とした。この基線変動を、移動平均フィルタを使ったハイパスフィルタで除去し、正弦波を抽出する。設定値は以下の通り。

//table[sinwavebaselineparams_ma][疑似波形の設定値]{
設定項目	値など
------------------
波形の長さ[s]	3.0
サンプリング周波数[Hz]	125
抽出したい波形	正弦波
波形の振幅[]	1
波形の周波数[Hz]	10
基線変動の傾き[]	0.5
//}

以下にPythonスクリプトと、実行結果のグラフを示す。

//list[04rmbaselinema.py][移動平均フィルタを使ったハイパスフィルタで基線変動を除去する]{
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
plt.plot(t, wave)
plt.plot(t[delay:-delay], sin_hp)
plt.show()
//}

//image[04rmbaselinema][正弦波に基線変動が重畳した波形（灰）と移動平均フィルタによるハイパスフィルタで基線変動を除去した波形（黒）][scale=0.8]

//noindent
@<list>{04rmbaselinema.py}で作ったフィルタは、@<hd>{highpass|移動平均フィルタで低周波ノイズを除去}のハイパスフィルタと同じである。@<img>{04rmbaselinema}より、移動平均フィルタで作ったハイパスフィルタで基線変動が除去できていることがわかる。


== 練習問題
読者には以下を試してほしい。

 1. ハイパスフィルタを通した波形@<code>{wave_ma}を描画し、どのような波形だろうか
 2. 移動平均フィルタの設計値を変えると、振る舞いがどう変わるか

2.について、@<code>{wave_ma}の波形もプロットするとより理解が深まるだろう。


== 微分フィルタで基線変動を除去
実は微分フィルタでも基線変動が除去できる。前述と同じように@<m>$x(t) = A_{1} \sin 2 \pi f_{1} \ t + a t$を題材にして考えてみる。簡単のため@<m>$\omega_{1} = 2\pi f_{1}$とし、時間@<m>{t}で微分する。

//texequation[dxdt]{
\begin{aligned}
\frac{d}{dt} x(t) &= \lim_{\Delta t \to 0} \frac{x(t+\Delta t) - x(t)}{\Delta t} \\
\frac{d}{dt} x(t) &= \lim_{\Delta t \to 0} \frac{ \{ A_{1} \sin \omega_{1} \ (t+\Delta t) + a (t+\Delta t) \} - (A_{1} \sin \omega_{1} \ t + a t) }{\Delta t} \\
\frac{d}{dt} x(t) &= \lim_{\Delta t \to 0} \frac{ \{ A_{1} \sin \omega_{1} \ (t+\Delta t) - A_{1} \sin \omega_{1} \ t \} + \{ a (t+\Delta t) - a t) \} }{\Delta t} \\
\frac{d}{dt} x(t) &= \lim_{\Delta t \to 0} \frac{ A_{1} \sin \omega_{1} \ (t+\Delta t) - A_{1} \sin \omega_{1} \ t }{\Delta t} +  \lim_{\Delta t \to 0} \frac{ a (t+\Delta t) - a t }{\Delta t} \\
\frac{d}{dt} x(t) &=   A_{1} \omega_{1} \cos \omega_{1} \ t  + a \\
\end{aligned}
//}

//noindent
微分をすると基線変動の@<m>{at}から@<m>{t}が取れて、定数である@<m>{a}だけとなり時間変動がなくなったことがわかる。

これを使って、基線変動を除去する。

//texequation[sinwavebaselines][@<eq>{sinwavebaseline}再掲]{
x(t) = A_{1} \sin 2 \pi f_{1} \ t + a t
//}

//noindent
設定値は以下の通り。

//table[sinwavebaselineparams_diff][疑似波形の設定値]{
設定項目	値など
------------------
波形の長さ[s]	3.0
サンプリング周波数[Hz]	125
抽出したい波形	正弦波
波形の振幅[]	1
波形の周波数[Hz]	1
基線変動の傾き[]	2
//}

以下にPythonスクリプトと、実行結果のグラフを示す。

//list[05rmbaselinediff.py][微分フィルタを使ったハイパスフィルタで基線変動を除去する]{
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
plt.plot(t, wave)
plt.plot(t[1:], wave_diff)
plt.show()
//}

//image[05rmbaselinediff][正弦波に基線変動が重畳した波形（灰）と微分フィルタによるハイパスフィルタで基線変動を除去した波形（黒）][scale=0.8]

//noindent
@<img>{05rmbaselinediff}より、微分フィルタで基線変動が除去できていることがわかる。

ところで、元の波形（灰）と基線変動を除去した波形（黒）を比べると、後者の方が振幅が大きい。これは@<eq>{xt_dxdt}のように、微分後は微分前と比べて振幅が@<m>$\omega_{1}$倍になるためとわかる。

//texequation[xt_dxdt][]{
\begin{aligned}
x(t) &= A_{1} \sin 2 \omega_{1} t + a t \\
\frac{d}{dt} x(t) &= A_{1} \omega_{1} \cos \omega_{1} \ t  + a
\end{aligned}
//}

//noindent
また@<m>{\sin}から@<m>{\cos}になるため、微分フィルタを通すと振幅だけでなく位相もズレる。このように注意しなければならない点が多く、扱いが難しい手法ではある。


== 練習問題
以下の2つを考えてもらいたい。

 1. 正弦波を微分すると、位相はどのくらいズレるか
 2. 設定値を変えると振る舞いがどう変わるか

特に1.は高校数学の範囲なので、ぜひ確認してもらいたい。
