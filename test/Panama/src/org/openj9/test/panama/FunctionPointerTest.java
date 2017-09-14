/*******************************************************************************
 * Copyright (c) 2012, 2017 IBM Corp. and others
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
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/
package org.openj9.test.panama;

import org.testng.annotations.*;
import org.testng.Assert;

import java.lang.invoke.*;
import java.nicl.*;

import panama.test.panamatest;

public class FunctionPointerTest {
	panamatest panama;

	@Parameters({ "path" })
	public void setUp(String path) throws Exception {
		Library lib = NativeLibrary.loadLibraryFile(path + "/libpanamatest.so");
		panama = NativeLibrary.bindRaw(panamatest.class, lib);
	}
	
	@Test
	public void testVoidVirtual() throws Throwable {
		panama.testFunctionVoid(this::funcVoidV);
	}
	
	@Test
	public void testByteVirtual() throws Throwable {
		byte resultByte = panama.testFunctionByte(this::funcByteV, (byte)5);
		Assert.assertEquals((byte)5, resultByte);
	}
	
	@Test
	public void testShortVirtual() throws Throwable {
		short resultShort = panama.testFunctionShort(this::funcShortV, (short)6);
		Assert.assertEquals((short)6, resultShort);
	}
	
	@Test
	public void testIntVirtual() throws Throwable {
		int resultInt = panama.testFunctionInt(this::funcIntV, 7);
		Assert.assertEquals(7, resultInt);
	}
	
	@Test
	public void testLongVirtual() throws Throwable {
		long resultLong = panama.testFunctionLong(this::funcLongV, 8L);
		Assert.assertEquals(8L, resultLong);
	}
	
	@Test
	public void testFloatVirtual() throws Throwable {
		float resultFloat = panama.testFunctionFloat(this::funcFloatV, 9f);
		Assert.assertEquals(9f, resultFloat);
	}
	
	@Test
	public void testDoubleVirtual() throws Throwable {
		double resultDouble = panama.testFunctionDouble(this::funcDoubleV, 10.0);
		Assert.assertEquals(10.0, resultDouble);
	}
	
	@Test
	public void testVoidStatic() throws Throwable {
		panama.testFunctionVoid(FunctionPointerTest::funcVoid);
	}
	
	@Test
	public void testByteStatic() throws Throwable {
		byte resultByte = panama.testFunctionByte(FunctionPointerTest::funcByte, (byte)5);
		Assert.assertEquals((byte)5, resultByte);
	}
	
	@Test
	public void testShortStatic() throws Throwable {
		short resultShort = panama.testFunctionShort(FunctionPointerTest::funcShort, (short)6);
		Assert.assertEquals((short)6, resultShort);
	}
	
	@Test
	public void testIntStatic() throws Throwable {
		int resultInt = panama.testFunctionInt(FunctionPointerTest::funcInt, 7);
		Assert.assertEquals(7, resultInt);
	}
	
	@Test
	public void testLongStatic() throws Throwable {
		long resultLong = panama.testFunctionLong(FunctionPointerTest::funcLong, 8L);
		Assert.assertEquals(8L, resultLong);
	}
	
	@Test
	public void testFloatStatic() throws Throwable {
		float resultFloat = panama.testFunctionFloat(FunctionPointerTest::funcFloat, 9f);
		Assert.assertEquals(9f, resultFloat);
	}
	
	@Test
	public void testDoubleStatic() throws Throwable {
		double resultDouble = panama.testFunctionDouble(FunctionPointerTest::funcDouble, 10.0);
		Assert.assertEquals(10.0, resultDouble);
	}
	
	@Test
	public void testSumAllScalars() throws Throwable {
		long result = panama.testSumAllScalar(FunctionPointerTest::sumAllScalars, (byte)1, (short)2, 3, 4L);
		Assert.assertEquals(10, result);
	}
	
	@Test
	public void testSumAllFloatingPoint() throws Throwable {
		double result = panama.testSumAllFloatingPoint(FunctionPointerTest::sumAllFloatingPoint, 1f, 2d, 3f, 4d);
		Assert.assertEquals(10d, result);
	}
	
	static void funcVoid() {
		return;
	}
	
	static int funcInt(int val) {
		return val;
	}
	
	static long funcLong(long val) {
		return val;
	}
	
	static short funcShort(short val) {
		return val;
	}
	
	static byte funcByte(byte val) {
		return val;
	}
	
	static float funcFloat(float val) {
		return val;
	}
	
	static double funcDouble(double val) {
		return val;
	}
	
	static long sumAllScalars(byte v1, short v2, int v3, long v4) {
		return v1 + v2 + v3 + v4;
	}
	
	static double sumAllFloatingPoint(float v1, double v2, float v3, double v4) {
		return v1 + v2 + v3 + v4;
	}
	
	void funcVoidV() {
		return;
	}
	
	int funcIntV(int val) {
		return val;
	}
	
	long funcLongV(long val) {
		return val;
	}
	
	short funcShortV(short val) {
		return val;
	}
	
	byte funcByteV(byte val) {
		return val;
	}
	
	float funcFloatV(float val) {
		return val;
	}
	
	double funcDoubleV(double val) {
		return val;
	}
}
