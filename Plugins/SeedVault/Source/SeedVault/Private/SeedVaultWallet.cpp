//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SeedVaultWallet.h"
#include "SeedVault.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"
#include "Android/GameActivity.h"
#include "Android/Wallet.h"
#include "Android/Throwable.h"
#endif


USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::AuthorizeSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::AuthorizeSeedFailure;
USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::CreateSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::CreateSeedFailure;
USeedVaultWallet::FSuccessWithTokenDynDelegate USeedVaultWallet::ImportSeedSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::ImportSeedFailure;
USeedVaultWallet::FSignSuccessDynDelegate USeedVaultWallet::SignTransactionsSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::SignTransactionsFailure;
USeedVaultWallet::FSignSuccessDynDelegate USeedVaultWallet::SignMessagesSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::SignMessagesFailure;
USeedVaultWallet::FGetPublicKeysSuccessDynDelegate USeedVaultWallet::GetPublicKeysSuccess;
USeedVaultWallet::FFailureDynDelegate USeedVaultWallet::GetPublicKeysFailure;


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

void USeedVaultWallet::SignTransaction(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Transaction,
	const FSignSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (SignTransactionsSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("SignTransaction request: AuthToken = %d, DerivationPath = '%s'"), AuthToken, *DerivationPath);
	SignTransactionsSuccess = Success;
	SignTransactionsFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::SignTransaction(AuthToken, DerivationPath, Transaction, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_SIGN_TRANSACTIONS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::SignMessage(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Message,
	const FSignSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (SignMessagesSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("SignMessage request: AuthToken = %d, DerivationPath = '%s'"), AuthToken, *DerivationPath);
	SignMessagesSuccess = Success;
	SignMessagesFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::SignMessage(AuthToken, DerivationPath, Message, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_SIGN_MESSAGES, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::RequestPublicKey(int64 AuthToken, const FString& DerivationPath, const FGetPublicKeysSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (GetPublicKeysSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("Public keys request: AuthToken = %d, DerivationPath = '%s'"), AuthToken, *DerivationPath);
	GetPublicKeysSuccess = Success;
	GetPublicKeysFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::RequestPublicKey(AuthToken, DerivationPath, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_GET_PUBLIC_KEYS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::RequestPublicKeys(int64 AuthToken, const TArray<FString>& DerivationPaths,
                                         const FGetPublicKeysSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (GetPublicKeysSuccess.IsBound())
	{
		UE_LOG(LogSeedVault, Warning, TEXT("Received a request while another is pending"));
		return;
	}
	
	UE_LOG(LogSeedVault, Log, TEXT("Public keys request: AuthToken = %d"), AuthToken);
	GetPublicKeysSuccess = Success;
	GetPublicKeysFailure = Failure;

	TSharedPtr<FThrowable> Exception;
	FJavaClassObjectWrapperPtr Intent = FWallet::RequestPublicKeys(AuthToken, DerivationPaths, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during intent creation: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(Intent.IsValid());
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_GET_PUBLIC_KEYS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

bool USeedVaultWallet::DeauthorizeSeed(int64 AuthToken)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	return FWallet::DeauthorizeSeed(AppContext, AuthToken, &Exception);
#endif
	return false;
}

bool USeedVaultWallet::HasUnauthorizedSeedsForPurpose(EWalletContractV1 Purpose)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	bool Result = FWallet::HasUnauthorizedSeedsForPurpose(AppContext, (int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during HasUnauthorizedSeedsForPurpose call: %s"), *Exception->GetMessage());
		return false;
	}
	return Result;
#endif

	return false;
}
