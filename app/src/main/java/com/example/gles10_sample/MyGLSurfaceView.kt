package com.example.gles10_sample

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class MyGLSurfaceView : GLSurfaceView {
	
	private val renderer: MyGLRenderer
	
	constructor(context: Context) : super(context) {
		renderer = MyGLRenderer()
		init()
	}
	
	constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
		renderer = MyGLRenderer()
		init()
	}
	
	private fun init() {
		// OpenGL ES 1.0 context を作成
		setEGLContextClientVersion(1) // ここが重要！GLES 1.0を指定
		setRenderer(renderer)
		
		// 必要に応じてレンダーモードを設定
		// setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY) // 変更があったときのみ描画
		setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY) // 常時描画 (アニメーションなど)
	}
}
