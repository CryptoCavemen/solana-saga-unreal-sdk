//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"

SOLANAMOBILE_API DECLARE_LOG_CATEGORY_EXTERN(LogSolanaMobile, Log, All);

class SOLANAMOBILE_API FSolanaMobileModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
