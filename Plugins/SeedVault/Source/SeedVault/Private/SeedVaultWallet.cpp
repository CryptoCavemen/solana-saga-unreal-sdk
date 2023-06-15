//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SeedVaultWallet.h"
#include "SeedVault.h"
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


USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::AuthorizeSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::AuthorizeSeedFailure;
USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::CreateSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::CreateSeedFailure;
USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::ImportSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::ImportSeedFailure;


enum class EActivityRequestCode
{
	REQUEST_AUTHORIZE_SEED_ACCESS = 0,
	REQUEST_CREATE_NEW_SEED = 1,
	REQUEST_IMPORT_EXISTING_SEED = 2,
	REQUEST_SIGN_TRANSACTIONS = 3,
	REQUEST_SIGN_MESSAGES = 4,
	REQUEST_GET_PUBLIC_KEYS = 5
};


void USeedVaultWallet::AuthorizeSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (AuthorizeSeedSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("AuthorizeSeed: Purpose = %d"), Purpose);
	AuthorizeSeedSuccess = Success;
	AuthorizeSeedFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::AuthorizeSeed((int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_AUTHORIZE_SEED_ACCESS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif	
}

void USeedVaultWallet::CreateSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (CreateSeedSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("CreateSeed: Purpose = %d"), Purpose);
	CreateSeedSuccess = Success;
	CreateSeedFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::CreateSeed((int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_CREATE_NEW_SEED, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif	
}

void USeedVaultWallet::ImportSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (ImportSeedSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("ImportSeed: Purpose = %d"), Purpose);
	ImportSeedSuccess = Success;
	ImportSeedFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::ImportSeed((int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_IMPORT_EXISTING_SEED, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif	
}
