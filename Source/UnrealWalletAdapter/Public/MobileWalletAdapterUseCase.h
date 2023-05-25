//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "WalletAdapterClient.h"
#include "MobileWalletAdapterUseCase.generated.h"

UNREALWALLETADAPTER_API DECLARE_LOG_CATEGORY_EXTERN(LogWalletAdapterUseCase, Log, All);

/**
 * 
 */
UCLASS()
class UNREALWALLETADAPTER_API UMobileWalletAdapterUseCase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE(FSuccessCallback);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureCallback, const FString&, ErrorMessage);
	
	UFUNCTION(BlueprintCallable)
	static void SignTransaction(UWalletAdapterClient* Client, const FSuccessCallback& Success, const FFailureCallback& Failure);
};
