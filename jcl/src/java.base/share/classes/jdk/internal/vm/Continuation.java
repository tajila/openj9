/*[INCLUDE-IF JAVA_SPEC_VERSION >= 19]*/
/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/
package jdk.internal.vm;

import java.util.Set;
import java.util.function.Supplier;
import jdk.internal.access.JavaLangAccess;
import jdk.internal.access.SharedSecrets;

/**
 * Continuation class performing the mount/unmount operation for VirtualThread
 */
public class Continuation {
	private long vmRef; /* J9VMContinuation */

	private final ContinuationScope scope;
	private final Runnable runnable;
	private Continuation parent;
	private boolean started = false;
	private boolean finished = false;

	private static final JavaLangAccess JLA = SharedSecrets.getJavaLangAccess();

	/**
	 * Continuation's Pinned reasons
	 */
	public enum Pinned {
		/** In native code */
		NATIVE,
		/** Holding monitor(s) */
		MONITOR,
		/** In critical section */
		CRITICAL_SECTION
	}

	public enum PreemptStatus {
		/** Success */
		SUCCESS(null),
		/** Permanent fail */
		PERM_FAIL_UNSUPPORTED(null),
		/** Permanent fail due to yielding */
		PERM_FAIL_YIELDING(null),
		/** Permanent fail due to continuation unmounted */
		PERM_FAIL_NOT_MOUNTED(null),
		/** Transient fail due to continuation pinned {@link Pinned#CRITICAL_SECTION} */
		TRANSIENT_FAIL_PINNED_CRITICAL_SECTION(Pinned.CRITICAL_SECTION),
		/** Transient fail due to continuation pinned {@link Pinned#NATIVE} */
		TRANSIENT_FAIL_PINNED_NATIVE(Pinned.NATIVE),
		/** Transient fail due to continuation pinned {@link Pinned#MONITOR} */
		TRANSIENT_FAIL_PINNED_MONITOR(Pinned.MONITOR);

		final Pinned pinned;

		public Pinned pinned() {
			return pinned;
		}

		private PreemptStatus(Pinned reason) {
			this.pinned = reason;
		}
	}

	/**
	 * Public constructor for Continuation
	 * @param scope The scope of the Continuation
	 * @param target The target of the Continuation to execute
	 */
	public Continuation(ContinuationScope scope, Runnable target) {
		this.scope = scope;
		this.runnable = target;
		createContinuationImpl();
	}

	public ContinuationScope getScope() {
		return scope;
	}

	public Continuation getParent() {
		return parent;
	}

	public static Continuation getCurrentContinuation(ContinuationScope scope) {
		throw new UnsupportedOperationException();
	}

	/**
	 * @return new StackWalker for this Continuation
	 */
	public StackWalker stackWalker() {
		throw new UnsupportedOperationException();
	}

	/**
	 * @param options options for the StackWalker
	 * @return new StackWalker for this Continuation with set of options
	 */
	public StackWalker stackWalker(Set<StackWalker.Option> options) {
		throw new UnsupportedOperationException();
	}

	/**
	 * @param options options for the StackWalker
	 * @param scope scope of Continuations to include in the StackWalker
	 * @return new StackWalker for Continuations in the scope with set of options
	 */
	public StackWalker stackWalker(Set<StackWalker.Option> options, ContinuationScope scope) {
		throw new UnsupportedOperationException();
	}

	/**
	 * @return The stacktrace of this Continuation
	 * @throws IllegalStateException if this Continuation is mounted
	 */
	public StackTraceElement[] getStackTrace() {
		throw new UnsupportedOperationException();
	}

	public static <R> R wrapWalk(Continuation cont, ContinuationScope scope, Supplier<R> sup) {
		throw new UnsupportedOperationException();
	}

	private static void execute(Continuation cont) {
		cont.runnable.run();
	}

	public final void run() {
		if (finished) {
			throw new IllegalStateException("Continuation has already finished.");
		}
/*
		Thread carrier = JLA.currentCarrierThread();
		Continuation currentContinuation = JLA.getContinuation(carrier);

		if ((null != parent) && (parent != currentContinuation)) {
			throw new IllegalStateException("Running on carrier with incorrect parent.");
		} else {
			parent = currentContinuation;
		}

		JLA.setContinuation(carrier, this);
*/
		enterImpl();
/*
		JLA.setContinuation(carrier, parent);
*/
	}

	/**
	 * Suspend Continuations in the given scope
	 * @param scope the scope to lookup/suspend
	 * @return {@link true} or {@link false} based on success/failure
	 */
	public static boolean yield(ContinuationScope scope) {
		return yieldImpl(scope);
	}

	protected void onPinned(Pinned reason) {
		throw new UnsupportedOperationException();
	}

	protected void onContinue() {
		throw new UnsupportedOperationException();
	}

	public boolean isDone() {
		return finished;
	}

	public boolean isPreempted() {
		throw new UnsupportedOperationException();
	}

	public static native void pin();

	public static native void unpin();

	public static boolean isPinned(ContinuationScope scope) {
		throw new UnsupportedOperationException();
	}

	public PreemptStatus tryPreempt(Thread t) {
		throw new UnsupportedOperationException();
	}

	/* Continuation Native APIs */
	private native boolean createContinuationImpl();
	private native boolean enterImpl();
	private static native boolean yieldImpl(ContinuationScope scope);

}
