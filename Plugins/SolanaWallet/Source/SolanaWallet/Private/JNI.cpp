#if PLATFORM_ANDROID
/*
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android/log.h>

#undef LOG_TAG
#define LOG_TAG "WaveHMD"

JavaVM* mJavaVM;
JNIEnv* JavaENV = NULL;

jclass mFUClass;
jobject mFUObject;

extern "C" void Java_com_solana_ue_Wallet_initNative(JNIEnv* LocalJNIEnv, jobject LocalThiz) {
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Java_com_solana_ue_Wallet_initNative");

	JavaENV = LocalJNIEnv;// FAndroidApplication::GetJavaEnv();

	JavaENV->GetJavaVM(&mJavaVM);

	if (mJavaVM == nullptr) {
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Java VM is null!!");
		return;
	}

	mFUObject = JavaENV->NewGlobalRef(LocalThiz);

	jclass localClass = JavaENV->FindClass("com/solana/ue/Wallet");
	if (localClass == nullptr) {
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s: Can't find Java Class - com/solana/ue/Wallet.", __func__);
		return;
	}

	mFUClass = reinterpret_cast<jclass>(JavaENV->NewGlobalRef(localClass));
}

*/
#endif  // PLATFORM_ANDROID
