//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "BipLevel.h"

#if PLATFORM_ANDROID
#include "Android/JavaUtils.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FBipLevel, FJavaClassObjectWrapper, "com/solanamobile/seedvault/BipLevel", "(IZ)V")
	IndexField = GetClassField("index", "I");
	HardenedField = GetClassField("hardened", "Z");
END_IMPLEMENT_JAVA_CLASS_OBJECT

int32 FBipLevel::GetIndex() const
{
	return GetIntField(IndexField);
}

bool FBipLevel::GetHardened() const
{
	return GetBooleanField(HardenedField);
}

#endif
