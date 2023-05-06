//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LocalAssociationWithWallet.generated.h"

class UWalletAdapterClient;

#if PLATFORM_ANDROID
class FLocalAssociationScenario;
class FFuture;
#endif

/**
 * ULocalAssociationScenarioBP
 * OpenWallet -> Start -> IsDone? -> GetMobileWalletAdapterClient -> [any actions with the client] -> Close
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API ULocalAssociationWithWallet : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool OpenWallet(const FString& UriPrefix);
	/** Starts a local association with a wallet. Blocking call */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Start();
	/** Closes the local association. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Close();
	/** Returns a mobile wallet adapter client. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	UWalletAdapterClient* GetMobileWalletAdapterClient();	

protected:
#if PLATFORM_ANDROID
	TSharedPtr<FLocalAssociationScenario> LocalAssociation;
#endif
	UPROPERTY()
	UWalletAdapterClient* Client;
};
