// Copyright (c) Sergey Makovkin (makovkin.s@gmail.com). All Rights Reserved.

package com.solanamobile.unreal;
//import android.app.Activity;

public final class WalletJavaHelper 
{
    //private Activity mParent;
    //public WalletJavaHelper(Activity unrealActivity) 
    //{
    //    mParent = unrealActivity;
    //}

    public static native void onCreateSeed(boolean bSuccess, long authToken);
}