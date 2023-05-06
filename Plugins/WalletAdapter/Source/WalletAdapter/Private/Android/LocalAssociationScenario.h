//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "MobileWalletAdapterSession.h"
#include "Future.h"
#include "Scenario.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.LocalAssociationScenario 
 */
class FLocalAssociationScenario : public FScenario
{
	DECLARE_JAVA_CLASS_OBJECT(FLocalAssociationScenario, int32 ClientTimeoutMs);
public:
	int32 GetPort();
	TSharedRef<FMobileWalletAdapterSession> GetSession();
	TSharedRef<FFuture> Start();
	TSharedRef<FFuture> Close();

protected:
	FJavaClassMethod GetPortMethod;
	FJavaClassMethod GetSessionMethod;
	FJavaClassMethod StartMethod;
	FJavaClassMethod CloseMethod;
};


#endif