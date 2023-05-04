//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Future.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FMobileWalletAdapterClient : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, int32 ClientTimeoutMs);
public:
	TSharedRef<FFuture> Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster);
protected:
	FJavaClassMethod AuthorizeMethod;	
};

#endif