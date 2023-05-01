// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobileWalletAdapterSession.h"
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

private:
	FJavaClassMethod GetPortMethod;
	FJavaClassMethod GetSessionMethod;
};


#endif