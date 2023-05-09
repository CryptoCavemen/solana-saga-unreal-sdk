//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#if USE_ANDROID_JNI
#include "Android/AndroidJavaEnv.h"
#include "Defines.h"
#include <jni.h>


class FJavaUtils
{
public:
	static FScopedJavaObject<jstring> GetJString(const FString& String);
	static FScopedJavaObject<jobject> GetJUri(const FString& Uri);
	static FString JUriToString(jobject JUri);
	
	static void VerifyException(JNIEnv* Env);
	static void LogException(JNIEnv* Env);
};

#endif