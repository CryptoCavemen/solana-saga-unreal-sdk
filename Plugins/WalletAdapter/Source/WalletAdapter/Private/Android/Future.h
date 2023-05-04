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
	FJavaClassObjectWrapperRef Get();
	/** Waits if necessary for the computation to complete, and then retrieves its result. Throwable pointer is valid if exception occured. */
	FJavaClassObjectWrapperPtr Get(TSharedPtr<FThrowable>& Throwable);
	/** Waits if necessary for at most the given time for the computation to complete, and then retrieves its result, if available. */
	FJavaClassObjectWrapperRef Get(int64 TimeoutMilliseconds);
	/** Waits if necessary for at most the given time for the computation to complete, and then retrieves its result, if available. Throwable pointer is valid if exception occured. */
	FJavaClassObjectWrapperPtr Get(int64 TimeoutMilliseconds, TSharedPtr<FThrowable>& Throwable);	
	
public:
	FJavaClassMethod CancelMethod;
	FJavaClassMethod IsCancelledMethod;
	FJavaClassMethod IsDoneMethod;
	FJavaClassMethod GetMethod;
	FJavaClassMethod GetWithTimeoutMethod;
};

#endif
