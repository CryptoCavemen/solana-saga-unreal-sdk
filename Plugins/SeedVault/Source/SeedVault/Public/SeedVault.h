//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"

SEEDVAULT_API DECLARE_LOG_CATEGORY_EXTERN(LogSeedVault, Log, All);

class SEEDVAULT_API FSeedVaultModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
