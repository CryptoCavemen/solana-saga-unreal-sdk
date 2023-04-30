// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

#define DEFAULT_CLIENT_TIMEOUT_MS 90000

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FScenario : public FJavaClassObjectEx
{
protected:	
	FScenario();
public:
	virtual ~FScenario() override;
	static FScenario* Construct(int32 ClientTimeoutMs = DEFAULT_CLIENT_TIMEOUT_MS, ...);
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;
};

#endif