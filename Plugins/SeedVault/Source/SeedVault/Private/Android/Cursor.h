﻿//
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
 * Wrapper for android.database.Cursor 
 */
class FCursor : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FCursor);
public:
	int32 GetCount();
	int32 GetPosition();
	TArray<uint8> GetBlob(int32 var1);
	FString GetString(int32 var1);
	int16 GetShort(int32 var1);
	int32 GetInt(int32 var1);
	int64 GetLong(int32 var1);
	void Close();
protected:
	FJavaClassMethod GetCountMethod;
	FJavaClassMethod GetPositionMethod;
	FJavaClassMethod GetBlobMethod;
	FJavaClassMethod GetStringMethod;
	FJavaClassMethod GetShortMethod;
	FJavaClassMethod GetIntMethod;
	FJavaClassMethod GetLongMethod;
	FJavaClassMethod CloseMethod;
};
	
}

#endif