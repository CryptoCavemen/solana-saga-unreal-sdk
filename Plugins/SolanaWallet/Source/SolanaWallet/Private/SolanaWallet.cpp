// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolanaWallet.h"
#include "Wallet.h"

void FSolanaWalletModule::StartupModule()
{
	UWallet::Initialize();
}

void FSolanaWalletModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSolanaWalletModule, SolanaWallet)