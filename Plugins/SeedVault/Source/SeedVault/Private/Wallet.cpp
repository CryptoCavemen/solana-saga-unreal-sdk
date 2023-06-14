//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Wallet.h"
#include "Android/Defines.h"
#include "Android/GameActivity.h"
#include "Android/Wallet.h"
#include "Android/Throwable.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"
#endif

#if PLATFORM_ANDROID
using namespace SeedVault;
#endif


UWallet::FCreateSeedCallback UWallet::OnCreateSeedCallback;


enum class EActivityRequestCode
{
	REQUEST_AUTHORIZE_SEED_ACCESS = 0,
	REQUEST_CREATE_NEW_SEED = 1,
	REQUEST_IMPORT_EXISTING_SEED = 2,
	REQUEST_SIGN_TRANSACTIONS = 3,
	REQUEST_SIGN_MESSAGES = 4,
	REQUEST_GET_PUBLIC_KEYS = 5
};


void UWallet::CreateSeed(FCreateSeedCallback FinishCallback, EWalletContractV1 Purpose)
{
#if PLATFORM_ANDROID
	UE_LOG(LogSeedVault, Log, TEXT("CreateSeed: Purpose = %d"), Purpose);
	OnCreateSeedCallback = FinishCallback;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::CreateSeed((int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		//Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_CREATE_NEW_SEED, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
	}
#endif	
}

void UWallet::OnCreateSeed(bool bSuccess, int64 AuthToken)
{
	if (bSuccess)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Seed created: AuthToken = %lld"), AuthToken);
	}
	else
	{
		UE_LOG(LogSeedVault, Error, TEXT("Seed creation failed"));		
	}

	AsyncTask(ENamedThreads::GameThread, [&bSuccess, &AuthToken] {
		OnCreateSeedCallback.ExecuteIfBound(bSuccess, AuthToken);
	});
}
