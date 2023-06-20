//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"

namespace SeedVault
{
	
/**
 * Wrapper for com/solanamobile/seedvault/PublicKeyResponse.java. 
 */
class FPublicKeyResponseWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FPublicKeyResponseWrapper);
public:
	FString GetResolvedDerivationPath();
	TArray<uint8> GetPublicKey();
	FString GetPublicKeyEncoded();
protected:
	FJavaClassField ResolvedDerivationPathField;
	FJavaClassMethod GetPublicKeyMethod;
	FJavaClassMethod GetPublicKeyEncodedMethod;
};
	
}

#endif