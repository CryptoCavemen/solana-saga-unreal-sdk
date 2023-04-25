// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeedVault.h"
#include "Wallet.h"

void FSeedVaultModule::StartupModule()
{
	UWallet::Initialize();
}

void FSeedVaultModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSeedVaultModule, SeedVault)