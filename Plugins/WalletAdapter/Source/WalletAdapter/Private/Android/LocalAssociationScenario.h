// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scenario.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.LocalAssociationScenario 
 */
class FLocalAssociationScenario : public FScenario
{
protected:
	FLocalAssociationScenario();
public:
	virtual ~FLocalAssociationScenario() override;
	static FLocalAssociationScenario* Construct(int32 ClientTimeoutMs = DEFAULT_CLIENT_TIMEOUT_MS, ...);
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;
};


#endif