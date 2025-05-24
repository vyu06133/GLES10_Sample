#include <jni.h>
#include <string>
#include <GLES/gl.h>

// JNI_OnLoad はライブラリがロードされたときに呼び出される関数
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	return JNI_VERSION_1_6; // 使用するJNIバージョンを指定
}

// onSurfaceCreatedの実装
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnSurfaceCreated( // ここを修正
		JNIEnv* env,
		jobject /* this */) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

// onSurfaceChangedの実装
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnSurfaceChanged( // ここを修正
		JNIEnv* env,
		jobject /* this */,
		jint width,
		jint height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// onDrawFrameの実装
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles10_1sample_MyGLRenderer_nativeOnDrawFrame( // ここを修正
		JNIEnv* env,
		jobject /* this */) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLfloat vertices[] = {
			0.0f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}