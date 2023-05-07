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

public:
	/** Authorizes a client. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);
	/** Reauthorizes a client. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken);
	/** Deauthorizes a client. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool Deauthorize(FString AuthorizationToken);	

public:
	UPROPERTY(BlueprintReadOnly)
	FString AuthToken;
	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> PublicKey;
	UPROPERTY(BlueprintReadOnly)
	FString AccountLabel;
	UPROPERTY(BlueprintReadOnly)
	FString WalletUriBase;
	
protected:
#if PLATFORM_ANDROID
	TSharedPtr<FMobileWalletAdapterClient> Client;
#endif
};
