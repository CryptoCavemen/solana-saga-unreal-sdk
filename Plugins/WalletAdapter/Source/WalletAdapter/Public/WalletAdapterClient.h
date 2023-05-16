//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Android/MobileWalletAdapterClient.h"
#include "WalletAdapterClient.generated.h"

USTRUCT(BlueprintType)
struct FSolanaTransaction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> Data; 
};

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
	/** Signs transactions. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool SignTransactions(const TArray<FSolanaTransaction>& Transactions);
	/** Signs a transaction. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool SignTransaction(const FSolanaTransaction& Transaction);
	/** Signs and sends transactions. Blocking call. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool SignAndSendTransactions(const TArray<FSolanaTransaction>& Transactions, int32 MinContextSlot = 0);	

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
