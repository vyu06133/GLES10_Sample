環境：Android Studio Meerkat Feature Drop | 2024.3.2

テスト機：Android14タブレット端末、Android15 Pixel4a

使用ライブラリ：GLM

GLES20のシェーダベースプロジェクトでシェーダコンパイルでコケることがあったので非シェーダ（GLES10）を試してみる

ついでにC++コードで動作させるテストもする

C++コードはGLUTで作ってたboidsデモを引っ張り出してタスクシステムに載せ替えたコード。

GLES10は推奨されないらしい（？）のでシェーダコンパイルの確認がてらGLES20ベースにチャレンジするかも。
