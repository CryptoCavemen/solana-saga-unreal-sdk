//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"
#include <jni.h>

namespace WalletAdapter
{

class FJavaUtils
{
public:
	static FScopedJavaObject<jstring> GetJString(const FString& String);
	static FScopedJavaObject<jobject> GetJUri(const FString& Uri);
	static FScopedJavaObject<jbyteArray> GetByteArray(const TArray<uint8>& Bytes);
	static FScopedJavaObject<jobjectArray> GetArrayOfByteArray(const TArray<TArray<uint8>>& ByteArray);
	static FString JUriToString(jobject JUri);
	static TArray<uint8> JByteArrayToTArray(jbyteArray JByteArray);
	
	static void VerifyException(JNIEnv* Env);
	static void LogException(JNIEnv* Env);
};

}

#endif