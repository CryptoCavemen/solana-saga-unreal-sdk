//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Bip44DerivationPath.h"

#if PLATFORM_ANDROID
#include "Android/Throwable.h"
#include "Android/JavaUtils.h"
#include "Android/List.h"
#include "Android/BipLevel.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidApplication.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FBip44DerivationPath, FJavaClassObjectWrapper,
		"com/solanamobile/seedvault/Bip44DerivationPath",
		"(Lcom/solanamobile/seedvault/BipLevel;Lcom/solanamobile/seedvault/BipLevel;Lcom/solanamobile/seedvault/BipLevel;)V")
	GetAccountMethod = GetClassMethod("getAccount", "()Lcom/solanamobile/seedvault/BipLevel;");
	HasChangeMethod = GetClassMethod("hasChange", "()Z");
	GetChangeMethod = GetClassMethod("getChange", "()Lcom/solanamobile/seedvault/BipLevel;");
	HasAddressIndexMethod = GetClassMethod("hasAddressIndex", "()Z");
	GetAddressIndexMethod = GetClassMethod("getAddressIndex", "()Lcom/solanamobile/seedvault/BipLevel;");
	GetLevelsMethod = GetClassMethod("getLevels", "()Ljava/util/List;");
	ToUriMethod = GetClassMethod("toUri", "()Landroid/net/Uri;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FJavaClassStaticMethod FBip44DerivationPath::FromUriMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FBip44DerivationPath, "com/solanamobile/seedvault/Bip44DerivationPath")
	FromUriMethod = GetClassStaticMethod(StaticClass, "fromUri", "(Landroid/net/Uri;)Lcom/solanamobile/seedvault/Bip44DerivationPath;");
END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC

TSharedRef<FBipLevel> FBip44DerivationPath::GetAccount()
{
	jobject JBipLevel = CallMethod<jobject>(GetAccountMethod);
	return FBipLevel::CreateFromExisting(JBipLevel);
}

bool FBip44DerivationPath::HasChange()
{
	return CallMethod<bool>(HasChangeMethod);
}

TSharedRef<FBipLevel> FBip44DerivationPath::GetChange()
{
	jobject JBipLevel = CallMethod<jobject>(GetChangeMethod);
	return FBipLevel::CreateFromExisting(JBipLevel);
}

bool FBip44DerivationPath::HasAddressIndex()
{
	return CallMethod<bool>(HasAddressIndexMethod);
}

TSharedRef<FBipLevel> FBip44DerivationPath::GetAddressIndex()
{
	jobject JBipLevel = CallMethod<jobject>(GetAddressIndexMethod);
	return FBipLevel::CreateFromExisting(JBipLevel);	
}

TArray<TSharedRef<FBipLevel>> FBip44DerivationPath::GetLevels()
{
	TArray<TSharedRef<FBipLevel>> Levels;
	auto JLevelsList = FList::CreateFromExisting(CallMethod<jobject>(GetLevelsMethod));

	int32 NumLevels = JLevelsList->Size();
	for (int32 LevelIndex = 0; LevelIndex < NumLevels; LevelIndex++)
	{
		auto Level = FBipLevel::CreateFromExisting(**JLevelsList->Get(LevelIndex));
		Levels.Add(Level);
	}
	
	return Levels;	
}

FString FBip44DerivationPath::ToUri()
{
	return FJavaUtils::JUriToString(CallMethod<jobject>(ToUriMethod));
}

TSharedPtr<FBip44DerivationPath> FBip44DerivationPath::FromUri(const FString& Uri, TSharedPtr<FThrowable>* OutException)
{
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JBip44DerivationPath = CallThrowableStaticMethod<jobject>(JThrowable, FromUriMethod, *FJavaUtils::GetJUri(Uri));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		return !OutException ? CreateFromExisting(JBip44DerivationPath).ToSharedPtr() : nullptr;
	}
	else
	{
		jobject JBip44DerivationPath = CallStaticMethod<jobject>(FromUriMethod, *FJavaUtils::GetJUri(Uri));
		return CreateFromExisting(JBip44DerivationPath);
	}
}

#endif
