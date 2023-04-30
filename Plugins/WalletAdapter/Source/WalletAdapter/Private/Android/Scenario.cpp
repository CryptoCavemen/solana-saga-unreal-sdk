// Fill out your copyright notice in the Description page of Project Settings.

#include "Scenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

FScenario::FScenario()
{
}

FScenario::~FScenario()
{
}

FScenario* FScenario::Construct(int32 ClientTimeoutMs, ...)
{
	FScenario* Object = new FScenario();
	
	va_list Args;
	va_start(Args, ClientTimeoutMs);
	Object->PostConstruct("com/solana/mobilewalletadapter/clientlib/scenario/Scenario", "(I)V", Args);
	va_end(Args);
	
	return Object;
}

void FScenario::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	FJavaClassObjectEx::PostConstruct(ClassName, CtorSig, Args);
}

#endif
