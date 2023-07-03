//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Android/Defines.h"
#include "CoreMinimal.h"
#include "BipLevel.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"

class FThrowable;

/**
 * Wrapper for com/solanamobile/seedvault/Bip44DerivationPath.java. 
 */
class FBip44DerivationPath : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FBip44DerivationPath)
	DECLARE_JAVA_CLASS_OBJECT_STATIC()
public:
	TSharedRef<FBipLevel> GetAccount();	
	bool HasChange();
	TSharedRef<FBipLevel> GetChange();
	bool HasAddressIndex();
	TSharedRef<FBipLevel> GetAddressIndex();
	TArray<TSharedRef<FBipLevel>> GetLevels();
	FString ToUri();
	static TSharedPtr<FBip44DerivationPath> FromUri(const FString& Uri, TSharedPtr<FThrowable>* OutException);
protected:
	FJavaClassMethod GetAccountMethod;
	FJavaClassMethod HasChangeMethod;
	FJavaClassMethod GetChangeMethod;
	FJavaClassMethod HasAddressIndexMethod;
	FJavaClassMethod GetAddressIndexMethod;
	FJavaClassMethod GetLevelsMethod;
	FJavaClassMethod ToUriMethod;
	static FJavaClassStaticMethod FromUriMethod;
};

#endif