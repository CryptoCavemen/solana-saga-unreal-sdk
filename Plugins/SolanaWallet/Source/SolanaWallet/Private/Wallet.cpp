// Fill out your copyright notice in the Description page of Project Settings.


#include "Wallet.h"

#if PLATFORM_ANDROID && USE_ANDROID_JNI
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"

static jclass WalletClass;
static jmethodID WalletCreateSeedMethod;
static jmethodID WalletOnCreateSeedResult;

#endif

const char* UWallet::GetClassName()
{
	return "com/solanamobile/seedvault/Wallet";
}

void UWallet::Initialize()
{
#if PLATFORM_ANDROID && USE_ANDROID_JNI	
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	WalletClass = FAndroidApplication::FindJavaClassGlobalRef(GetClassName());
	//jclass WalletClass = Env->FindClass("com/solanamobile/seedvault/Wallet");
	check(WalletClass);

	WalletCreateSeedMethod = Env->GetStaticMethodID(WalletClass, "createSeed", "(I)Landroid/content/Intent;");
	check(WalletCreateSeedMethod);

	WalletOnCreateSeedResult = Env->GetStaticMethodID(WalletClass, "onCreateSeedResult", "(ILandroid/content/Intent;)I");
	check(WalletOnCreateSeedResult);
#endif
}

FIntent UWallet::CreateSeed(int32 Purpose)
{
#if PLATFORM_ANDROID && USE_ANDROID_JNI
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	return FIntent(Env->CallStaticObjectMethod(WalletClass, WalletCreateSeedMethod, Purpose));
#else
	return FIntent();
#endif	
}

int64 UWallet::OnCreateSeedResult(int32 ResultCode, FIntent Intent)
{
#if PLATFORM_ANDROID && USE_ANDROID_JNI
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jlong Value = Env->CallStaticLongMethod(WalletClass, WalletOnCreateSeedResult, ResultCode, Intent);
	return Value; 
#else
	return -1;
#endif
}
