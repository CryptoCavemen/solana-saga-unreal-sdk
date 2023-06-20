//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "PublicKeyResponseWrapper.h"

#include "JavaUtils.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

using namespace SeedVault;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FPublicKeyResponseWrapper, FJavaClassObjectWrapper, "com/solanamobile/seedvault/PublicKeyResponse", "([BLjava/lang/String;Landroid/net/Uri;)V")
	ResolvedDerivationPathField = GetClassField("resolvedDerivationPath", "Landroid/net/Uri;");
	GetPublicKeyMethod = GetClassMethod("getPublicKey", "()[B");
	GetPublicKeyEncodedMethod = GetClassMethod("getPublicKeyEncoded", "()Ljava/lang/String;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FString FPublicKeyResponseWrapper::GetResolvedDerivationPath()
{
	jobject JUri = GetObjectField(ResolvedDerivationPathField);
	return FJavaUtils::JUriToString(JUri);
}

TArray<uint8> FPublicKeyResponseWrapper::GetPublicKey()
{
	jobjectArray JKey = CallMethod<jobjectArray>(GetPublicKeyMethod);
	TArray<uint8> Key = FJavaUtils::JByteArrayToTArray((jbyteArray)JKey);
	return Key;
}

FString FPublicKeyResponseWrapper::GetPublicKeyEncoded()
{
	return CallMethod<FString>(GetPublicKeyEncodedMethod);
}

#endif
