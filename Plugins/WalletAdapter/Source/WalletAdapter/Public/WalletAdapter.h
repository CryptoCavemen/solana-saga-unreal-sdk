//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"

WALLETADAPTER_API DECLARE_LOG_CATEGORY_EXTERN(LogWalletAdapter, Log, All);

class WALLETADAPTER_API FWalletAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
