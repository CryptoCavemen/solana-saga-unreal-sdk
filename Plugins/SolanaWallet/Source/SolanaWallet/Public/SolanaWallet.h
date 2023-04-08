// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class SOLANAWALLET_API FSolanaWalletModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void StartActivity(const FString& Action, const FString& Uri);
	void RunTest();

protected:
	void InitJNI() const;
};
