//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Wallet.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

using namespace SeedVault;

FJavaClassMethod FWallet::AuthorizeSeedMethod;
FJavaClassMethod FWallet::CreateSeedMethod;
FJavaClassMethod FWallet::ImportSeedMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FWallet, "com/solanamobile/seedvault/Wallet")
	AuthorizeSeedMethod = GetClassStaticMethod("authorizeSeed", "(I)Landroid/content/Intent;");
	CreateSeedMethod = GetClassStaticMethod("createSeed", "(I)Landroid/content/Intent;");
	ImportSeedMethod = GetClassStaticMethod("importSeed", "(I)Landroid/content/Intent;");
END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC

FJavaClassObjectWrapperPtr FWallet::AuthorizeSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, AuthorizeSeedMethod, Purpose);
		if (JThrowable)
		{
			*OutException = MakeShareable(FThrowable::Construct(JThrowable));
		}
		else
		{
			*OutException = nullptr;
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
		}
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(AuthorizeSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;	
}

FJavaClassObjectWrapperPtr FWallet::CreateSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, CreateSeedMethod, Purpose);
		if (JThrowable)
		{
			*OutException = MakeShareable(FThrowable::Construct(JThrowable));
		}
		else
		{
			*OutException = nullptr;
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
		}
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(CreateSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

FJavaClassObjectWrapperPtr FWallet::ImportSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, ImportSeedMethod, Purpose);
		if (JThrowable)
		{
			*OutException = MakeShareable(FThrowable::Construct(JThrowable));
		}
		else
		{
			*OutException = nullptr;
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
		}
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(ImportSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}
#endif
