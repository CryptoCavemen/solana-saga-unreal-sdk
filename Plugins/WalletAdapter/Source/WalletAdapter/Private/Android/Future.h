//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Throwable.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FFuture : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FFuture);
public:
	/** Attempts to cancel execution of this task. */
	bool Cancel(bool MayInterruptIfRunning);
	/** Returns true if this task was cancelled before it completed normally. */
	bool IsCancelled();
	/** Returns true if this task completed. */
	bool IsDone();
	/** Waits if necessary for the computation to complete, and then retrieves its result. */
	FJavaClassObjectWrapperPtr Get(TSharedPtr<FThrowable>* OutException = nullptr);
	/** Waits if necessary for at most the given time for the computation to complete, and then retrieves its result, if available. */
	FJavaClassObjectWrapperPtr Get(int64 TimeoutMilliseconds, TSharedPtr<FThrowable>* OutException = nullptr);	
	
public:
	FJavaClassMethod CancelMethod;
	FJavaClassMethod IsCancelledMethod;
	FJavaClassMethod IsDoneMethod;
	FJavaClassMethod GetMethod;
	FJavaClassMethod GetWithTimeoutMethod;
};

#endif
