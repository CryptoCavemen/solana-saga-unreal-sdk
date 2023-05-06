//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "WalletAdapter.h"
#include "Android/LocalAssociationIntentCreator.h"

DEFINE_LOG_CATEGORY(LogWalletAdapter);

void FWalletAdapterModule::StartupModule()
{
#if PLATFORM_ANDROID
	FLocalAssociationIntentCreator::StaticConstruct();
#endif
}

void FWalletAdapterModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWalletAdapterModule, WalletAdapter)