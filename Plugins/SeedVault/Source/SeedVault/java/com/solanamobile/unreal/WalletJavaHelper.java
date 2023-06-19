//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

package com.solanamobile.unreal;
import com.solanamobile.seedvault;

public final class WalletJavaHelper 
{
    //private Activity mParent;
    //public WalletJavaHelper(Activity unrealActivity) 
    //{
    //    mParent = unrealActivity;
    //}

    public static native void onAuthorizeSeedSuccess(long authToken);
    public static native void onAuthorizeSeedFailure(String errorMessage);
    
    public static native void onCreateSeedSuccess(long authToken);
    public static native void onCreateSeedFailure(String errorMessage);
    
    public static native void onImportSeedSuccess(long authToken);
    public static native void onImportSeedFailure(String errorMessage);
   
    public static native void onSignTransactionsSuccess(ArrayList<SigningResponse> signingResponses); 
    public static native void onSignTransactionsFailure(String errorMessage);
    
    public static native void onSignMessagesSuccess(ArrayList<SigningResponse> signingResponses);
    public static native void onSignMessagesFailure(String errorMessage);    
}