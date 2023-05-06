//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociationScenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FLocalAssociationScenario, FScenario, "com/solana/mobilewalletadapter/clientlib/scenario/LocalAssociationScenario", "(I)V", int ClientTimeoutMs)
	GetPortMethod = GetClassMethod("getPort", "()I");
	GetSessionMethod = GetClassMethod("getSession", "()Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterSession;");
	StartMethod = GetClassMethod("start", "()Lcom/solana/mobilewalletadapter/common/util/NotifyOnCompleteFuture;");
	CloseMethod = GetClassMethod("close", "()Lcom/solana/mobilewalletadapter/common/util/NotifyOnCompleteFuture;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

int32 FLocalAssociationScenario::GetPort()
{
	return CallMethod<int32>(GetPortMethod);
}

TSharedRef<FMobileWalletAdapterSession> FLocalAssociationScenario::GetSession()
{
	jobject JObject = CallMethod<jobject>(GetSessionMethod);
	return FMobileWalletAdapterSession::MakeFromExistingObject(JObject);
}

TSharedRef<FFuture> FLocalAssociationScenario::Start()
{	
	jobject RetVal = CallMethod<jobject>(StartMethod);
	return FFuture::MakeFromExistingObject(RetVal);
}

TSharedRef<FFuture> FLocalAssociationScenario::Close()
{	
	jobject RetVal = CallMethod<jobject>(CloseMethod);
	return FFuture::MakeFromExistingObject(RetVal);
}

#endif
