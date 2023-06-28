//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociationWithWallet.h"
#include "WalletAdapterClient.h"
#include "WalletAdapter.h"

#if PLATFORM_ANDROID
#include "Android/GameActivity.h"
#include "Android/LocalAssociationScenario.h"
#include "Android/LocalAssociationIntentCreator.h"
#include "Android/MobileWalletAdapterClient.h"

#include "Android/AndroidApplication.h"
#endif

#define DEFAULT_CLIENT_TIMEOUT_MS 60000

#define LOCAL_ASSOCIATION_START_TIMEOUT_MS 60000L // LocalAssociationScenario.start() has a shorter timeout; this is just a backup safety measure
#define LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS 2000L


void ULocalAssociationWithWallet::Start(const FStartSuccessDelegate& Success, const FFailureDelegate& Failure)
{
#if PLATFORM_ANDROID
	if (!LocalAssociation.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("OpenWallet() must be called before starting the association"));
		Failure.ExecuteIfBound("Wallet is not opened");
		return;
	}
	
	auto JFutureClient = LocalAssociation->Start();

	AsyncTask(ENamedThreads::AnyThread, [this, JFutureClient, Success, Failure]
	{
		TSharedPtr<FThrowable> Exception;
		auto JClient = JFutureClient->Get(LOCAL_ASSOCIATION_START_TIMEOUT_MS, &Exception);
	
		if (Exception)
		{
			UE_LOG(LogWalletAdapter, Error, TEXT("Failed establishing local association with wallet: %s"), *Exception->GetMessage());
			Failure.ExecuteIfBound(Exception->GetMessage());
			return;
		}

		auto NativeClient = FMobileWalletAdapterClient::CreateFromExisting(JClient->GetJObject());
		Client = NewObject<UWalletAdapterClient>();
		Client->SetClientImpl(NativeClient);

		Success.ExecuteIfBound(Client);
	});
#else
	Failure.ExecuteIfBound("Current platform is not supported");	
#endif	
}

void ULocalAssociationWithWallet::Close(const FCloseSuccessDelegate& Success, const FFailureDelegate& Failure)
{
#if PLATFORM_ANDROID
	Client = nullptr;

	if (!LocalAssociation.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Start() must be called before closing the association"));
		Failure.ExecuteIfBound("Local association is not started");
		return;
	}
	
	auto JFuture = LocalAssociation->Close();
	
	AsyncTask(ENamedThreads::AnyThread, [this, JFuture, Success, Failure]
	{
		TSharedPtr<FThrowable> Exception;
		JFuture->Get(LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS, &Exception);

		LocalAssociation.Reset();
		if (Exception)
		{
			UE_LOG(LogWalletAdapter, Warning, TEXT("Local association close failed: %s"), *Exception->GetMessage());
			Failure.ExecuteIfBound(Exception->GetMessage());
			return;
		}
		
		Success.ExecuteIfBound();
	});

#else
	Failure.ExecuteIfBound("Current platform is not supported");
#endif	
}

bool ULocalAssociationWithWallet::OpenWallet(const FString& UriPrefix)
{
#if PLATFORM_ANDROID
	TSharedPtr<FThrowable> Exception;
	
	auto Activity = FGameActivity::CreateFromExisting(FAndroidApplication::GetGameActivityThis());
	
	LocalAssociation = FLocalAssociationScenario::CreateInstance(DEFAULT_CLIENT_TIMEOUT_MS);
	auto AssociationIntent = FLocalAssociationIntentCreator::CreateAssociationIntent(UriPrefix, LocalAssociation->GetPort(), *LocalAssociation->GetSession());
	if (!AssociationIntent.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("No Mobile Wallet Adapter-compatible wallet is available: %s"), *Exception->GetMessage());
		return false;
	}
	
	Activity->StartActivity(AssociationIntent.ToSharedRef(), &Exception);
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

void ULocalAssociationWithWallet::K2_Start(const FStartSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
	Start(
		FStartSuccessDelegate::CreateLambda([Success](UWalletAdapterClient* InClient)
		{
			if (!IsInGameThread())
			{
				AsyncTask(ENamedThreads::GameThread, [Success, InClient]
				{			
					Success.ExecuteIfBound(InClient);
				});
			}
			else
			{
				Success.ExecuteIfBound(InClient);
			}
		}),
		FFailureDelegate::CreateLambda([Failure](const FString& ErrorMessage)
		{
			if (!IsInGameThread())
			{
				AsyncTask(ENamedThreads::GameThread, [Failure, ErrorMessage]
				{			
					Failure.ExecuteIfBound(ErrorMessage);
				});
			}
			else
			{
				Failure.ExecuteIfBound(ErrorMessage);
			}
		}));	
}

void ULocalAssociationWithWallet::K2_Close(const FCloseSuccessDynDelegate& Success, const FFailureDynDelegate& Failure)
{
	Close(
		FCloseSuccessDelegate::CreateLambda([Success]
		{
			if (!IsInGameThread())
			{
				AsyncTask(ENamedThreads::GameThread, [Success]
				{
					Success.ExecuteIfBound();
				});
			}
			else
			{
				Success.ExecuteIfBound();
			}
		}),
		FFailureDelegate::CreateLambda([Failure](const FString& ErrorMessage)
		{
			if (!IsInGameThread())
			{
				AsyncTask(ENamedThreads::GameThread, [Failure, ErrorMessage]
				{			
					Failure.ExecuteIfBound(ErrorMessage);
				});
			}
			else
			{
				Failure.ExecuteIfBound(ErrorMessage);
			}			
		}));	
}

UWalletAdapterClient* ULocalAssociationWithWallet::GetWalletAdapterClient()
{
	return Client;
}
