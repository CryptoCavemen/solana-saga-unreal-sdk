//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Android/Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/ArrayList.h"
#include "Android/JavaClassObjectWrapper.h"

/**
 * Wrapper for com/solanamobile/seedvault/SigningResponse.java. 
 */
class FSigningResponseWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSigningResponseWrapper)
public:
	TSharedRef<FArrayList> GetSignatures();
	TSharedRef<FArrayList> GetResolvedDerivationPaths();
protected:
	FJavaClassMethod GetSignaturesMethod;
	FJavaClassMethod GetResolvedDerivationPathsMethod;
};

#endif