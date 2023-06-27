//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SeedVaultWallet.h"
#include "SeedVault.h"
#include "Android/Cursor.h"

#if PLATFORM_ANDROID
#include "Android/GameActivity.h"
#include "Android/Wallet.h"
#include "Android/Throwable.h"
#include "Android/Bip44DerivationPath.h"
#include "Android/BipLevel.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#endif

#define ACCOUNTS_ACCOUNT_ID					TEXT("_id")
#define ACCOUNTS_BIP32_DERIVATION_PATH		TEXT("Accounts_Bip32DerivationPath")
#define ACCOUNTS_PUBLIC_KEY_RAW				TEXT("Accounts_PublicKeyRaw")
#define ACCOUNTS_PUBLIC_KEY_ENCODED			TEXT("Accounts_PublicKeyEncoded")
#define ACCOUNTS_ACCOUNT_NAME				TEXT("Accounts_AccountName")
#define ACCOUNTS_ACCOUNT_IS_USER_WALLET		TEXT("Accounts_IsUserWallet")
#define ACCOUNTS_ACCOUNT_IS_VALID			TEXT("Accounts_IsValid")

#define AUTHORIZED_SEEDS_AUTH_TOKEN			TEXT("_id")
#define AUTHORIZED_SEEDS_AUTH_PURPOSE		TEXT("AuthorizedSeeds_AuthPurpose")
#define AUTHORIZED_SEEDS_SEED_NAME			TEXT("AuthorizedSeeds_SeedName")


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


void USeedVaultWallet::AuthorizeSeed(EWalletContractV1 Purpose, FSuccessWithTokenDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_AUTHORIZE_SEED_ACCESS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::CreateSeed(EWalletContractV1 Purpose, FSuccessWithTokenDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_CREATE_NEW_SEED, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::ImportSeed(EWalletContractV1 Purpose, FSuccessWithTokenDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_IMPORT_EXISTING_SEED, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::SignTransaction(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Transaction, FSignSuccessDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_SIGN_TRANSACTIONS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::SignMessage(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Message, FSignSuccessDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_SIGN_MESSAGES, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::RequestPublicKey(int64 AuthToken, const FString& DerivationPath, FGetPublicKeysSuccessDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_GET_PUBLIC_KEYS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

void USeedVaultWallet::RequestPublicKeys(int64 AuthToken, const TArray<FString>& DerivationPaths, FGetPublicKeysSuccessDynDelegate Success, FFailureDynDelegate Failure)
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
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	Activity->StartActivityForResult(Intent.ToSharedRef(), (int32)EActivityRequestCode::REQUEST_GET_PUBLIC_KEYS, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to start the activity: %s"), *Exception->GetMessage());
		Failure.ExecuteIfBound(Exception->GetMessage());
	}
#endif
}

TArray<FWalletAccount> USeedVaultWallet::GetAccounts(int64 AuthToken)
{
	TArray<FWalletAccount> Accounts;

#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	
	TArray<FString> Columns;
	Columns.Add(ACCOUNTS_ACCOUNT_ID);
	Columns.Add(ACCOUNTS_BIP32_DERIVATION_PATH);
	Columns.Add(ACCOUNTS_PUBLIC_KEY_RAW);
	Columns.Add(ACCOUNTS_PUBLIC_KEY_ENCODED);
	Columns.Add(ACCOUNTS_ACCOUNT_NAME);
	Columns.Add(ACCOUNTS_ACCOUNT_IS_USER_WALLET);
	Columns.Add(ACCOUNTS_ACCOUNT_IS_VALID);
	
	auto AccountsCursor = FWallet::GetAccounts(AppContext, AuthToken, Columns, &Exception);

	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to request the accounts list: %s"), *Exception->GetMessage());
		return Accounts;
	}

	while (AccountsCursor->MoveToNext())
	{
		FWalletAccount Account;
		Account.AccountId = AccountsCursor->GetLong(0);
		Account.DerivationPath = AccountsCursor->GetString(1);
		Account.PublicKeyRaw = AccountsCursor->GetBlob(2);
		Account.PublicKeyEncoded = AccountsCursor->GetString(3);
		Account.Name = AccountsCursor->GetString(4);
		Account.bIsUserWallet = AccountsCursor->GetShort(5) == 1 ? true : false;
		Account.bIsValid = AccountsCursor->GetShort(6) == 1 ? true : false;
		Accounts.Add(Account);
	}

#else
	
	/*
	// FOR TEST PURPOSES
	FWalletAccount Account;
	Account.AccountId = 0;
	Account.DerivationPath = "1000/000/0";
	Account.PublicKeyEncoded = "fdfdfdfddf";
	Account.Name = "AccountName 1";
	Accounts.Add(Account);
	Account.AccountId = 0;
	Account.DerivationPath = "1000/000/1";
	Account.PublicKeyEncoded = "fdfdfdfddf";
	Account.Name = "AccountName 2";
	Accounts.Add(Account);	
	*/
	
	UE_LOG(LogSeedVault, Error, TEXT("SeedVault: platform is not supported"));
#endif

	return Accounts;
}

bool USeedVaultWallet::GetAccount(int64 AuthToken, int32 AccountId, FWalletAccount& OutAccount)
{
#if PLATFORM_ANDROID
	auto AccountBipLevel = FBipLevel::CreateInstance(AccountId, true);
	auto DerivationPath = FBip44DerivationPath::CreateInstance(**AccountBipLevel, nullptr, nullptr);

	FString DerivationPathUri = DerivationPath->ToUri();
	FString ResolvedDerivationPath = ResolveDerivationPath(DerivationPathUri, EWalletContractV1::PURPOSE_SIGN_SOLANA_TRANSACTION);
	UE_LOG(LogSeedVault, Log, TEXT("Resolved BIP derivation path '%s' to BIP32 derivation path '%s' for purpose PURPOSE_SIGN_SOLANA_TRANSACTION"),
		*DerivationPathUri, *ResolvedDerivationPath);

	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	
	TArray<FString> Columns;
	Columns.Add(ACCOUNTS_ACCOUNT_ID);
	Columns.Add(ACCOUNTS_BIP32_DERIVATION_PATH);
	Columns.Add(ACCOUNTS_PUBLIC_KEY_RAW);
	Columns.Add(ACCOUNTS_PUBLIC_KEY_ENCODED);
	Columns.Add(ACCOUNTS_ACCOUNT_NAME);
	Columns.Add(ACCOUNTS_ACCOUNT_IS_USER_WALLET);
	Columns.Add(ACCOUNTS_ACCOUNT_IS_VALID);
	
	auto Cursor = FWallet::GetAccounts(AppContext, AuthToken, Columns, ACCOUNTS_BIP32_DERIVATION_PATH, ResolvedDerivationPath, &Exception);

	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to find expected account '%s': %s"), *ResolvedDerivationPath, *Exception->GetMessage());
		return false;
	}

	if (!Cursor->MoveToNext())
	{
		UE_LOG(LogSeedVault, Error, TEXT("Failed to find expected account '%s'"), *ResolvedDerivationPath);
		return false;
	}

	OutAccount.AccountId = Cursor->GetLong(0);
	OutAccount.DerivationPath = Cursor->GetString(1);
	OutAccount.PublicKeyRaw = Cursor->GetBlob(2);
	OutAccount.PublicKeyEncoded = Cursor->GetString(3);
	OutAccount.Name = Cursor->GetString(4);
	OutAccount.bIsUserWallet = Cursor->GetShort(5) == 1 ? true : false;
	OutAccount.bIsValid = Cursor->GetShort(6) == 1 ? true : false;
	
	return true;
#else
	UE_LOG(LogSeedVault, Error, TEXT("SeedVault: platform is not supported"));
	return false;
#endif	
}

TArray<FWalletSeed> USeedVaultWallet::GetAuthorizedSeeds()
{
	TArray<FWalletSeed> AuthorizedSeeds;

#if PLATFORM_ANDROID
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	
	TArray<FString> Columns;
	Columns.Add(AUTHORIZED_SEEDS_AUTH_TOKEN);
	Columns.Add(AUTHORIZED_SEEDS_AUTH_PURPOSE);
	Columns.Add(AUTHORIZED_SEEDS_SEED_NAME);

	auto AuthorizedSeedsCursor = FWallet::GetAuthorizedSeeds(AppContext, Columns);

	while (AuthorizedSeedsCursor->MoveToNext())
	{
		FWalletSeed Seed;
		Seed.AuthToken = AuthorizedSeedsCursor->GetLong(0);
		Seed.AuthPurpose = AuthorizedSeedsCursor->GetInt(1);
		Seed.Name = AuthorizedSeedsCursor->GetString(2);
		AuthorizedSeeds.Add(Seed);
	}
#else

	/*
	// FOR TEST PURPOSES
	FWalletSeed Seed;
	Seed.AuthToken = 111;
	Seed.AuthPurpose = 1;
	Seed.Name = "Seed 1";
	AuthorizedSeeds.Add(Seed);
	Seed.AuthToken = 222;
	Seed.AuthPurpose = 1;
	Seed.Name = "Seed 2";
	AuthorizedSeeds.Add(Seed);	
	*/
	
	UE_LOG(LogSeedVault, Error, TEXT("SeedVault: platform is not supported"));
#endif

	return AuthorizedSeeds;
}

bool USeedVaultWallet::DeauthorizeSeed(int64 AuthToken)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	return FWallet::DeauthorizeSeed(AppContext, AuthToken, &Exception);
#endif
	return false;
}

bool USeedVaultWallet::HasUnauthorizedSeedsForPurpose(EWalletContractV1 Purpose)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
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

FString USeedVaultWallet::ResolveDerivationPath(const FString& DerivationPath, EWalletContractV1 Purpose)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	auto AppContext = Activity->GetApplication();
	FString Uri = FWallet::ResolveDerivationPath(AppContext, DerivationPath, (int32)Purpose, &Exception);
	if (Exception)
	{
		UE_LOG(LogSeedVault, Error, TEXT("Exception occured during ResolveDerivationPath call: %s"), *Exception->GetMessage());
		return "";
	}
	return Uri;
#endif

	return "";
}
