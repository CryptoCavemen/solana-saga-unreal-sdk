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

#include "Crypto/Base58.h"
#include "Math/BigInt.h"

const char ALPHABET[58] = {
	'1', '2', '3', '4', '5', '6', '7', '8',
	'9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
	'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
	'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
	'y', 'z'
};

TArray<uint8> FBase58::EncodeBase58(const TArray<uint8>& Data)
{
	TBigInt<1024> IntData = 0;
	for (int32 I = 0; I < Data.Num(); I++)
	{
		IntData = IntData * 256 + Data[I];
	}
	
	TArray<uint8> Result;
	while (IntData.IsGreater(0))
	{
		TBigInt<1024> Remainder = IntData % 58;
		IntData /= 58;
		Result.Insert(ALPHABET[Remainder.ToInt()], 0);
	}
	
	for (int32 I = 0; I < Data.Num() && Data[I] == 0; I++)
	{
		Result.Insert('1', 0);
	}
	
	return Result;	
}

TArray<uint8> FBase58::DecodeBase58(const TArray<uint8>& Encoded)
{
	TArray<uint8> Result;
	
	FString Alphabet = ALPHABET;
	
	TBigInt<1024> IntData;
	for (int i = 0; i < Encoded.Num(); i++)
	{
		int Digit = -1;
		unsigned char temp = Encoded[i];
		if (!Alphabet.FindChar(temp, Digit) || Digit < 0 )
		{
			//Error
		}
		IntData = IntData * 58 + Digit;
	}
	
	uint32* Bits = IntData.GetBits();
	constexpr int32 BitEndIndex = (1024 / 32) - 1;
	for (int k = BitEndIndex; k >= 0 ; k--)
	{
		if (Bits[k] != 0)
		{
			uint8 Bytes[4];
			Bytes[0] = (Bits[k] >> 24) & 0xff;
			Bytes[1] = (Bits[k] >> 16) & 0xff;
			Bytes[2] = (Bits[k] >> 8) & 0xff;
			Bytes[3] = (Bits[k] & 0xff);
			Result.Append(Bytes, 4);
		}
	}

	return Result;
}