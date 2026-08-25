/*
 * Copyright IBM Corp. and others 2026
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 * Assisted-by: IBM Bob
 */
package org.openj9.test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import jdk.jfr.Event;
import jdk.jfr.Recording;
import jdk.jfr.consumer.RecordedEvent;
import jdk.jfr.consumer.RecordingFile;


public class EventWriterTests {

	static class TinyEvent extends Event {
		int value;
	}

	static class SequenceEvent extends Event {
		int index;
	}

	static class FatEvent extends Event {
		long a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
	}

	static class ConcurrentEvent extends Event {
		int threadIndex;
		int iteration;
	}

	public static void main(String[] args) throws Exception {
		System.out.println("Start tests");
		testGetEventWriterReturnsSameInstance();
		testTinyEvent();
		testSequenceEvents();
		testFatEventLargeSizePromotion();
		testConcurrentWriters();
		System.out.println("All EventWriterNatives tests passed.");
	}

	static void testGetEventWriterReturnsSameInstance() throws Exception {
		final Throwable[] error = new Throwable[1];
		final Object[]    holder = new Object[2];

		Thread t = new Thread(() -> {
			try (Recording r = new Recording()) {
				r.start();

				Object first = jdk.jfr.internal.JVM.getEventWriter();
				holder[0] = first;

				jdk.jfr.internal.EventWriter w = jdk.jfr.internal.EventWriter.getEventWriter();
				holder[1] = w;

				Object again = jdk.jfr.internal.JVM.getEventWriter();
				if (again != w) {
					throw new RuntimeException(
						"getEventWriter returned a different object on 2nd call: " + again + " vs " + w);
				}
				Object third = jdk.jfr.internal.JVM.getEventWriter();
				if (third != w) {
					throw new RuntimeException(
						"getEventWriter returned a different object on 3rd call");
				}
			} catch (Throwable ex) {
				error[0] = ex;
			}
		});
		t.start();
		t.join();

		if (error[0] != null) throw new Exception("Thread failed", error[0]);
		if (holder[0] != null) {
			throw new Exception("Expected null from getEventWriter on fresh thread, got: " + holder[0]);
		}
		if (holder[1] == null) {
			throw new Exception("Expected non-null EventWriter after newEventWriter");
		}
		System.out.println("PASS: testGetEventWriterReturnsSameInstance");
	}

	static void testTinyEvent() throws Exception {
		try (Recording r = new Recording()) {
			r.enable(TinyEvent.class).withoutStackTrace();
			r.start();
			TinyEvent e = new TinyEvent();
			e.value = 42;
			e.commit();
			r.stop();
			List<RecordedEvent> events = readEvents(r, TinyEvent.class);
			assertEquals(1, events.size(), "Expected exactly 1 TinyEvent");
			assertEquals(42, (int) events.get(0).getValue("value"), "Wrong value in TinyEvent");
		}
		System.out.println("PASS: testTinyEvent");
	}

	static void testSequenceEvents() throws Exception {
		final int COUNT = 5_000;
		try (Recording r = new Recording()) {
			r.enable(SequenceEvent.class).withoutStackTrace();
			r.start();
			for (int i = 0; i < COUNT; i++) {
				SequenceEvent e = new SequenceEvent();
				e.index = i;
				e.commit();
			}
			r.stop();
			List<RecordedEvent> events = readEvents(r, SequenceEvent.class);
			assertEquals(COUNT, events.size(), "Wrong number of SequenceEvents");
			boolean[] seen = new boolean[COUNT];
			for (RecordedEvent re : events) {
				int idx = re.getValue("index");
				seen[idx] = true;
			}
			for (int i = 0; i < COUNT; i++) {
				if (!seen[i]) {
					throw new Exception("Missing SequenceEvent with index=" + i);
				}
			}
		}
		System.out.println("PASS: testSequenceEvents");
	}

	static void testFatEventLargeSizePromotion() throws Exception {
		final long SENTINEL = Long.MAX_VALUE;
		final int  COUNT    = 100;
		try (Recording r = new Recording()) {
			r.enable(FatEvent.class).withoutStackTrace();
			r.start();
			for (int i = 0; i < COUNT; i++) {
				FatEvent e = new FatEvent();
				e.a = e.b = e.c = e.d = e.e = e.f = e.g = e.h = e.i = e.j = e.k = e.l = e.m =
				e.n = e.o = e.p = e.q = e.r = e.s = e.t = e.u = e.v = e.w = e.x = e.y = e.z = SENTINEL;
				e.commit();
			}
			r.stop();
			List<RecordedEvent> events = readEvents(r, FatEvent.class);
			assertEquals(COUNT, events.size(), "Wrong number of FatEvents");
			for (RecordedEvent re : events) {
				for (String field : new String[]{"a","b","c","d","e","f","g","h","i","j",
				                                 "k","l","m","n","o","p","q","r","s","t",
				                                 "u","v","w","x","y","z"}) {
					long v = re.getValue(field);
					if (v != SENTINEL) {
						throw new Exception("FatEvent field '" + field + "' = " + v + ", expected " + SENTINEL);
					}
				}
			}
		}
		System.out.println("PASS: testFatEventLargeSizePromotion");
	}

	static void testConcurrentWriters() throws Exception {
		final int THREADS    = 20;
		final int ITERATIONS = 100;
		final Throwable[] errors = new Throwable[THREADS];

		try (Recording r = new Recording()) {
			r.enable(ConcurrentEvent.class).withoutStackTrace();
			r.start();

			@SuppressWarnings("unchecked")
			final jdk.jfr.internal.EventWriter[] writers = new jdk.jfr.internal.EventWriter[THREADS];

			List<Thread> threads = new ArrayList<>();
			for (int ti = 0; ti < THREADS; ti++) {
				final int threadIndex = ti;
				Thread t = new Thread(() -> {
					try {
						writers[threadIndex] = jdk.jfr.internal.EventWriter.getEventWriter();
						for (int iter = 0; iter < ITERATIONS; iter++) {
							ConcurrentEvent e = new ConcurrentEvent();
							e.threadIndex = threadIndex;
							e.iteration   = iter;
							e.commit();
						}
					} catch (Throwable ex) {
						errors[threadIndex] = ex;
					}
				}, "jfr-test-" + ti);
				threads.add(t);
			}
			for (Thread t : threads) t.start();
			for (Thread t : threads) t.join();

			r.stop();

			for (int i = 0; i < THREADS; i++) {
				if (errors[i] != null) throw new Exception("Thread " + i + " failed", errors[i]);
			}

			for (int i = 0; i < THREADS; i++) {
				if (writers[i] == null) throw new Exception("Thread " + i + " got a null EventWriter");
				for (int j = i + 1; j < THREADS; j++) {
					if (writers[i] == writers[j]) {
						throw new Exception("Threads " + i + " and " + j + " share an EventWriter");
					}
				}
			}

			List<RecordedEvent> events = readEvents(r, ConcurrentEvent.class);
			assertEquals(THREADS * ITERATIONS, events.size(), "Wrong number of ConcurrentEvents");

			boolean[][] seen = new boolean[THREADS][ITERATIONS];
			for (RecordedEvent re : events) {
				int ti   = re.getValue("threadIndex");
				int iter = re.getValue("iteration");
				seen[ti][iter] = true;
			}
			for (int ti = 0; ti < THREADS; ti++) {
				for (int iter = 0; iter < ITERATIONS; iter++) {
					if (!seen[ti][iter]) {
						throw new Exception("Missing ConcurrentEvent thread=" + ti + " iter=" + iter);
					}
				}
			}
		}
		System.out.println("PASS: testConcurrentWriters");
	}

	private static void assertEquals(int expected, int actual, String msg) throws Exception {
		if (expected != actual) {
			throw new Exception(msg + ": expected " + expected + " but got " + actual);
		}
	}

	private static List<RecordedEvent> readEvents(Recording r, Class<? extends Event> eventClass)
			throws IOException {
		Path tmp = Files.createTempFile("jfr-test", ".jfr");
		try {
			r.dump(tmp);
			List<RecordedEvent> result = new ArrayList<>();
			for (RecordedEvent e : RecordingFile.readAllEvents(tmp)) {
				if (e.getEventType().getName().equals(eventClass.getName())) {
					result.add(e);
				}
			}
			return result;
		} finally {
			Files.deleteIfExists(tmp);
		}
	}

	private static String buildString(int length) {
		StringBuilder sb = new StringBuilder(length);
		for (int i = 0; i < length; i++) {
			sb.append((char) ('A' + (i % 26)));
		}
		return sb.toString();
	}
}
