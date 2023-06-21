//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Data.generated.h"

/**
 * FByteArray
 */
USTRUCT(BlueprintType)
struct SOLANAMOBILE_API FByteArray
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> Data;
	
	FByteArray() {}
	FByteArray(const TArray<uint8>& InData) : Data(InData) {}
};