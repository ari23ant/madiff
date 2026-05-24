== 移動平均フィルタで基線変動を除去
一般に、基線変動は取得したい波形よりも低周波である。したがって、移動平均フィルタを使ったハイパスフィルタで除去できる。ここでは、以下の波形を考えてみる。

//texequation[sinwavebaseline]{
x(t) = A_{1} \sin 2 \pi f_{1} \ t + a t
//}

第1項が抽出したい正弦波で、第2項が重畳する基線変動である。簡単のため基線変動は直線とした。この基線変動を、移動平均フィルタを使ったハイパスフィルタで除去し、正弦波を抽出する。設定値は以下の通り。

//table[sinwavebaselineparams][疑似波形の設定値]{
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
