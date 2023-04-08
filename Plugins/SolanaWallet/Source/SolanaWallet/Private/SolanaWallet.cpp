// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolanaWallet.h"

#include "Wallet.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"
#endif

// __RESHARPER__ is only defined while in IDE. Used to help editing this file with proper highlighting.
#ifdef __RESHARPER__
#include "C:\Users\Sergey\AppData\Local\Android\Sdk\ndk\25.1.8937393\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\include\jni.h"
#endif

#define LOCTEXT_NAMESPACE "FSolanaWalletModule"

#if PLATFORM_ANDROID

jclass ActivityClass = nullptr;
jmethodID ActivityStartActivityMethod = nullptr;

jclass IntentClass = nullptr;
jmethodID IntentConstructor = nullptr;
jmethodID IntentSetActionMethod = nullptr;
jmethodID IntentSetDataMethod = nullptr;

jclass UriClass = nullptr;
jmethodID UriParseMethod = nullptr;

#endif


void FSolanaWalletModule::StartupModule()
{
	InitJNI();
}

void FSolanaWalletModule::ShutdownModule()
{
}

void FSolanaWalletModule::StartActivity(const FString& Action, const FString& Uri)
{	
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	check(Env);

	// Construct the Intent
	jobject IntentInstance = Env->NewObject(IntentClass, IntentConstructor);
	
	jstring ActionString = Env->NewStringUTF(TCHAR_TO_UTF8(*Action));
	Env->CallObjectMethod(IntentInstance, IntentSetActionMethod, ActionString);

	if (!Uri.IsEmpty())
	{
		jstring UriString = Env->NewStringUTF(TCHAR_TO_UTF8(*Uri));
		jobject UriInstance = Env->CallStaticObjectMethod(UriClass, UriParseMethod, UriString);
		Env->CallObjectMethod(IntentInstance, IntentSetDataMethod, UriInstance);
	}

	// Start the activity
	Env->CallVoidMethod(FAndroidApplication::GetGameActivityThis(), ActivityStartActivityMethod, IntentInstance);

	// Clean up
	Env->DeleteLocalRef(IntentInstance);
#endif
}

void FSolanaWalletModule::RunTest()
{
#if PLATFORM_ANDROID	
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	check(Env);

	jclass WalletClass = FAndroidApplication::FindJavaClass("com/solanamobile/seedvault/Wallet");
	check(WalletClass);
	
	//jclass WalletClass = Env->FindClass("com/solanamobile/seedvault/Wallet");
	//check(WalletClass);

	jmethodID WalletCreateSeedMethod = Env->GetStaticMethodID(WalletClass, "createSeed", "(I)Landroid/content/Intent;");
	check(WalletCreateSeedMethod);

	int32 Value = 0;
	Env->CallStaticObjectMethod(WalletClass, WalletCreateSeedMethod, Value);
#endif
}

void FSolanaWalletModule::InitJNI() const
{
#if PLATFORM_ANDROID
	FWallet::Initialize();
		
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	check(Env);

	ActivityClass = Env->GetObjectClass(FAndroidApplication::GetGameActivityThis());
	ActivityStartActivityMethod = Env->GetMethodID(ActivityClass, "startActivity", "(Landroid/content/Intent;)V");
		
	IntentClass = Env->FindClass("android/content/Intent");
	IntentConstructor = Env->GetMethodID(IntentClass, "<init>", "()V");	
	IntentSetActionMethod = Env->GetMethodID(IntentClass, "setAction", "(Ljava/lang/String;)Landroid/content/Intent;");
	IntentSetDataMethod = Env->GetMethodID(IntentClass, "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");

	UriClass = Env->FindClass("android/net/Uri");
	UriParseMethod = Env->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSolanaWalletModule, SolanaWallet)