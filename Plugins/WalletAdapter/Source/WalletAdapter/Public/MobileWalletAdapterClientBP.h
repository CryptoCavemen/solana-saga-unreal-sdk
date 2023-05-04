﻿//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Android/MobileWalletAdapterClient.h"
#include "MobileWalletAdapterClientBP.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WALLETADAPTER_API UMobileWalletAdapterClientBP : public UObject
{
	GENERATED_BODY()

public:
	UMobileWalletAdapterClientBP();
	virtual ~UMobileWalletAdapterClientBP() override;

//	UFUNCTION(BlueprintCallable)
//	void Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

	UFUNCTION(BlueprintCallable)
	void LocalAssociateAndExecute(FString UriPrefix);
	
private:
#if PLATFORM_ANDROID
	FMobileWalletAdapterClient* Wrapper;
#endif
};
