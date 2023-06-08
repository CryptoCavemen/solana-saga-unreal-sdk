/*
Copyright 2022 ATMTA, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Jon Sawler
*/
#pragma once

#include "CoreMinimal.h"

struct FRequestData;
struct FAccountInfoJson;
struct FBalanceResultJson;
struct FTokenAccountArrayJson;
struct FProgramAccountJson;

class FRequestUtils
{
public:
	
	static FRequestData* RequestAccountInfo(const FString& PublicKey);
	static FAccountInfoJson ParseAccountInfoResponse(const FJsonObject& Data);

	static FRequestData* RequestAccountBalance(const FString& PublicKey);
	static double ParseAccountBalanceResponse(const FJsonObject& Data);

	static FRequestData* RequestTokenAccount(const FString& PublicKey, const FString& Mint);
	static FString ParseTokenAccountResponse(const FJsonObject& Data);

	static FRequestData* RequestAllTokenAccounts(const FString& PublicKey, const FString& ProgramID);
	static FTokenAccountArrayJson ParseAllTokenAccountsResponse(const FJsonObject& data);

	static FRequestData* RequestProgramAccounts(const FString& ProgramID, const uint32& Size, const FString& PublicKey);
	static TArray<FProgramAccountJson> ParseProgramAccountsResponse(const FJsonObject& Data);

	static FRequestData* RequestMultipleAccounts(const TArray<FString>& PublicKey);
	static TArray<FAccountInfoJson> ParseMultipleAccountsResponse(const FJsonObject& Data);
	
	static FRequestData* RequestBlockHash();
	static FString ParseBlockHashResponse(const FJsonObject& Data);
	static int32 ParseBlockHashResponseContextSlot(const FJsonObject& Data);

	static FRequestData* GetTransactionFeeAmount(const FString& Transaction);
	static int ParseTransactionFeeAmountResponse(const FJsonObject& Data);
	
	static FRequestData* SendTransaction(const FString& Transaction);
	static FString ParseTransactionResponse(const FJsonObject& Data);
	
	static FRequestData* RequestAirDrop(const FString& PublicKey);

	static void DisplayError(const FString& Error);
	static void DisplayInfo(const FString& Info);
};
