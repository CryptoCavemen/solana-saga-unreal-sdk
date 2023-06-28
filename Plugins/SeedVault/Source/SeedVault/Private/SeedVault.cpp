//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SeedVault.h"
#include "SeedVaultWallet.h"

#if PLATFORM_ANDROID
#include "Android/Wallet.h"
#include "Android/Bip44DerivationPath.h"
#endif

DEFINE_LOG_CATEGORY(LogSeedVault);

void FSeedVaultModule::StartupModule()
{
#if PLATFORM_ANDROID
	FWallet::StaticConstruct();
	FBip44DerivationPath::StaticConstruct();
#endif
}

void FSeedVaultModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSeedVaultModule, SeedVault)