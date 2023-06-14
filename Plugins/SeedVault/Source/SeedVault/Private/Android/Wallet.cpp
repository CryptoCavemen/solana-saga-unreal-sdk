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

FJavaClassMethod FWallet::CreateSeedMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FWallet, "com/solanamobile/seedvault/Wallet")
	CreateSeedMethod = GetClassStaticMethod("createSeed", "(I)Landroid/content/Intent;");
END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC

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
#endif
