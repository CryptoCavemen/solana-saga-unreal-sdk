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
class FArrayList;
	
/**
 * Wrapper for com/solanamobile/seedvault/SigningResponse.java. 
 */
class FSigningResponseWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSigningResponseWrapper);
public:
	TSharedRef<FArrayList> GetSignatures();
	TSharedRef<FArrayList> GetResolvedDerivationPaths();
protected:
	FJavaClassMethod GetSignaturesMethod;
	FJavaClassMethod GetResolvedDerivationPathsMethod;
};
	
}

#endif