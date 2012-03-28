/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class yon_AndroidGLView */

#ifndef _Included_yon_AndroidGLView
#define _Included_yon_AndroidGLView
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnSurfaceCreated
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_yon_AndroidGLView_nativeOnSurfaceCreated
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnSurfaceChanged
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_yon_AndroidGLView_nativeOnSurfaceChanged
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnDrawFrame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_yon_AndroidGLView_nativeOnDrawFrame
  (JNIEnv *, jobject);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnPause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_yon_AndroidGLView_nativeOnPause
  (JNIEnv *, jobject);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnResume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_yon_AndroidGLView_nativeOnResume
  (JNIEnv *, jobject);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnBack
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_yon_AndroidGLView_nativeOnBack
  (JNIEnv *, jobject);

/*
 * Class:     yon_AndroidGLView
 * Method:    nativeOnTouch
 * Signature: (IFF)Z
 */
JNIEXPORT jboolean JNICALL Java_yon_AndroidGLView_nativeOnTouch
  (JNIEnv *, jobject, jint, jfloat, jfloat);

#ifdef __cplusplus
}
#endif
#endif