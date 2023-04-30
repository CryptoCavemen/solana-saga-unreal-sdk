// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalAssociationScenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

FLocalAssociationScenario::FLocalAssociationScenario()
	: FScenario()
{
}

FLocalAssociationScenario::~FLocalAssociationScenario()
{
}

FLocalAssociationScenario* FLocalAssociationScenario::Construct(int32 ClientTimeoutMs, ...)
{
	FLocalAssociationScenario* Object = new FLocalAssociationScenario();
	
	va_list Args;
	va_start(Args, ClientTimeoutMs);
	Object->PostConstruct("com/solana/mobilewalletadapter/clientlib/scenario/LocalAssociationScenario", "(I)V", Args);
	va_end(Args);
	
	return Object;
}

void FLocalAssociationScenario::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	FScenario::PostConstruct(ClassName, CtorSig, Args);
}


#endif
