package com.example.gles10_sample

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10 // GL10 をインポート

class MyGLRenderer : GLSurfaceView.Renderer {
	
	// JNI ライブラリをロード
	init {
		System.loadLibrary("native-lib")
	}
	
	// ネイティブC++の描画関数を宣言
	private external fun nativeOnSurfaceCreated()
	private external fun nativeOnSurfaceChanged(width: Int, height: Int)
	private external fun nativeOnDrawFrame()
	
	override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
		nativeOnSurfaceCreated()
	}
	
	override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
		nativeOnSurfaceChanged(width, height)
	}
	
	override fun onDrawFrame(gl: GL10) {
		nativeOnDrawFrame()
	}
}
