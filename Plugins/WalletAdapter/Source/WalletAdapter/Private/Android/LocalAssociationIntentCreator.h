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

class FThrowable;
class FMobileWalletAdapterSession;

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.FLocalAssociationIntentCreator 
 */
class FLocalAssociationIntentCreator : FJavaClassObjectWrapper
{
private:
	FLocalAssociationIntentCreator() {}
	
	DECLARE_JAVA_CLASS_OBJECT_STATIC()
public:
	static FJavaClassObjectWrapperPtr CreateAssociationIntent(const FString& EndpointPrefix, int32 Port, const FMobileWalletAdapterSession& Session, TSharedPtr<FThrowable>* OutException = nullptr);

protected:
	static FJavaClassMethod CreateAssociationIntentMethod;
};
	
}

#endif