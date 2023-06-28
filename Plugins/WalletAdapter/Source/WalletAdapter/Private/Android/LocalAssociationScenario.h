//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "Android/Defines.h"
#include "MobileWalletAdapterSession.h"
#include "Scenario.h"

#if PLATFORM_ANDROID
#include "Android/Future.h"
#include "Android/JavaClassObjectWrapper.h"

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