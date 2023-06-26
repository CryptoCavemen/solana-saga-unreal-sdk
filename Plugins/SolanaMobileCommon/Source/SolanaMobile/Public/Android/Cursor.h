//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"

/**
 * Wrapper for android.database.Cursor 
 */
class FCursor : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FCursor)
public:
	int32 GetCount();
	int32 GetPosition();
	bool MoveToFirst();
	bool MoveToLast();
	bool MoveToNext();
	TArray<uint8> GetBlob(int32 Var1);
	FString GetString(int32 Var1);
	int16 GetShort(int32 Var1);
	int32 GetInt(int32 Var1);
	int64 GetLong(int32 Var1);
	void Close();
protected:
	FJavaClassMethod GetCountMethod;
	FJavaClassMethod GetPositionMethod;
	FJavaClassMethod MoveToFirstMethod;
	FJavaClassMethod MoveToLastMethod;
	FJavaClassMethod MoveToNextMethod;
	FJavaClassMethod GetBlobMethod;
	FJavaClassMethod GetStringMethod;
	FJavaClassMethod GetShortMethod;
	FJavaClassMethod GetIntMethod;
	FJavaClassMethod GetLongMethod;
	FJavaClassMethod CloseMethod;
};

#endif