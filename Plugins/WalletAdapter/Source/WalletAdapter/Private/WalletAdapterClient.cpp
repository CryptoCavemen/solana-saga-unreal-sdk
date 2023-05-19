//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "WalletAdapterClient.h"
#include "WalletAdapter.h"
#include "Android/GameActivity.h"
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

void UWalletAdapterClient::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, const FAuthSuccessCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
    TSharedPtr<FThrowable> Exception;

	auto JAuthFuture = Client->Authorize(IdentityUri, IconUri, IdentityName, Cluster, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Authorization failed: %s"), *Exception->GetMessage());
		Fail.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(JAuthFuture.IsValid());
	
	AsyncTask(ENamedThreads::AnyThread, [this, JAuthFuture, Success, Fail]
	{
		TSharedPtr<FThrowable> FutureGetException;		
		auto JAuthResult = JAuthFuture->Get(&FutureGetException);
		if (FutureGetException)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Authorization failed: %s"), *FutureGetException->GetMessage());
			AsyncTask(ENamedThreads::GameThread, [Fail, FutureGetException]
			{
				Fail.ExecuteIfBound(FutureGetException->GetMessage());
			});
			return;
		}

		check(JAuthResult.IsValid());
		auto AuthResult = FAuthorizationResult::MakeFromExistingObject(JAuthResult->GetJObject());
		
		// SUCCESS
		UE_LOG(LogWalletAdapter, Log, TEXT("Authorized successfully: AuthToken = %s"), *AuthToken);
		AsyncTask(ENamedThreads::GameThread, [this, AuthResult, Success]
		{
			AuthToken = AuthResult->GetAuthToken();
			PublicKey = AuthResult->GetPublicKey();
			AccountLabel = AuthResult->GetAccountLabel();
			WalletUriBase = AuthResult->GetWalletUriBase();
			
			Success.ExecuteIfBound(AuthToken);
		});
	});;
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
}

void UWalletAdapterClient::Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, const FAuthSuccessCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto JAuthFuture = Client->Reauthorize(IdentityUri, IconUri, IdentityName, AuthToken, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Reauthorization failed: %s"), *Exception->GetMessage());
		Fail.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(JAuthFuture.IsValid());

	AsyncTask(ENamedThreads::AnyThread, [this, JAuthFuture, Success, Fail]
	{
		TSharedPtr<FThrowable> FutureGetException;	
		auto JAuthResult = JAuthFuture->Get(&FutureGetException);
		if (FutureGetException)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Reauthorization failed: %s"), *FutureGetException->GetMessage());
			AsyncTask(ENamedThreads::GameThread, [Fail, FutureGetException]
			{
				Fail.ExecuteIfBound(FutureGetException->GetMessage());
			});
			return;
		}

		check(JAuthResult.IsValid());
		auto AuthResult = FAuthorizationResult::MakeFromExistingObject(JAuthResult->GetJObject());
	
		// SUCCESS
		UE_LOG(LogWalletAdapter, Log, TEXT("Reauthorized successfully"));
		AsyncTask(ENamedThreads::GameThread, [this, AuthResult, Success]
		{
			AuthToken = AuthResult->GetAuthToken();
			PublicKey = AuthResult->GetPublicKey();
			AccountLabel = AuthResult->GetAccountLabel();
			WalletUriBase = AuthResult->GetWalletUriBase();
			
			Success.ExecuteIfBound(AuthToken);
		});
	});
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
}

void UWalletAdapterClient::Deauthorize(const FSuccessCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	auto JDeauthFuture = Client->Deauthorize(AuthToken, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Deauthorization failed: %s"), *Exception->GetMessage());
		Fail.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(JDeauthFuture.IsValid());
	
	AsyncTask(ENamedThreads::AnyThread, [this, JDeauthFuture, Success, Fail]
	{
		TSharedPtr<FThrowable> FutureGetException;	
		JDeauthFuture->Get(&FutureGetException);
		if (FutureGetException)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Deauthorization failed: %s"), *FutureGetException->GetMessage());
			AsyncTask(ENamedThreads::GameThread, [Fail, FutureGetException]
			{
				Fail.ExecuteIfBound(FutureGetException->GetMessage());
			});
			return;
		}	
		
		// SUCCESS
		UE_LOG(LogWalletAdapter, Log, TEXT("Deauthorized successfully"));
		AsyncTask(ENamedThreads::GameThread, [this, Success]
		{
			AuthToken.Empty();
			Success.ExecuteIfBound();
		});
	});
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
}

void UWalletAdapterClient::SignTransaction(const FSolanaTransaction& Transaction, const FSuccessCallback& Success, const FFailCallback& Fail)
{
	TArray<FSolanaTransaction> Transactions;
	Transactions.Add(Transaction);
	SignTransactions(Transactions, Success, Fail);
}

void UWalletAdapterClient::SignTransactions(const TArray<FSolanaTransaction>& Transactions, const FSuccessCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	TArray<TArray<uint8>> RawTransactions;
	for (const auto& [Data] : Transactions)
		RawTransactions.Add(Data);

	auto JSignFuture = Client->SignTransactions(RawTransactions, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Failed to sign %d transaction(s): %s"), Transactions.Num(), *Exception->GetMessage());
		Fail.ExecuteIfBound(Exception->GetMessage());
		return;
	}

	check(JSignFuture.IsValid());
	
	AsyncTask(ENamedThreads::AnyThread, [JSignFuture, Transactions, Success, Fail]
	{
		TSharedPtr<FThrowable> FutureGetException;
		
		JSignFuture->Get(&FutureGetException);
		if (FutureGetException)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Failed to sign %d transaction(s): %s"), Transactions.Num(), *FutureGetException->GetMessage());
			AsyncTask(ENamedThreads::GameThread, [Fail, FutureGetException]
			{
				Fail.ExecuteIfBound(FutureGetException->GetMessage());
			});
			return;
		}

		// SUCCESS
		UE_LOG(LogWalletAdapter, Log, TEXT("Signed %d transaction(s)"), Transactions.Num());
		AsyncTask(ENamedThreads::GameThread, [Success]
		{
			Success.ExecuteIfBound();
		});
	});
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
}

void UWalletAdapterClient::SignAndSendTransactions(const TArray<FSolanaTransaction>& Transactions, int32 MinContextSlot, const FSuccessCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;

	TArray<TArray<uint8>> RawTransactions;
	for (const auto& [Data] : Transactions)
		RawTransactions.Add(Data);

	auto JSignFuture = Client->SignAndSendTransactions(RawTransactions, MinContextSlot ? &MinContextSlot : nullptr, Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Failed to sign and send %d transaction(s): %s"), Transactions.Num(), *Exception->GetMessage());
		Fail.ExecuteIfBound(Exception->GetMessage());
		return;
	}
	
	check(JSignFuture.IsValid());

	AsyncTask(ENamedThreads::AnyThread, [JSignFuture, Transactions, Success, Fail]
	{
		TSharedPtr<FThrowable> FutureGetException;
		JSignFuture->Get(&FutureGetException);
		if (FutureGetException)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Failed to sign and send %d transaction(s): %s"), Transactions.Num(), *FutureGetException->GetMessage());
			AsyncTask(ENamedThreads::GameThread, [Fail, FutureGetException]
			{
				Fail.ExecuteIfBound(FutureGetException->GetMessage());
			});
			return;
		}

		// SUCCESS
		UE_LOG(LogWalletAdapter, Log, TEXT("Signed and sent %d transaction(s)"), Transactions.Num());
		AsyncTask(ENamedThreads::GameThread, [Success]
		{
			Success.ExecuteIfBound();
		});
	});
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
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
