//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Android/Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
	
/**
 * Wrapper for com/solanamobile/seedvault/BipLevel.java. 
 */
class FBipLevel : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FBipLevel)
public:
	int32 GetIndex() const;
	bool GetHardened() const;
	FString ToString();
protected:
	FJavaClassField IndexField;
	FJavaClassField HardenedField;
	FJavaClassMethod ToStringMethod;
};

#endif