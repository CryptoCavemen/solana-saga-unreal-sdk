//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Android/Defines.h"
#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/Throwable.h"
#include "Android/JavaClassObjectWrapper.h"


/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
class FWallet : FJavaClassObjectWrapper
{
private:
	FWallet() {}
	
	DECLARE_JAVA_CLASS_OBJECT_STATIC()
public:
	/**
	 * Request authorization of a new seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onAuthorizeSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used.
	 * @return an {@link Intent} suitable for usage with Activity#startActivityForResult(Intent, int)
	 */	
	static FJavaClassObjectWrapperPtr AuthorizeSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Request creation of a new seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onCreateSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used.
	 * @return an {@link Intent} suitable for usage with Activity#startActivityForResult(Intent, int)
	 */	
	static FJavaClassObjectWrapperPtr CreateSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Request import of an existing seed for the specified purpose. The returned {@link Intent}
	 * should be used with {@link Activity#startActivityForResult(Intent, int)}, and the result (as
	 * returned to {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters
	 * to {@link #onImportSeedResult(int, Intent)}.
	 * @param Purpose the purpose for which the seed will be used.
	 * @return an {@link Intent} suitable for usage with Activity#startActivityForResult(Intent, int)
	 */	
	static FJavaClassObjectWrapperPtr ImportSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);

    /**
     * Request that the provided transaction be signed (with whatever method is appropriate for the
     * purpose originally specified for this auth token). The returned Intent should be used
     * with Activity#startActivityForResult(Intent, int), and the result (as returned to
     * Activity#onActivityResult(int, int, Intent)) should be used as parameters to
     * OnSignTransactionsResult(int, Intent).
     * @param AuthToken the auth token for the seed with which to perform transaction signing
     * @param DerivationPath a BipDerivationPath representing the account with which to
     *      sign this transaction
     * @param Transaction a byte[] containing the transaction to be signed
     * @return an Intent suitable for usage with Activity#startActivityForResult(Intent, int)
     */
    static FJavaClassObjectWrapperPtr SignTransaction(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Transaction, TSharedPtr<FThrowable>* OutException = nullptr);
	
	/**
	 * Request that the provided message be signed (with whatever method is appropriate for the
	 * purpose originally specified for this auth token). The returned Intent should be used
	 * with {@link Activity#startActivityForResult(Intent, int)}, and the result (as returned to
	 * {@link Activity#onActivityResult(int, int, Intent)}) should be used as parameters to
	 * {@link #onSignMessagesResult(int, Intent)}.
	 * @param AuthToken the auth token for the seed with which to perform message signing
	 * @param DerivationPath a BipDerivationPath representing the account with which to
	 *      sign this message
	 * @param Message a byte[] containing the message to be signed
	 * @return an Intent suitable for usage with Activity#startActivityForResult(Intent, int)
	 */
	static FJavaClassObjectWrapperPtr SignMessage(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Message, TSharedPtr<FThrowable>* OutException = nullptr);	

	/**
	 * Request the public key for a given {@link BipDerivationPath} of a seed. The returned
	 * {@link Intent} should be used with {@link Activity#startActivityForResult(Intent, int)}, and
	 * the result (as returned to {@link Activity#onActivityResult(int, int, Intent)}) should be
	 * used as parameters to {@link #onRequestPublicKeysResult(int, Intent)}. If the public key is
	 * not present in the results of {@link #getAccounts(Context, long, String[])}, the user will be
	 * asked to authorize access to this public key.
	 * @param AuthToken the auth token for the seed with which to request a public key
	 * @param DerivationPath a {@link BipDerivationPath} representing the account from which to
	 *      request the public key
	 * @return an Intent suitable for usage with Activity#startActivityForResult
	 */
	static FJavaClassObjectWrapperPtr RequestPublicKey(int64 AuthToken, const FString& DerivationPath, TSharedPtr<FThrowable>* OutException = nullptr);
	
	/**
	 * Request the public keys for a set of {@link BipDerivationPath}s of a seed. The returned
	 * {@link Intent} should be used with {@link Activity#startActivityForResult(Intent, int)}, and
	 * the result (as returned to {@link Activity#onActivityResult(int, int, Intent)}) should be
	 * used as parameters to {@link #onRequestPublicKeysResult(int, Intent)}. If the public keys are
	 * not present in the results of {@link #getAccounts(Context, long, String[])}, the user will be
	 * asked to authorize access to these public keys.
	 * @param AuthToken the auth token for the seed with which to request a public key
	 * @param DerivationPaths an {@link ArrayList} of {@link BipDerivationPath}s representing the
	 *      accounts from which to request the public keys
	 * @return an Intent suitable for usage with Activity#startActivityForResult
	 */	
	static FJavaClassObjectWrapperPtr RequestPublicKeys(int64 AuthToken, const TArray<FString>& DerivationPaths, TSharedPtr<FThrowable>* OutException = nullptr);

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
	static FJavaClassMethod SignTransactionMethod;
	static FJavaClassMethod SignMessageMethod;
	static FJavaClassMethod RequestPublicKeysMethod;
	static FJavaClassMethod DeauthorizeSeedMethod;
	static FJavaClassMethod HasUnauthorizedSeedsForPurposeMethod;
};

#endif