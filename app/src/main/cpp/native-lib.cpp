#include <jni.h>
#include <string>
#include "framework.h"
#include <GLES/gl.h>
#include "boids.h"

// JNI_OnLoad はライブラリがロードされたときに呼び出される関数
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	return JNI_VERSION_1_6; // 使用するJNIバージョンを指定
}

//glm::mat4 modelViewMatrix;

// onSurfaceCreatedの実装
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnSurfaceCreated( // ここを修正
		JNIEnv* env,
		jobject /* this */) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	Boids_Init();
//	modelViewMatrix = glm::mat4(1.0f);
}

// onSurfaceChangedの実装
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnSurfaceChanged( // ここを修正
		JNIEnv* env,
		jobject /* this */,
		jint width,
		jint height) {
	glViewport(0, 0, width, height);
	Boids_Reshape(width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrthof(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnDrawFrame(
		JNIEnv* env,
		jobject /* this */) {
	Boids_FrameMove();
	Boids_FrameDraw();
}

