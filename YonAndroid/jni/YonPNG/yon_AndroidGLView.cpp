#include "../yon_AndroidGLView.h"
#include "framework.h"
#include <GLES/gl.h>
#include "../log.h"

const char* LOG_TAG = "yon_AndroidGLView";
const char* className="yon/AndroidGLView";
const char* utilClassName="yon/util/Util";

JNIEnv *g_env=NULL;
jobject g_obj=NULL;


const static s32 MSG_SHOW_SPINNER=0;
const static s32 MSG_HIDE_SPINNER=1;
const static s32 MSG_SETUP_INPUT=2;
const static s32 MSG_COMPLETE_INPUT=3;
const static s32 MSG_TOAST=4;
const static s32 MSG_SETUP_CONFIRM=5;
const static s32 MSG_POSITIVE_CONFIRM=6;
const static s32 MSG_NEGATIVE_CONFIRM=7;

const static s32 ACTION_MASK = 255;
const static s32 ACTION_DOWN = 0;
const static s32 ACTION_UP = 1;
const static s32 ACTION_MOVE = 2;
const static s32 ACTION_POINTER_DOWN = 5;
const static s32 ACTION_POINTER_UP = 6;
const static s32 ACTION_POINTER_ID_MASK = 65280;
const static s32 ACTION_POINTER_ID_SHIFT = 8;


void Java_yon_AndroidGLView_nativeOnSurfaceCreated(JNIEnv *pEnv, jobject obj, jboolean first,jint width, jint height, jstring apkFilePath, jstring sdcardPath){
	LOGD(LOG_TAG,"first:%s,screen:{%d,%d},pEnv:%08x,nativeOnSurfaceCreated",first?"true":"false",width,height,pEnv);
	g_env=pEnv;
	g_obj=obj;
	if(first)
		init(pEnv,NULL,width,height);
}
void Java_yon_AndroidGLView_nativeOnSurfaceChanged(JNIEnv *pEnv, jobject obj, jint w, jint h){
	Logger->debug("nativeOnSurfaceChanged->w:%d,h:%d\n",w,h);
	resize(w,h);
}
void Java_yon_AndroidGLView_nativeOnDrawFrame(JNIEnv *pEnv, jobject obj){
	getEngine()->run();
	drawFrame();
}
void Java_yon_AndroidGLView_nativeOnPause(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnPause\n");
}
void Java_yon_AndroidGLView_nativeOnResume(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnResume\n");
}
//TODO改为发送Event
jboolean Java_yon_AndroidGLView_nativeOnBack(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnBack\n");
	//SINCE JDK/JRE 1.2:
	//In JDK 1.1, FindClass searched only local classes in CLASSPATH. The resulting classes did not have a class loader.
	//The Java security model has been extended to allow non-system classes to load and call native methods. 
	//In the Java 2 Platform, FindClass locates the class loader associated with the current native method. 
	//If the native code belongs to a system class, no class loader will be involved. Otherwise, 
	//the proper class loader will be invoked to load and link the named class.
	//When FindClass is called through the Invocation Interface, there is no current native method or its associated class loader. 
	//In that case, the result of ClassLoader.getBaseClassLoader is used. 
	//This is the class loader the virtual machine creates for applications, 
	//and is able to locate classes listed in the java.class.path property.

#if 1
	const char* className="yon/AndroidGLView";
	jclass cls = pEnv->FindClass(className);
	if (cls == NULL) {
		Logger->warn("can not find %s\n",className);
		return true;
	}
	//再找类中的方法
	jmethodID callbackDestroy = pEnv->GetMethodID(cls, "callbackDestroy", "()V");
	if (destroy == NULL) 
	{
		Logger->warn("no callbackDestroy function\n");
		return true;  
	}
	//回调java中的方法
	Logger->info("callbackDestroy function\n");
	destroy();
	pEnv->CallVoidMethod(obj, callbackDestroy);
#else
	const char* className="yon/AndroidGLView";
	jclass cls = pEnv->FindClass(className);
	if (cls == NULL) {
		Logger->warn("can not find %s\n",className);
		return true;
	}
	//再找类中的方法
	jmethodID callbackShowConfirm = pEnv->GetMethodID(cls, "callbackShowConfirm", "()V");
	if (destroy == NULL) 
	{
		Logger->warn("no callbackDestroy function\n");
		return true;  
	}
	//回调java中的方法
	Logger->info("callbackShowConfirm function\n");
	pEnv->CallVoidMethod(obj, callbackShowConfirm);
#endif
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnTouch(JNIEnv *pEnv, jobject obj, jint iAction, jint id, jfloat x, jfloat y, jint count){
	g_env=pEnv;
	//Logger->debug("jobject:%08X\r\n",obj);
	if(id>=YON_TOUCH_MAX_INPUTS)
	{
		Logger->warn("exceed max touch input limit: %d>=%d, ignore it!\n",id,YON_TOUCH_MAX_INPUTS);
		return true;
	}
	SEvent evt;
	evt.type=ENUM_EVENT_TYPE_TOUCH;
	evt.touchInput.count=core::min_(count,YON_TOUCH_MAX_INPUTS);
	evt.touchInput.ids[0]=(s32)id;
	evt.touchInput.xs[0]=(f32)x;
	evt.touchInput.ys[0]=(f32)y;
	switch (iAction&ACTION_MASK){
	case ACTION_DOWN:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_DOWN;
		break;  
	case ACTION_UP:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_UP;
		break;
	case ACTION_POINTER_UP:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_UP;
		break;
	case ACTION_POINTER_DOWN:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_DOWN;
		break;
	default:
		Logger->warn("unexpect event %d, do nothing!\n",iAction);
		return false;
	}
	getEngine()->postEventFromUser(evt);
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnMove(JNIEnv *pEnv, jobject obj, jint iAction, jintArray ids, jfloatArray xs, jfloatArray ys, jint count){
	//Logger->debug("nativeOnTouch:action:%d,%.2f,%.2f\n",iAction,fX,fY);
	SEvent evt;
	evt.type=ENUM_EVENT_TYPE_TOUCH;
	evt.touchInput.count=core::min_(count,YON_TOUCH_MAX_INPUTS);
	evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_MOVE;

	jint* iids = pEnv->GetIntArrayElements(ids,0);
	jfloat* fxs = pEnv->GetFloatArrayElements(xs,0);
	jfloat* fys = pEnv->GetFloatArrayElements(ys,0);
	for(int i=0;i<evt.touchInput.count;++i)
	{
		evt.touchInput.ids[i]=iids[i];
		evt.touchInput.xs[i]=fxs[i];
		evt.touchInput.ys[i]=fys[i];
	}
	pEnv->ReleaseIntArrayElements(ids, iids, 0);
	pEnv->ReleaseFloatArrayElements(xs, fxs, 0);
	pEnv->ReleaseFloatArrayElements(ys, fys, 0);

	getEngine()->postEventFromUser(evt);
	return true;
}

jboolean Java_yon_AndroidGLView_nativeOnUI(JNIEnv *pEnv, jobject obj, jint msg, jobjectArray args){
	switch(msg)
	{
	case MSG_POSITIVE_CONFIRM:
		Logger->debug("MSG_POSITIVE_CONFIRM\r\n");
		break;
	case MSG_NEGATIVE_CONFIRM:
		Logger->debug("MSG_NEGATIVE_CONFIRM\r\n");
		break;
	case MSG_COMPLETE_INPUT:
		//int getArrayLen(JNIEnv * env, jobjectArray jarray): //获取一个Java数组长度，返回为jsize类型
		jstring str = (jstring)pEnv->GetObjectArrayElement(args, 0);
		const char *word = pEnv->GetStringUTFChars(str, 0); 
		//Logger->debug("MSG_COMPLETE_INPUT:%s\r\n",word);
		SEvent evt;
		evt.type=ENUM_EVENT_TYPE_UI;
		evt.uiInput.type=ENUM_UI_INPUT_TYPE_EDITTEXT;
		evt.uiInput.str=const_cast<c8*>(word);
		getEngine()->postEventFromUser(evt);
		break;
	}
	return true;
}
void Java_yon_AndroidGLView_nativeDebug(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	Logger->debug(text);
	pEnv->ReleaseStringUTFChars(str, text);
}
void Java_yon_AndroidGLView_nativeInfo(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	Logger->info(text);
	pEnv->ReleaseStringUTFChars(str, text);
}
void Java_yon_AndroidGLView_nativeWarn(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	Logger->warn(YON_LOG_WARN_FORMAT,text);
	pEnv->ReleaseStringUTFChars(str, text);
}
void Java_yon_AndroidGLView_nativeError(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	Logger->error(YON_LOG_FAILED_FORMAT,text);
	pEnv->ReleaseStringUTFChars(str, text);
}
void Java_yon_AndroidGLView_nativeOnSurfaceDestroy(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnSurfaceDestroy\n");
}

//很遗憾，测试发现此方法没有调用到
void JNI_OnUnload(JavaVM *jvm, void *reserved){
	//JNIEnv *env;
	//if(jvm->GetEnv(jvm,(void **)&env, JNI_VERSION_1_4)){
	//	return;
	//}
	//(*env)->DeleteWeakGlobalRef(env, Class_C);
	LOGD(LOG_TAG,"JNI_OnUnload");
}
