//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "WalletAdapterClient.h"
#include "WalletAdapter.h"
#include "Android/GameActivity.h"
#include "Android/LocalAssociationIntentCreator.h"
#include "Android/LocalAssociationScenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

#if PLATFORM_ANDROID
void UWalletAdapterClient::SetClientImpl(const TSharedPtr<FMobileWalletAdapterClient>& InClient)
{
	Client = InClient;
}
#endif

bool UWalletAdapterClient::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
#if PLATFORM_ANDROID
    TSharedPtr<FThrowable> Exception;

	auto JAuthFuture = Client->Authorize(IdentityUri, IconUri, IdentityName, Cluster, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Authorization failed: %s"), *Exception->GetMessage());
		return false;
	}

	check(JAuthFuture.IsValid());
	auto JAuthResult = JAuthFuture->Get(&Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Authorization failed: %s"), *Exception->GetMessage());
		return false;
	}

	check(JAuthResult.IsValid());

	auto AuthResult = FAuthorizationResult::MakeFromExistingObject(JAuthResult->GetJObject());
	AuthToken = AuthResult->GetAuthToken();
	PublicKey = AuthResult->GetPublicKey();
	AccountLabel = AuthResult->GetAccountLabel();
	WalletUriBase = AuthResult->GetWalletUriBase();

	// SUCCESS
	UE_LOG(LogWalletAdapter, Log, TEXT("Authorized successfully: AuthToken = %s"), *AuthToken);
	return true;
#endif
	return false;
}

bool UWalletAdapterClient::Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto JAuthFuture = Client->Reauthorize(IdentityUri, IconUri, IdentityName, AuthToken, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Reauthorization failed: %s"), *Exception->GetMessage());
		return false;
	}

	check(JAuthFuture.IsValid());
	auto JAuthResult = JAuthFuture->Get(&Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Reauthorization failed: %s"), *Exception->GetMessage());
		return false;
	}

	check(JAuthResult.IsValid());

	auto AuthResult = FAuthorizationResult::MakeFromExistingObject(JAuthResult->GetJObject());
	AuthToken = AuthResult->GetAuthToken();
	PublicKey = AuthResult->GetPublicKey();
	AccountLabel = AuthResult->GetAccountLabel();
	WalletUriBase = AuthResult->GetWalletUriBase();
	
	// SUCCESS
	UE_LOG(LogWalletAdapter, Log, TEXT("Reauthorized successfully"));
	return true;
#endif
	return false;	
}

bool UWalletAdapterClient::Deauthorize(FString AuthorizationToken)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto JDeauthFuture = Client->Deauthorize(AuthToken, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Deauthorization failed: %s"), *Exception->GetMessage());
		return false;
	}

	check(JDeauthFuture.IsValid());
	JDeauthFuture->Get(&Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Deauthorization failed: %s"), *Exception->GetMessage());
		return false;
	}	

	AuthToken.Empty();

	// SUCCESS
	UE_LOG(LogWalletAdapter, Log, TEXT("Deauthorized successfully"));
	return true;
#endif
	return false;	
}

/*
void UWalletAdapterClient::LocalAssociateAndExecute(FString UriPrefix)
{
#if PLATFORM_ANDROID
	const int64 LOCAL_ASSOCIATION_START_TIMEOUT_MS = 60000L; // LocalAssociationScenario.start() has a shorter timeout; this is just a backup safety measure
	const int64 LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS = 2000L;
	
	TSharedPtr<FThrowable> Exception;
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	
	auto LocalAssociation = FLocalAssociationScenario::MakeInstance(DEFAULT_CLIENT_TIMEOUT_MS);
	auto AssociationIntent = FLocalAssociationIntentCreator::CreateAssociationIntent(UriPrefix, LocalAssociation->GetPort(), *LocalAssociation->GetSession());

	auto CloseLocalAssociation = [&LocalAssociation]
	{
		TSharedPtr<FThrowable> Ex;
		LocalAssociation->Close()->Get(LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS, &Ex);
		if (Ex)
		{
			UE_LOG(LogWalletAdapter, Warning, TEXT("Local association close failed: %s"), *Ex->GetMessage());			
		}
	};

	Activity->StartActivity(AssociationIntent, &Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("No Mobile Wallet Adapter-compatible wallet is available: %s"), *Exception->GetMessage());
		return;
	}
	
	auto FutureResult = LocalAssociation->Start()->Get(LOCAL_ASSOCIATION_START_TIMEOUT_MS, &Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Failed establishing local association with wallet: %s"), *Exception->GetMessage());
		CloseLocalAssociation();
		return;
	}
	
 	Client = FMobileWalletAdapterClient::MakeFromExistingObject(FutureResult->GetJObject());
	
	Client->Authorize("https://solanamobile.com", "favicon.ico", "UnrealDApp", "testnet")->Get(&Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Authorization failed: %s"), *Exception->GetMessage());
		CloseLocalAssociation();
		return;
	}

	// SUCCESS
	UE_LOG(LogWalletAdapter, Log, TEXT("Authorized successfully"));

	Client.Reset();
	CloseLocalAssociation();

	//public static final String CLUSTER_MAINNET_BETA = "mainnet-beta";
	//public static final String CLUSTER_TESTNET = "testnet";
	//public static final String CLUSTER_DEVNET = "devnet";*
	
#endif
}
*/
