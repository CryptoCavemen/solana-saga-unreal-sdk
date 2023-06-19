//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Throwable.h"
#include "JavaClassObjectWrapper.h"

namespace SeedVault
{

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
class FWallet : FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT_STATIC()
public:
	/**
	 * Request authorization of a new seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onAuthorizeSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used. One of the
	 *      {@code WalletContractV1.PURPOSE_*} constants.
	 * @return an {@link Intent} suitable for usage with
	 *      {@link Activity#startActivityForResult(Intent, int)}
	 */	
	static FJavaClassObjectWrapperPtr AuthorizeSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Request creation of a new seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onCreateSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used. One of the
	 *      {@code WalletContractV1.PURPOSE_*} constants.
	 * @return an {@link Intent} suitable for usage with
	 *      {@link Activity#startActivityForResult(Intent, int)}
	 */	
	static FJavaClassObjectWrapperPtr CreateSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Request import of an existing seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onImportSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used. One of the
	 *      {@code WalletContractV1.PURPOSE_*} constants.
	 * @return an {@link Intent} suitable for usage with
	 *      {@link Activity#startActivityForResult(Intent, int)}
	 */	
	static FJavaClassObjectWrapperPtr ImportSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Deauthorize the specified seed for the current app
	 * @param Context the Context in which to perform this request
	 * @param AuthToken the auth token of the seed to deauthorize
	 * @param OutException NotModifiedException if the seed was not authorized for this app
	 */	
	static bool DeauthorizeSeed(FJavaClassObjectWrapperRef Context, int64 AuthToken, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Test whether there are any unauthorized seeds with the specified purpose available for the
	 * current app
	 * @param Context the context in which to perform this request
	 * @param Purpose the purpose
	 * @param OutException throws IllegalArgumentException if purpose is not a known value
	 * @return true if there are unauthorized seeds for purpose, else false
	 */	
	static bool HasUnauthorizedSeedsForPurpose(FJavaClassObjectWrapperRef Context, int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);
	
protected:
	static FJavaClassMethod AuthorizeSeedMethod;
	static FJavaClassMethod CreateSeedMethod;
	static FJavaClassMethod ImportSeedMethod;
	static FJavaClassMethod DeauthorizeSeedMethod;
	static FJavaClassMethod HasUnauthorizedSeedsForPurposeMethod;
};
	
}

#endif