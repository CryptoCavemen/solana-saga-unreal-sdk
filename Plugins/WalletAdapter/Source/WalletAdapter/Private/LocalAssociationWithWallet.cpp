//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociationWithWallet.h"
#include "WalletAdapterClient.h"
#include "WalletAdapter.h"
#include "Android/GameActivity.h"
#include "Android/LocalAssociationScenario.h"
#include "Android/LocalAssociationIntentCreator.h"
#include "Android/MobileWalletAdapterClient.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

#define DEFAULT_CLIENT_TIMEOUT_MS 60000

#define LOCAL_ASSOCIATION_START_TIMEOUT_MS 60000L // LocalAssociationScenario.start() has a shorter timeout; this is just a backup safety measure
#define LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS 2000L


bool ULocalAssociationWithWallet::OpenWallet(const FString& UriPrefix)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;
	
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	
	LocalAssociation = FLocalAssociationScenario::MakeInstance(DEFAULT_CLIENT_TIMEOUT_MS);
	auto AssociationIntent = FLocalAssociationIntentCreator::CreateAssociationIntent(UriPrefix, LocalAssociation->GetPort(), *LocalAssociation->GetSession());

	Activity->StartActivity(AssociationIntent, &Exception);
	if (Exception)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("No Mobile Wallet Adapter-compatible wallet is available: %s"), *Exception->GetMessage());
		return false;
	}
	
	return true;
#else
	return false;
#endif
}

void ULocalAssociationWithWallet::Start(const FSuccessStartCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	if (!LocalAssociation.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("OpenWallet() must be called before starting the association"));
		Fail.ExecuteIfBound("Wallet is not opened");
		return;
	}
	
	auto JFutureClient = LocalAssociation->Start();

	AsyncTask(ENamedThreads::AnyThread, [this, JFutureClient, Success, Fail]
	{
		TSharedPtr<FThrowable> Exception;
		auto JClient = JFutureClient->Get(LOCAL_ASSOCIATION_START_TIMEOUT_MS, &Exception);

		AsyncTask(ENamedThreads::GameThread, [this, JClient, Exception, Success, Fail]
		{		
			if (Exception)
			{
				UE_LOG(LogWalletAdapter, Error, TEXT("Failed establishing local association with wallet: %s"), *Exception->GetMessage());
				AsyncTask(ENamedThreads::GameThread, [Fail, Exception]
				{
					Fail.ExecuteIfBound(Exception->GetMessage());
				});
				return;
			}

			auto NativeClient = FMobileWalletAdapterClient::MakeFromExistingObject(JClient->GetJObject());
			Client = NewObject<UWalletAdapterClient>();
			Client->SetClientImpl(NativeClient);

			Success.ExecuteIfBound(Client);
		});
	});
#else
	Fail.ExecuteIfBound("Current platform is not supported");	
#endif
}

void ULocalAssociationWithWallet::Close(const FSuccessCloseCallback& Success, const FFailCallback& Fail)
{
#if PLATFORM_ANDROID
	Client = nullptr;

	if (!LocalAssociation.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Start() must be called before closing the association"));
		Fail.ExecuteIfBound("Local association is not started");
		return;
	}
	
	auto JFuture = LocalAssociation->Close();
	
	AsyncTask(ENamedThreads::AnyThread, [this, JFuture, Success, Fail]
	{
		TSharedPtr<FThrowable> Exception;
		JFuture->Get(LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS, &Exception);

		AsyncTask(ENamedThreads::GameThread, [this, Exception, Fail, Success]
		{
			LocalAssociation.Reset();
			if (Exception)
			{
				UE_LOG(LogWalletAdapter, Warning, TEXT("Local association close failed: %s"), *Exception->GetMessage());
				Fail.ExecuteIfBound(Exception->GetMessage());
				return;
			}
			
			Success.ExecuteIfBound();
		});
	});

#else
	Fail.ExecuteIfBound("Current platform is not supported");
#endif
}

UWalletAdapterClient* ULocalAssociationWithWallet::GetMobileWalletAdapterClient()
{
	return Client;
}