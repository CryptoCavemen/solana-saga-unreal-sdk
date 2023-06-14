//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"

namespace WalletAdapter
{
	
class FMobileWalletAdapterSession;

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.FLocalAssociationIntentCreator 
 */
class FLocalAssociationIntentCreator	
{
private:
	FLocalAssociationIntentCreator() {}
public:
	/** Initialize java objects and cache them for further usage. Called when the module is loaded. */
	static void StaticConstruct();	

	static FJavaClassObjectWrapperRef CreateAssociationIntent(const FString& EndpointPrefix, int32 Port, const FMobileWalletAdapterSession& Session);	
};
	
}

#endif