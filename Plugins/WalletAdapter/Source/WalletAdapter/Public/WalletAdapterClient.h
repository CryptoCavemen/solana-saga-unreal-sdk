//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Android/MobileWalletAdapterClient.h"
#include "WalletAdapterClient.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API UWalletAdapterClient : public UObject
{
	GENERATED_BODY()
	
public:
#if PLATFORM_ANDROID
	void SetClientImpl(const TSharedPtr<FMobileWalletAdapterClient>& InClient);
#endif

	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

	//UFUNCTION(BlueprintCallable, Category = "Solana")
	//void LocalAssociateAndExecute(FString UriPrefix);
	
private:
#if PLATFORM_ANDROID
	TSharedPtr<FMobileWalletAdapterClient> Client;
#endif
};
