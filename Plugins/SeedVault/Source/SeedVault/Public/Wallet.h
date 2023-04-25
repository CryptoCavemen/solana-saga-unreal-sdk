// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "Defines.h"
#include "Wallet.generated.h"

struct FIntent
{
#if PLATFORM_ANDROID
	jobject JObject;
	FIntent(jobject InJObject) : JObject(InJObject) {}
#else
	void* JObject;
	FIntent(void* InJObject) : JObject(InJObject) {}
#endif

	FIntent() : JObject(nullptr) {}
	FIntent(const FIntent& Other) : JObject(Other.JObject) {}
};

UENUM(BlueprintType)
enum class EWalletContractV1 : uint8
{
	PURPOSE_SIGN_SOLANA_TRANSACTION = 0,
};

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
UCLASS()
class SEEDVAULT_API UWallet : public UBlueprintFunctionLibrary
{
	friend class FSeedVaultModule;
	GENERATED_BODY()
protected:
	static const char* GetClassName();
	/** initialize java objects and cache them for further usage. called when the module is loaded */
	static void Initialize();
	static void StartActivityForResult(FIntent Intent, int32 RequestCode);
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FCreateSeedCallback, bool, bSuccess, int64, AuthToken);
	UFUNCTION(BlueprintCallable)
	static void CreateSeed(FCreateSeedCallback FinishCallback, EWalletContractV1 Purpose = EWalletContractV1::PURPOSE_SIGN_SOLANA_TRANSACTION);
	static void OnCreateSeed(bool bSuccess, int64 AuthToken);

protected:
	static FCreateSeedCallback OnCreateSeedCallback;
};