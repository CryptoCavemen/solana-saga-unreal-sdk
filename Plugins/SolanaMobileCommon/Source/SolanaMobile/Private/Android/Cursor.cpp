//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Cursor.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FCursor, FJavaClassObjectWrapper, "android/database/Cursor", "()V")
	GetCountMethod = GetClassMethod("getCount", "()I");
	GetPositionMethod = GetClassMethod("getPosition", "()I");
	MoveToFirstMethod = GetClassMethod("moveToFirst", "()Z");
	MoveToLastMethod = GetClassMethod("moveToLast", "()Z");
	MoveToNextMethod = GetClassMethod("moveToNext", "()Z");
	GetBlobMethod = GetClassMethod("getBlob", "(I)[B");
	GetStringMethod = GetClassMethod("getString", "(I)Ljava/lang/String;");
	GetShortMethod = GetClassMethod("getShort", "(I)S");
	GetIntMethod = GetClassMethod("getInt", "(I)I");
	GetLongMethod = GetClassMethod("getLong", "(I)J");
	CloseMethod = GetClassMethod("close", "()V");
END_IMPLEMENT_JAVA_CLASS_OBJECT

int32 FCursor::GetCount()
{
	return CallMethod<int32>(GetCountMethod);
}

int32 FCursor::GetPosition()
{
	return CallMethod<int32>(GetPositionMethod);
}

bool FCursor::MoveToFirst()
{
	return CallMethod<bool>(MoveToFirstMethod);
}

bool FCursor::MoveToLast()
{
	return CallMethod<bool>(MoveToLastMethod);
}

bool FCursor::MoveToNext()
{
	return CallMethod<bool>(MoveToNextMethod);
}

TArray<uint8> FCursor::GetBlob(int32 Var1)
{
	return CallMethod<TArray<uint8>>(GetBlobMethod, Var1);
}

FString FCursor::GetString(int32 Var1)
{
	return CallMethod<FString>(GetStringMethod, Var1);
}

int16 FCursor::GetShort(int32 Var1)
{
	return CallMethod<int16>(GetShortMethod, Var1);
}

int32 FCursor::GetInt(int32 Var1)
{
	return CallMethod<int32>(GetIntMethod, Var1);
}

int64 FCursor::GetLong(int32 Var1)
{
	return CallMethod<int64>(GetLongMethod, Var1);
}

void FCursor::Close()
{
	return CallMethod<void>(CloseMethod);
}

#endif
