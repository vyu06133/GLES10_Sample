package com.example.gles10_sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class MainActivity : AppCompatActivity() {
	
	private lateinit var glView: MyGLSurfaceView
	
	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		
		glView = MyGLSurfaceView(this)
		setContentView(glView)
	}
	
	override fun onPause() {
		super.onPause()
		glView.onPause()
	}
	
	override fun onResume() {
		super.onResume()
		glView.onResume()
	}
}