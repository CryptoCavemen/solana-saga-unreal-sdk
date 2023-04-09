﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Wallet.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"

jclass ActivityClass = nullptr;
jmethodID ActivityStartActivityMethod = nullptr;
jmethodID ActivityStartActivityForResultMethod = nullptr;

static jclass WalletClass;
static jmethodID WalletCreateSeedMethod;

#endif


enum class EActivityRequestCode
{
	REQUEST_AUTHORIZE_SEED_ACCESS = 0,
	REQUEST_CREATE_NEW_SEED = 1,
	REQUEST_IMPORT_EXISTING_SEED = 2,
	REQUEST_SIGN_TRANSACTIONS = 3,
	REQUEST_SIGN_MESSAGES = 4,
	REQUEST_GET_PUBLIC_KEYS = 5
};

const char* UWallet::GetClassName()
{
	return "com/solanamobile/seedvault/Wallet";
}

void UWallet::Initialize()
{
#if PLATFORM_ANDROID	
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	ActivityClass = Env->GetObjectClass(FAndroidApplication::GetGameActivityThis());
	check(ActivityClass);
	ActivityStartActivityMethod = Env->GetMethodID(ActivityClass, "startActivity", "(Landroid/content/Intent;)V");
	check(ActivityStartActivityMethod);
	ActivityStartActivityForResultMethod = Env->GetMethodID(ActivityClass, "startActivityForResult", "(Landroid/content/Intent;I)V");
	check(ActivityStartActivityForResultMethod);	

	WalletClass = FAndroidApplication::FindJavaClassGlobalRef(GetClassName());
	check(WalletClass);
	WalletCreateSeedMethod = Env->GetStaticMethodID(WalletClass, "createSeed", "(I)Landroid/content/Intent;");
	check(WalletCreateSeedMethod);
#endif
}

void UWallet::StartActivityForResult(FIntent Intent, int32 RequestCode)
{
#if PLATFORM_ANDROID	
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	Env->CallVoidMethod(FAndroidApplication::GetGameActivityThis(), ActivityStartActivityForResultMethod, Intent.JObject, RequestCode);
	AndroidJavaEnv::CheckJavaException();
#endif
}

FIntent UWallet::CreateSeed(EWalletContractV1 Purpose)
{
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	FIntent Intent = Env->CallStaticObjectMethod(WalletClass, WalletCreateSeedMethod, (int32)Purpose);
	AndroidJavaEnv::CheckJavaException();
	if (Intent.JObject)
		StartActivityForResult(Intent, (int32)EActivityRequestCode::REQUEST_CREATE_NEW_SEED);
	return Intent;
#else
	return FIntent();
#endif	
}

void UWallet::OnCreateSeed(int64 AuthToken)
{
	UE_LOG(LogTemp, Log, TEXT("OnCreateSeed(): AuthToken = %lld"), AuthToken);	
}
