//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "JavaUtils.h"

#if USE_ANDROID_JNI

FScopedJavaObject<jstring> FJavaUtils::GetJString(const FString& String)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	return FJavaHelper::ToJavaString(Env, String);
}

FScopedJavaObject<jobject> FJavaUtils::GetJUri(const FString& Uri)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	if (Uri.IsEmpty())
		return FScopedJavaObject<jobject>(Env, nullptr);
	
	jclass UriClass = Env->FindClass("android/net/Uri");
	jmethodID UriParseMethod = Env->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	jobject JUri = Env->CallStaticObjectMethod(UriClass, UriParseMethod, *FJavaHelper::ToJavaString(Env, Uri));
	LogException(Env);
	return NewScopedJavaObject(Env, JUri);
}

FString FJavaUtils::JUriToString(jobject JUri)
{
	if (!JUri)
		return FString();
		
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	jclass UriClass = Env->FindClass("android/net/Uri");
	jmethodID ToStringMethod = Env->GetMethodID(UriClass, "toString", "()Ljava/lang/String;");
	jstring JString = static_cast<jstring>(Env->CallObjectMethod(JUri, ToStringMethod));
	LogException(Env);
	return FJavaHelper::FStringFromLocalRef(Env, JString);
}

void FJavaUtils::VerifyException(JNIEnv* Env)
{
	if (Env->ExceptionCheck())
	{
		Env->ExceptionDescribe();
		Env->ExceptionClear();
		verify(false && "Java JNI call failed with an exception.");
	}
}

void FJavaUtils::LogException(JNIEnv* Env)
{
	if (Env->ExceptionCheck())
	{
		Env->ExceptionDescribe();
		Env->ExceptionClear();
		UE_LOG(LogAndroid, Warning, TEXT("Java JNI call failed with an exception."));
	}	
}

#endif