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

class FCursor;


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
	 * Request a {@link Cursor} containing account metadata for known accounts for the specified
	 * auth token. The projection should be a subset of the columns in
	 * {@link WalletContractV1#ACCOUNTS_ALL_COLUMNS}.
	 * @param Context the {@link Context} in which to perform this request
	 * @param AuthToken the auth token for which to retrieve account metadata
	 * @param Projection the set of columns to be present in the returned {@link Cursor}
	 * @return a Cursor
	 * @throws IllegalArgumentException if auth token is not valid for this app
	 */	
	static TSharedPtr<FCursor> GetAccounts(FJavaClassObjectWrapperRef Context, int64 AuthToken, const TArray<FString>& Projection, TSharedPtr<FThrowable>* OutException = nullptr);
	
	/**
	 * Request a Cursor containing account metadata for known accounts for the specified
	 * auth token which match the provided query. The projection should be a subset of the columns
	 * in {@link WalletContractV1#ACCOUNTS_ALL_COLUMNS}.
	 * @param Context the {@link Context} in which to perform this request
	 * @param AuthToken the auth token for which to retrieve account metadata
	 * @param projection the set of columns to be present in the returned {@link Cursor}
	 * @param FilterOnColumn the column from {@link WalletContractV1#ACCOUNTS_ALL_COLUMNS} on which
	 *      to filter
	 * @param Value the value of filterOnColumn which all returned rows must match
	 * @return a Cursor
	 * @throws IllegalArgumentException if auth token is not valid for this app, if filterOnColumn
	 *      is not a column in {@link WalletContractV1#ACCOUNTS_ALL_COLUMNS}, or if value cannot be
	 *      interpreted as an appropriate type to match against filterOnColumn values.
	 */	
	static TSharedPtr<FCursor> GetAccounts(FJavaClassObjectWrapperRef Context, int64 AuthToken,
		const TArray<FString>& Projection, const FString& FilterOnColumn, const FString& Value, TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Request a {@link Cursor} containing account metadata for the specified known account for the
	 * given auth token. The projection should be a subset of the columns in
	 * {@link WalletContractV1#ACCOUNTS_ALL_COLUMNS}.
	 * @param context the {@link Context} in which to perform this request
	 * @param authToken the auth token for which to retrieve account metadata
	 * @param id the account ID for which to retrieve account metadata
	 * @param projection the set of columns to be present in the returned {@link Cursor}
	 * @return a {@link Cursor}
	 * @throws IllegalArgumentException if auth token is not valid for this app
	 */
	static TSharedPtr<FCursor> GetAccount(FJavaClassObjectWrapperRef Context, int64 AuthToken, int64 Id, const TArray<FString>& Projection,
		TSharedPtr<FThrowable>* OutException = nullptr);

	/**
	 * Update the account name for the specified known account for the given auth token
	 * @param Context the Context in which to perform this request
	 * @param AuthToken the auth token for which to update account metadata
	 * @param Id the account ID to update
	 * @param Name the new name for the account. If null or blank, the account name will be cleared.
	 * @throws IllegalArgumentException if auth token is not valid for this app
	 * @throws NotModifiedException if ID does not represent a known account
	 */
	static void UpdateAccountName(FJavaClassObjectWrapperRef Context, int64 AuthToken, int64 Id, const FString& Name, TSharedPtr<FThrowable>* OutException = nullptr);
	
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

	/**
	 * Resolve the provided BIP derivation path Uri with scheme
	 * WalletContractV1#BIP32_URI_SCHEME or WalletContractV1#BIP44_URI_SCHEME and
	 * the provided @code WalletContractV1.PURPOSE_* purpose to a BIP32 derivation path.
	 * This is used to apply purpose-specific properties (such as mandatory hardening) and to
	 * translate from BIP44 to BIP32 derivation paths. Entries in the
	 * {@link WalletContractV1#ACCOUNTS_TABLE} table will always be resolved BIP32 derivation paths.
	 * @param Context the {@link Context} in which to perform this request
	 * @param DerivationPath a BIP32 or BIP44 {@link Uri} to resolve to a BIP32 derivation path for
	 *      the specified purpose
	 * @param Purpose the {@code WalletContractV1.PURPOSE_*} purpose for which to resolve
	 *      derivationPath
	 * @return a BIP32 derivation path {@link Uri}
	 * @throws UnsupportedOperationException on failure to resolve the provided derivationPath
	 */
	static FString ResolveDerivationPath(FJavaClassObjectWrapperRef Context, FString DerivationPath, int32 Purpose, TSharedPtr<FThrowable>* OutException = nullptr);	
	
protected:
	static FJavaClassMethod AuthorizeSeedMethod;
	static FJavaClassMethod CreateSeedMethod;
	static FJavaClassMethod ImportSeedMethod;
	static FJavaClassMethod SignTransactionMethod;
	static FJavaClassMethod SignMessageMethod;
	static FJavaClassMethod RequestPublicKeysMethod;
	static FJavaClassMethod GetAccountsMethod;
	static FJavaClassMethod GetAccountMethod;
	static FJavaClassMethod UpdateAccountNameMethod;
	static FJavaClassMethod DeauthorizeSeedMethod;
	static FJavaClassMethod HasUnauthorizedSeedsForPurposeMethod;
	static FJavaClassMethod ResolveDerivationPathMethod;
};

#endif