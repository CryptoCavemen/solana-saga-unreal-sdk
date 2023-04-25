// Copyright Epic Games, Inc. All Rights Reserved.

#include "WalletAdapter.h"

#include "MobileWalletAdapterClient.h"

void FWalletAdapterModule::StartupModule()
{
	//FMobileWalletAdapterClient::Initialize();
}

void FWalletAdapterModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWalletAdapterModule, WalletAdapter)