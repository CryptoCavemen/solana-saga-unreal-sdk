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

#include "Network/RequestUtils.h"
#include "Network/RequestManager.h"
#include "SolanaUtils/Utils/Types.h"

#include "JsonObjectConverter.h"
#include "Misc/MessageDialog.h"

static FText ErrorTitle = FText::FromString("Error");
static FText InfoTitle = FText::FromString("Info");

FRequestData* FRequestUtils::RequestAccountInfo(const FString& PublicKey)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());

	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%u,"method":"getAccountInfo","params":["%s",{"encoding": "base58"}]})")
		,Request->Id, *PublicKey );

	return Request;
}

FAccountInfoJson FRequestUtils::ParseAccountInfoResponse(const FJsonObject& Data)
{
	FAccountInfoJson JSONData;
	if(TSharedPtr<FJsonObject> Result = Data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> ResultData = Result->GetObjectField("value");

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(ResultData.ToSharedRef(), Writer);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, OutputString);

		FJsonObjectConverter::JsonObjectToUStruct(ResultData.ToSharedRef(), &JSONData);

		FString Out = FString::Printf(TEXT("lamports: %f, owner: %s, rentEpoch: %i"), JSONData.lamports, *JSONData.owner, JSONData.rentEpoch);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, Out);

	}
	return JSONData;
}

FRequestData* FRequestUtils::RequestAccountBalance(const FString& PublicKey)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getBalance","params":["%s",{"commitment": "processed"}]})")
			,Request->Id , *PublicKey );
	
	return Request;
}


double FRequestUtils::ParseAccountBalanceResponse(const FJsonObject& Data)
{
	FBalanceResultJson JSONData;
	if(TSharedPtr<FJsonObject> result = Data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonObjectToUStruct(result.ToSharedRef(), &JSONData);
		return JSONData.value;
	}
	return -1;
}

FRequestData* FRequestUtils::RequestTokenAccount(const FString& PublicKey, const FString& Mint)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"mint": "%s"},{"encoding": "jsonParsed"}]})")
			,Request->Id, *PublicKey, *Mint );
	
	return Request;
}

FString FRequestUtils::ParseTokenAccountResponse(const FJsonObject& Data)
{
	auto [value] = ParseAllTokenAccountsResponse(Data);

	FString Result;
	if( !value.IsEmpty() )
	{
		Result = value[0].pubkey;
	}
	return Result;
}

FRequestData* FRequestUtils::RequestAllTokenAccounts(const FString& PublicKey, const FString& ProgramID)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"programId": "%s"},{"encoding": "jsonParsed"}]})")
			,Request->Id, *PublicKey, *ProgramID );
	
	return Request;
}

FTokenAccountArrayJson FRequestUtils::ParseAllTokenAccountsResponse(const FJsonObject& data)
{
	FTokenAccountArrayJson JSONData;
	if(TSharedPtr<FJsonObject> Result = data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonObjectToUStruct(Result.ToSharedRef(), &JSONData);
	}
	return JSONData;
}

FRequestData* FRequestUtils::RequestProgramAccounts(const FString& ProgramID, const uint32& Size, const FString& PublicKey)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getProgramAccounts","params":["%s",{"encoding":"base64","filters":[{"dataSize":%d},{"memcmp":{"offset":8,"bytes":"%s"}}]}]})")
			,Request->Id, *ProgramID, Size,*PublicKey );
		
	return Request;
}

TArray<FProgramAccountJson> FRequestUtils::ParseProgramAccountsResponse(const FJsonObject& Data)
{
	TArray<FProgramAccountJson> List;
	TArray<TSharedPtr<FJsonValue>> DataArray = Data.GetArrayField("result");
	for(const TSharedPtr<FJsonValue>& Entry:  DataArray )
	{
		const TSharedPtr<FJsonObject> EntryObject = Entry->AsObject();
		if( TSharedPtr<FJsonObject> Account = EntryObject->GetObjectField("account") )
		{
			FProgramAccountJson AccountData;
			FJsonObjectConverter::JsonObjectToUStruct( Account.ToSharedRef() , &AccountData);
			List.Add(AccountData);
		}
		FString pubKey = EntryObject->GetStringField("pubkey");
	}
	
	return List;
}

FRequestData* FRequestUtils::RequestMultipleAccounts(const TArray<FString>& PublicKey)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	FString list;
	for( FString key: PublicKey )
	{
		list.Append(FString::Printf(TEXT(R"("%s")"),*key));

		if(key != PublicKey.Last())
		{
			list.Append(",");
		}
	}
		
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method": "getMultipleAccounts","params":[[%s],{"dataSlice":{"offset":0,"length":0}}]})")
			,Request->Id , *list );
	
	return Request;
}

TArray<FAccountInfoJson> FRequestUtils::ParseMultipleAccountsResponse(const FJsonObject& Data)
{
	TArray<FAccountInfoJson> JSONData;
	if(TSharedPtr<FJsonObject> Result = Data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonArrayToUStruct( Result->GetArrayField("value") , &JSONData);
	}
	return JSONData;
}

FRequestData* FRequestUtils::SendTransaction(const FString& Transaction)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"sendTransaction","params":["%s",{"encoding": "base64"}]})")
			,Request->Id, *Transaction );
	
	return Request;
}

FString FRequestUtils::ParseTransactionResponse(const FJsonObject& Data)
{
	return Data.GetStringField("result");
}

FRequestData* FRequestUtils::RequestBlockHash()
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"id":%d,"jsonrpc":"2.0","method":"getRecentBlockhash","params":[{"commitment":"processed"}]})")
				,Request->Id );
	
	return Request;
}

FString FRequestUtils::ParseBlockHashResponse(const FJsonObject& Data)
{
	FString Hash;
	if(TSharedPtr<FJsonObject> Result = Data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> Value = Result->GetObjectField("value");
		Hash = Value->GetStringField("blockhash");
	}
	return Hash;
}

FRequestData* FRequestUtils::GetTransactionFeeAmount(const FString& Transaction)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getFeeForMessage", "params":[%s,{"commitment":"processed"}]})")
			,Request->Id, *Transaction );
	
	return Request;
}

int FRequestUtils::ParseTransactionFeeAmountResponse(const FJsonObject& Data)
{
	int Fee = 0;
	if(TSharedPtr<FJsonObject> Result = Data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> Value = Result->GetObjectField("value");
		Fee = Value->GetNumberField("value");
	}
	return Fee;
}

FRequestData* FRequestUtils::RequestAirDrop(const FString& PublicKey)
{
	FRequestData* Request = new FRequestData(FRequestManager::GetNextMessageID());
	
	Request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d, "method":"requestAirdrop", "params":["%s", 1000000000]})")
				,Request->Id, *PublicKey );
	
	return Request;
}

void FRequestUtils::DisplayError(const FString& Error)
{
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Error), &ErrorTitle);
}

void FRequestUtils::DisplayInfo(const FString& Info)
{
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Info), &InfoTitle);
}
