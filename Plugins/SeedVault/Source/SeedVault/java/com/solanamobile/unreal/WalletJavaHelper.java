//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

package com.solanamobile.unreal;
//import android.app.Activity;

public final class WalletJavaHelper 
{
    //private Activity mParent;
    //public WalletJavaHelper(Activity unrealActivity) 
    //{
    //    mParent = unrealActivity;
    //}

    public static native void onCreateSeedSuccess(long authToken);
    public static native void onCreateSeedFailure(String errorMessage);
}