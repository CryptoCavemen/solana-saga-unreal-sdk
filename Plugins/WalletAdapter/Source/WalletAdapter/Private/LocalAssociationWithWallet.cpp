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

bool ULocalAssociationWithWallet::Start()
{
#if PLATFORM_ANDROID
	if (!LocalAssociation.IsValid())
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("OpenWallet() must be called before starting the association"));
		return false;
	}

	TSharedPtr<FThrowable> Ex;
	auto JClient = LocalAssociation->Start()->Get(LOCAL_ASSOCIATION_START_TIMEOUT_MS, &Ex);
	if (Ex)
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Failed establishing local association with wallet: %s"), *Ex->GetMessage());
		return false;
	}

	auto NativeClient = FMobileWalletAdapterClient::MakeFromExistingObject(JClient->GetJObject());
	Client = NewObject<UWalletAdapterClient>();
	Client->SetClientImpl(NativeClient);			
	
	return true;
#else
	return false;
#endif
}

bool ULocalAssociationWithWallet::Close()
{
#if PLATFORM_ANDROID
	Client = nullptr;
	
	if (LocalAssociation.IsValid())
	{
		TSharedPtr<FThrowable> Ex;
		LocalAssociation->Close()->Get(LOCAL_ASSOCIATION_CLOSE_TIMEOUT_MS, &Ex);
		LocalAssociation.Reset();
		if (Ex)
		{
			UE_LOG(LogWalletAdapter, Warning, TEXT("Local association close failed: %s"), *Ex->GetMessage());
			return false;
		}
	}
	else
	{
		UE_LOG(LogWalletAdapter, Error, TEXT("Start() must be called before closing the association"));
	}

	return true;

#endif
	return false;
}

UWalletAdapterClient* ULocalAssociationWithWallet::GetMobileWalletAdapterClient()
{
	return Client;
}