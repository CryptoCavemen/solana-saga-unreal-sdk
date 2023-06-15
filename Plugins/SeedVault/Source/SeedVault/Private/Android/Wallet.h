//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Throwable.h"
#include "JavaClassObjectWrapper.h"

namespace SeedVault
{

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
class FWallet : FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT_STATIC()
public:
	static FJavaClassObjectWrapperPtr AuthorizeSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);
	static FJavaClassObjectWrapperPtr CreateSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);
	static FJavaClassObjectWrapperPtr ImportSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);
protected:
	static FJavaClassMethod AuthorizeSeedMethod;
	static FJavaClassMethod CreateSeedMethod;
	static FJavaClassMethod ImportSeedMethod;
};
	
}

#endif