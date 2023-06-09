﻿//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/JavaUtils.h"
#include "SolanaMobile.h"

#if PLATFORM_ANDROID

FScopedJavaObject<jstring> FJavaUtils::GetJString(const FString& String)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	return FJavaHelper::ToJavaString(Env, String);
}

FScopedJavaObject<jobjectArray> FJavaUtils::GetJStringArray(const TArray<FString>& Strings)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	jclass ArrayClass = Env->FindClass("java/lang/String");
	jobjectArray JStringsArray = Env->NewObjectArray(Strings.Num(), ArrayClass, nullptr);
	Env->DeleteLocalRef(ArrayClass);

	for (int32 Index = 0; Index < Strings.Num(); Index++)
	{
		Env->SetObjectArrayElement(JStringsArray, Index, *FJavaHelper::ToJavaString(Env, Strings[Index]));
		VerifyException(Env);
	}

	return NewScopedJavaObject(Env, JStringsArray);
}

FScopedJavaObject<jobject> FJavaUtils::GetJUri(const FString& Uri)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	if (Uri.IsEmpty())
		return FScopedJavaObject<jobject>(Env, nullptr);
	
	jclass UriClass = Env->FindClass("android/net/Uri");
	jmethodID UriParseMethod = Env->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	jobject JUri = Env->CallStaticObjectMethod(UriClass, UriParseMethod, *FJavaHelper::ToJavaString(Env, Uri));
	VerifyException(Env);
	return NewScopedJavaObject(Env, JUri);
}

FScopedJavaObject<jbyteArray> FJavaUtils::GetByteArray(const TArray<uint8>& Bytes)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	if (!Bytes.Num())
		return FScopedJavaObject<jbyteArray>(Env, nullptr);
	
	jbyteArray JByteArray = Env->NewByteArray(Bytes.Num());
	jbyte* Buffer = Env->GetByteArrayElements(JByteArray, nullptr);
	FMemory::Memcpy(Buffer, Bytes.GetData(), Bytes.Num());
	Env->ReleaseByteArrayElements(JByteArray, Buffer, 0);

	return NewScopedJavaObject(Env, JByteArray);
}

FScopedJavaObject<jobjectArray> FJavaUtils::GetArrayOfByteArray(const TArray<TArray<uint8>>& ByteArray)
{	
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	if (!ByteArray.Num())
		return FScopedJavaObject<jobjectArray>(Env, nullptr);
		
	jclass ArrayClass = Env->FindClass("[B");
	jobjectArray JByteArray = Env->NewObjectArray(ByteArray.Num(), ArrayClass, Env->NewByteArray(1));
	Env->DeleteLocalRef(ArrayClass);

	for (int32 Index = 0; Index < ByteArray.Num(); Index++)
	{   
		auto JBytes = GetByteArray(ByteArray[Index]);
		Env->SetObjectArrayElement(JByteArray, Index, *JBytes);
		VerifyException(Env);
	}

	return NewScopedJavaObject(Env, JByteArray);	
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

TArray<uint8> FJavaUtils::JByteArrayToTArray(jbyteArray JByteArray)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	jbyte* ArrayDataPtr = Env->GetByteArrayElements(JByteArray, 0);
	int32 ArraySize = Env->GetArrayLength(JByteArray);

	TArray<uint8> ByteArray;
	
	if (JByteArray != nullptr)
	{
		ByteArray.SetNumUninitialized(ArraySize);
		memcpy(ByteArray.GetData(), ArrayDataPtr, ArraySize);
		Env->ReleaseByteArrayElements(JByteArray, ArrayDataPtr, JNI_ABORT);
	}
	
	return ByteArray;
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
		UE_LOG(LogSolanaMobile, Warning, TEXT("Java JNI call failed with an exception."));
	}	
}

#endif