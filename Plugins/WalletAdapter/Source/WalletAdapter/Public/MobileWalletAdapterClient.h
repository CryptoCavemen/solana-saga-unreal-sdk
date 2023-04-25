#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Android/MobileWalletAdapterClientWrapper.h"
#include "MobileWalletAdapterClient.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WALLETADAPTER_API UMobileWalletAdapterClient : public UObject
{
	GENERATED_BODY()

public:
	UMobileWalletAdapterClient();

	UFUNCTION(BlueprintCallable)
	void Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

private:
#if PLATFORM_ANDROID
	TSharedPtr<FMobileWalletAdapterClientWrapper> Wrapper;
#endif
};
