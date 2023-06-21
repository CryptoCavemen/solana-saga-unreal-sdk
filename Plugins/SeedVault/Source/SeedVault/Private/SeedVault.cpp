//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SeedVault.h"
#include "Android/Wallet.h"
#include "SeedVaultWallet.h"

DEFINE_LOG_CATEGORY(LogSeedVault);

void FSeedVaultModule::StartupModule()
{
#if PLATFORM_ANDROID
	FWallet::StaticConstruct();
#endif
}

void FSeedVaultModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSeedVaultModule, SeedVault)