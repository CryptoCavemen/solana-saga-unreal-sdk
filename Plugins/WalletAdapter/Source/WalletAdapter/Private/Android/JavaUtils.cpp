//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "JavaUtils.h"

#if PLATFORM_ANDROID

using namespace WalletAdapter;

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
		
	jclass ArrayElemClass = Env->FindClass("[B");
	jobjectArray JByteArray = Env->NewObjectArray(ByteArray.Num(), ArrayElemClass, Env->NewByteArray(1));
	Env->DeleteLocalRef(ArrayElemClass);

	for (int32 Index = 0; Index < ByteArray.Num(); Index++)
	{   
		auto JBytes = GetByteArray(ByteArray[Index]);
		Env->SetObjectArrayElement(JByteArray, Index, *JBytes);
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