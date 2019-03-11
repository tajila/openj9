/*******************************************************************************
 * Copyright (c) 2018, 2019 IBM Corp. and others
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
package org.openj9.test.lworld;

import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.invoke.MethodType;
import java.lang.reflect.Field;

import jdk.internal.misc.Unsafe;

import org.testng.Assert;
import static org.testng.Assert.*;
import org.testng.annotations.Test;

/*
 * Instructions to run this test:
 * 
 * 1) recompile the JVM with J9VM_OPT_VALHALLA_VALUE_TYPES flag turned on in j9cfg.h.ftl (or j9cfg.h.in when cmake is turned on)
 * 2) cd [openj9-openjdk-dir]/openj9/test/TestConfig
 * 3) export JAVA_BIN=[openj9-openjdk-dir]/build/linux-x86_64-normal-server-release/images/jdk/bin
 * 4) export PATH=$JAVA_BIN:$PATH
 * 5) export JDK_VERSION=Valhalla
 * 6) export SPEC=linux_x86-64_cmprssptrs
 * 7) export BUILD_LIST=functional/Valhalla
 * 8) export AUTO_DETECT=off
 * 9) export JDK_IMPL=openj9
 * 10) make -f run_configure.mk && make compile && make _sanity
 */

@Test(groups = { "level.sanity" })
public class ValueTypeTests {
	static Unsafe unsafe = null;
	static Lookup lookup = MethodHandles.lookup();
	static Class point2DClass = null;
	static Class line2DClass = null;
	static MethodHandle makePoint2D = null;
	static MethodHandle makeLine2D = null;
	static MethodHandle getX = null;
	static MethodHandle getY = null;
	static MethodHandle makeFlattenedTriangle2D = null;
	static MethodHandle makeFlattenedLine2D = null;
	static Class flattenedLine2DClass = null;
	static Class flattenedTriangle2DClass = null;
	static MethodHandle getFlatSt = null;
	static MethodHandle withFlatSt = null;
	static MethodHandle getFlatEn = null;
	static MethodHandle withFlatEn = null;
	
	static {
		try {
			Field f = Unsafe.class.getDeclaredField("theUnsafe");
			f.setAccessible(true);
			unsafe = (Unsafe) f.get(null);
		} catch (NoSuchFieldException | SecurityException | IllegalArgumentException | IllegalAccessException e) {
			e.printStackTrace();
		}
	}
	
	
	/*
	 * Create a value type
	 * 
	 * value Point2D {
	 * 	int x;
	 * 	int y;
	 * }
	 */
	@Test(priority=1)
	static public void testCreatePoint2D() throws Throwable {
		String fields[] = {"x:I", "y:I"};
		point2DClass = ValueTypeGenerator.generateValueClass("Point2D", fields);
		
		makePoint2D = lookup.findStatic(point2DClass, "makeValue", MethodType.methodType(point2DClass, int.class, int.class));
		
		getX = generateGetter(point2DClass, "x", int.class);
		MethodHandle withX = generateWither(point2DClass, "x", int.class);
		getY = generateGetter(point2DClass, "y", int.class);
		MethodHandle withY = generateWither(point2DClass, "y", int.class);

		int x = 0xFFEEFFEE;
		int y = 0xAABBAABB;
		int xNew = 0x11223344;
		int yNew = 0x99887766;
		
		Object point2D = makePoint2D.invoke(x, y);
		
		assertEquals(getX.invoke(point2D), x);
		assertEquals(getY.invoke(point2D), y);
		
		point2D = withX.invoke(point2D, xNew);
		point2D = withY.invoke(point2D, yNew);
		
		assertEquals(getX.invoke(point2D), xNew);
		assertEquals(getY.invoke(point2D), yNew);
	}

	/*
	 * Create a value type with double slot primative members
	 * 
	 * value Point2DComplex {
	 * 	double d;
	 * 	long j;
	 * }
	 */
	@Test(priority=1)
	static public void testCreatePoint2DComplex() throws Throwable {
		String fields[] = {"d:D", "j:J"};
		Class point2DComplexClass = ValueTypeGenerator.generateValueClass("Point2DComplex", fields);
		
		MethodHandle makePoint2DComplex = lookup.findStatic(point2DComplexClass, "makeValue", MethodType.methodType(point2DComplexClass, double.class, long.class));

		MethodHandle getD = generateGetter(point2DComplexClass, "d", double.class);
		MethodHandle withD = generateWither(point2DComplexClass, "d", double.class);
		MethodHandle getJ = generateGetter(point2DComplexClass, "j", long.class);
		MethodHandle withJ = generateWither(point2DComplexClass, "j", long.class);
		
		double d = Double.MAX_VALUE;
		long j = Long.MAX_VALUE;
		double dNew = Long.MIN_VALUE;
		long jNew = Long.MIN_VALUE;
		Object point2D = makePoint2DComplex.invoke(d, j);
		
		assertEquals(getD.invoke(point2D), d);
		assertEquals(getJ.invoke(point2D), j);
		
		point2D = withD.invoke(point2D, dNew);
		point2D = withJ.invoke(point2D, jNew);
		assertEquals(getD.invoke(point2D), dNew);
		assertEquals(getJ.invoke(point2D), jNew);


		MethodHandle getDGeneric = generateGenericGetter(point2DComplexClass, "d");
		MethodHandle withDGeneric = generateGenericWither(point2DComplexClass, "d");
		MethodHandle getJGeneric = generateGenericGetter(point2DComplexClass, "j");
		MethodHandle withJGeneric = generateGenericWither(point2DComplexClass, "j");
		
		point2D = withDGeneric.invoke(point2D, d);
		point2D = withJGeneric.invoke(point2D, j);
		assertEquals(getDGeneric.invoke(point2D), d);
		assertEquals(getJGeneric.invoke(point2D), j);
	}

	/*
	 * Test with nested values in reference type
	 * 
	 * value Line2D {
	 * 	Point2D st;
	 * 	Point2D en;
	 * }
	 * 
	 */
	@Test(priority=2)
	static public void testCreateLine2D() throws Throwable {
		String fields[] = {"st:LPoint2D;:value", "en:LPoint2D;:value"};
		line2DClass = ValueTypeGenerator.generateValueClass("Line2D", fields);
		
		makeLine2D = lookup.findStatic(line2DClass, "makeValue", MethodType.methodType(line2DClass, point2DClass, point2DClass));
		
		MethodHandle getSt = generateGetter(line2DClass, "st", point2DClass);
 		MethodHandle withSt = generateWither(line2DClass, "st", point2DClass);
 		MethodHandle getEn = generateGetter(line2DClass, "en", point2DClass);
 		MethodHandle withEn = generateWither(line2DClass, "en", point2DClass);
 		
		int x = 0xFFEEFFEE;
		int y = 0xAABBAABB;
		int xNew = 0x11223344;
		int yNew = 0x99887766;
		int x2 = 0xCCDDCCDD;
		int y2 = 0xAAFFAAFF;
		int x2New = 0x55337799;
		int y2New = 0x88662244;
		
		Object st = makePoint2D.invoke(x, y);
		Object en = makePoint2D.invoke(x2, y2);
		
		assertEquals(getX.invoke(st), x);
		assertEquals(getY.invoke(st), y);
		assertEquals(getX.invoke(en), x2);
		assertEquals(getY.invoke(en), y2);
		
		Object line2D = makeLine2D.invoke(st, en);
		
		assertEquals(getX.invoke(getSt.invoke(line2D)), x);
		assertEquals(getY.invoke(getSt.invoke(line2D)), y);
		assertEquals(getX.invoke(getEn.invoke(line2D)), x2);
		assertEquals(getY.invoke(getEn.invoke(line2D)), y2);
		
		Object stNew = makePoint2D.invoke(xNew, yNew);
		Object enNew = makePoint2D.invoke(x2New, y2New);
		
		line2D = withSt.invoke(line2D, stNew);
		line2D = withEn.invoke(line2D, enNew);
		
		assertEquals(getX.invoke(getSt.invoke(line2D)), xNew);
		assertEquals(getY.invoke(getSt.invoke(line2D)), yNew);
		assertEquals(getX.invoke(getEn.invoke(line2D)), x2New);
		assertEquals(getY.invoke(getEn.invoke(line2D)), y2New);
	}
	
	/*
	 * Test with nested values in reference type
	 * 
	 * value FlattenedLine2D {
	 * 	flattened Point2D st;
	 * 	flattened Point2D en;
	 * }
	 * 
	 */
	@Test(priority=2)
	static public void testCreateFlattenedLine2D() throws Throwable {
		String fields[] = {"st:QPoint2D;:value", "en:QPoint2D;:value"};
		flattenedLine2DClass = ValueTypeGenerator.generateValueClass("FlattenedLine2D", fields);
				
		makeFlattenedLine2D = lookup.findStatic(flattenedLine2DClass, "makeValueGeneric", MethodType.methodType(flattenedLine2DClass, Object.class, Object.class));
		
		getFlatSt = generateGenericGetter(flattenedLine2DClass, "st");
 		withFlatSt = generateGenericWither(flattenedLine2DClass, "st");
 		getFlatEn = generateGenericGetter(flattenedLine2DClass, "en");
 		withFlatEn = generateGenericWither(flattenedLine2DClass, "en");
 		
		int x = 0xFFEEFFEE;
		int y = 0xAABBAABB;
		int xNew = 0x11223344;
		int yNew = 0x99887766;
		int x2 = 0xCCDDCCDD;
		int y2 = 0xAAFFAAFF;
		int x2New = 0x55337799;
		int y2New = 0x88662244;
		
		Object st = makePoint2D.invoke(x, y);
		Object en = makePoint2D.invoke(x2, y2);
		
		assertEquals(getX.invoke(st), x);
		assertEquals(getY.invoke(st), y);
		assertEquals(getX.invoke(en), x2);
		assertEquals(getY.invoke(en), y2);
		
		Object line2D = makeFlattenedLine2D.invoke(st, en);
		
		assertEquals(getX.invoke(getFlatSt.invoke(line2D)), x);
		assertEquals(getY.invoke(getFlatSt.invoke(line2D)), y);
		assertEquals(getX.invoke(getFlatEn.invoke(line2D)), x2);
		assertEquals(getY.invoke(getFlatEn.invoke(line2D)), y2);
		
		Object stNew = makePoint2D.invoke(xNew, yNew);
		Object enNew = makePoint2D.invoke(x2New, y2New);
		
		line2D = withFlatSt.invoke(line2D, stNew);
		line2D = withFlatEn.invoke(line2D, enNew);
		
		assertEquals(getX.invoke(getFlatSt.invoke(line2D)), xNew);
		assertEquals(getY.invoke(getFlatSt.invoke(line2D)), yNew);
		assertEquals(getX.invoke(getFlatEn.invoke(line2D)), x2New);
		assertEquals(getY.invoke(getFlatEn.invoke(line2D)), y2New);
	}
	
	@Test(priority=3)
	static public void testCreateFlattenedTriangle2D() throws Throwable {
		String fields[] = {"ll:J", "l1:QFlattenedLine2D;:value", "l2:QFlattenedLine2D;:value", "l3:QFlattenedLine2D;:value"};
		flattenedTriangle2DClass = ValueTypeGenerator.generateValueClass("FlattenedTriangle2D", fields);
		
		makeFlattenedTriangle2D = lookup.findStatic(flattenedTriangle2DClass, "makeValueGeneric", MethodType.methodType(flattenedTriangle2DClass, Object.class, Object.class, Object.class, Object.class));
		
		MethodHandle getl1 = generateGenericGetter(flattenedTriangle2DClass, "l1");
 		MethodHandle withl1 = generateGenericWither(flattenedTriangle2DClass, "l1");
 		MethodHandle getl2 = generateGenericGetter(flattenedTriangle2DClass, "l2");
 		MethodHandle withl2 = generateGenericWither(flattenedTriangle2DClass, "l2");
 		MethodHandle getl3 = generateGenericGetter(flattenedTriangle2DClass, "l3");
 		MethodHandle withl3 = generateGenericWither(flattenedTriangle2DClass, "l3");
 		MethodHandle getll = generateGetter(flattenedTriangle2DClass, "ll", long.class);
 		
		int x = 0xFFEEFFEE;
		int y = 0xAABBAABB;
		int xNew = 0x11223344;
		int yNew = 0x99887766;
		int x2 = 0xCCDDCCDD;
		int y2 = 0xAAFFAAFF;
		int x2New = 0x55337799;
		int y2New = 0x88662244;
		int x3 = 0x11221122;
		int y3 = 0xAABBAABB;
		
		Object v1 = makePoint2D.invoke(x, y);
		Object v2 = makePoint2D.invoke(x2, y2);
		Object v3 = makePoint2D.invoke(x3, y3);
		
		Object line1 = makeFlattenedLine2D.invoke(v1, v2);
		Object line2 = makeFlattenedLine2D.invoke(v2, v3);
		Object line3 = makeFlattenedLine2D.invoke(v3, v1);
		
		Object triangle = makeFlattenedTriangle2D.invoke(new Long(0x1234500054321000L), line1, line2, line3);
		
		assertEquals(getll.invoke(triangle), 0x1234500054321000L);
		
		//checkValue(triangle);
		
		assertEquals(getX.invoke(getFlatSt.invoke(getl3.invoke(triangle))), x3);
		assertEquals(getX.invoke(getFlatEn.invoke(getl3.invoke(triangle))), x);
		assertEquals(getY.invoke(getFlatSt.invoke(getl3.invoke(triangle))), y3);
		assertEquals(getY.invoke(getFlatEn.invoke(getl3.invoke(triangle))), y);
		
		Object v2New = makePoint2D.invoke(x2New, y2New);
		Object line2New = makeFlattenedLine2D.invoke(v2New, v3);
		
		triangle = withl3.invoke(triangle, line2New);
		
		assertEquals(getX.invoke(getFlatSt.invoke(getl3.invoke(triangle))), x2New);
		assertEquals(getX.invoke(getFlatEn.invoke(getl3.invoke(triangle))), x3);
		assertEquals(getY.invoke(getFlatSt.invoke(getl3.invoke(triangle))), y2New);
		assertEquals(getY.invoke(getFlatEn.invoke(getl3.invoke(triangle))), y3);
		
		assertEquals(getFieldOffset(flattenedTriangle2DClass, "ll"), 8);
		assertEquals(getFieldOffset(flattenedTriangle2DClass, "l1"), 16);
		assertEquals(getFieldOffset(flattenedTriangle2DClass, "l2"), 32);
		assertEquals(getFieldOffset(flattenedTriangle2DClass, "l3"), 48);
	}
	
	/*
	 * Test with nested values in reference type
	 * 
	 * value LargeValueWithObjects {
	 * 	flattened FlattenedTriangle2D l1;
	 * 	flattened FlattenedTriangle2D l2;
	 *  flattened FlattenedTriangle2D l3;
	 *  Object o;
	 *  flattened FlattenedTriangle2D l4;
	 *  Object o2;
	 * }
	 * 
	 */
	@Test(priority=4)
	static public void testCreateLargeValueWithObjects() throws Throwable {
		Class valObjectClass = ValueTypeGenerator.generateValueClass("ValObject", new String[] {"o:Ljava/lang/Object;"});
		MethodHandle makeValObject = lookup.findStatic(valObjectClass, "makeValue", MethodType.methodType(valObjectClass, Object.class));
		
		String fields2[] = {"o1:QValObject;", "o2:QValObject;", "o3:QValObject;", "o4:QValObject;", "o5:QValObject;", "o6:QValObject;", "o7:QValObject;", "o8:QValObject;", "o9:QValObject;", "o10:QValObject;", "o11:QValObject;", "o12:QValObject;", "o13:QValObject;", "o14:QValObject;", "o15:QValObject;", "o16:QValObject;"};
		
		Class bigValObjectClass = ValueTypeGenerator.generateValueClass("BigValObject", fields2);
		MethodHandle makeBigValObject = lookup.findStatic(bigValObjectClass, "makeValueGeneric", MethodType.methodType(bigValObjectClass, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class));
		
		assertEquals(getFieldOffset(bigValObjectClass, "o1"), 4);
		assertEquals(getFieldOffset(bigValObjectClass, "o2"), 8);
		assertEquals(getFieldOffset(bigValObjectClass, "o3"), 12);
		assertEquals(getFieldOffset(bigValObjectClass, "o4"), 16);
		assertEquals(getFieldOffset(bigValObjectClass, "o5"), 20);
		assertEquals(getFieldOffset(bigValObjectClass, "o6"), 24);
		assertEquals(getFieldOffset(bigValObjectClass, "o7"), 28);
		assertEquals(getFieldOffset(bigValObjectClass, "o8"), 32);
		assertEquals(getFieldOffset(bigValObjectClass, "o9"), 36);
		assertEquals(getFieldOffset(bigValObjectClass, "o10"), 40);
		assertEquals(getFieldOffset(bigValObjectClass, "o11"), 44);
		assertEquals(getFieldOffset(bigValObjectClass, "o12"), 48);
		assertEquals(getFieldOffset(bigValObjectClass, "o13"), 52);
		assertEquals(getFieldOffset(bigValObjectClass, "o14"), 56);
		assertEquals(getFieldOffset(bigValObjectClass, "o15"), 60);
		assertEquals(getFieldOffset(bigValObjectClass, "o16"), 64);
		
		String fields3[] = {"o1:QBigValObject;", "o2:QBigValObject;", "o3:QBigValObject;", "o4:QBigValObject;", "o5:QBigValObject;", "o6:QBigValObject;", "o7:QBigValObject;", "o8:QBigValObject;"};
		Class megaValObjectClass = ValueTypeGenerator.generateValueClass("MegaValObject", fields3);
		MethodHandle makeMegaValObject = lookup.findStatic(megaValObjectClass, "makeValueGeneric", MethodType.methodType(megaValObjectClass, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class));
		
		assertEquals(getFieldOffset(megaValObjectClass, "o1"), 4);
		assertEquals(getFieldOffset(megaValObjectClass, "o2"), 68);
		assertEquals(getFieldOffset(megaValObjectClass, "o3"), 132);
		assertEquals(getFieldOffset(megaValObjectClass, "o4"), 196);
		assertEquals(getFieldOffset(megaValObjectClass, "o5"), 260);
		assertEquals(getFieldOffset(megaValObjectClass, "o6"), 324);
		assertEquals(getFieldOffset(megaValObjectClass, "o7"), 388);
		assertEquals(getFieldOffset(megaValObjectClass, "o8"), 452);
		
		String fields[] = {"l1:QFlattenedTriangle2D;:value","ll:J", "l2:QFlattenedTriangle2D;:value", "l3:QFlattenedTriangle2D;:value", "o:Ljava/lang/Object;", "l4:QFlattenedTriangle2D;:value", "o2:Ljava/lang/Object;", "big:QBigValObject;", "mega:QMegaValObject;"};
		Class largeValueWithObjectsClass = ValueTypeGenerator.generateValueClass("LargeValueWithObjects", fields);
		MethodHandle makeLargeValueWithObjects = lookup.findStatic(largeValueWithObjectsClass, "makeValueGeneric", MethodType.methodType(largeValueWithObjectsClass, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class, Object.class));
		
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "l1"), 8);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "l2"), 60);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "l3"), 112);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "l4"), 164);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "ll"), 216);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "big"), 224);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "mega"), 288);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "o"), 800);
//		assertEquals(getFieldOffset(largeValueWithObjectsClass, "o2"), 804);
		
		String fields4[] = {"ll:J", "l1:QFlattenedTriangle2D;:value", "o2:QBigValObject;"};
		Class largeRefClass = ValueTypeGenerator.generateRefClass("LargeRef", fields4);
		MethodHandle makeLargeRef = lookup.findStatic(largeRefClass, "makeRefGeneric", MethodType.methodType(largeRefClass, Object.class, Object.class, Object.class));
		
		assertEquals(getFieldOffset(largeRefClass, "l1"), 4);
		assertEquals(getFieldOffset(largeRefClass, "ll"), 64);
		assertEquals(getFieldOffset(largeRefClass, "o2"), 72);
		
		MethodHandle getll = generateGetter(largeValueWithObjectsClass, "ll", long.class);
		MethodHandle getl2 = generateGenericGetter(largeValueWithObjectsClass, "l2");
		MethodHandle geto = generateGetter(largeValueWithObjectsClass, "o", Object.class);
		MethodHandle geto2 = generateGetter(largeValueWithObjectsClass, "o2", Object.class);
		
		int x = 0xFFEEFFEE;
		int y = 0xAABBAABB;
		int xNew = 0x11223344;
		int yNew = 0x99887766;
		int x2 = 0xCCDDCCDD;
		int y2 = 0xAAFFAAFF;
		int x2New = 0x55337799;
		int y2New = 0x88662244;
		int x3 = 0x11221122;
		int y3 = 0xAABBAABB;
		
		Object v1 = makePoint2D.invoke(x, y);
		Object v2 = makePoint2D.invoke(x2, y2);
		Object v3 = makePoint2D.invoke(x3, y3);
		
		Object line1 = makeFlattenedLine2D.invoke(v1, v2);
		Object line2 = makeFlattenedLine2D.invoke(v2, v3);
		Object line3 = makeFlattenedLine2D.invoke(v3, v1);
		
		Object triangle = makeFlattenedTriangle2D.invoke(new Long(0x98796543211234L), line1, line2, line3);
		
		Object valObject = makeValObject.invoke(new Object());
		
		Object bigVal = makeBigValObject.invoke(makeValObject.invoke(new Object()), valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, valObject, makeValObject.invoke(new Object()));
		
		Object regObject = new Object();
		
		Object megaObject = makeMegaValObject.invoke(bigVal, bigVal, bigVal, bigVal, bigVal, bigVal, bigVal, bigVal);
		
		Object largeObject = makeLargeValueWithObjects.invoke(triangle, new Long(0x123456787654321L), triangle, triangle, "hello", triangle, "goodbye", bigVal, megaObject);
		
		Object largeRef = makeLargeRef.invoke(new Long(0x123456787654321L), triangle, bigVal);
		
		checkValue(largeObject, bigVal, regObject, largeRef, triangle, line1, v1);
		
		
		assertEquals(getll.invoke(largeObject), 0x123456787654321L);
		assertEquals(geto.invoke(largeObject), "hello");
		assertEquals(geto2.invoke(largeObject), "goodbye");
		triangle = getl2.invoke(largeObject);
		
		MethodHandle getl2t = generateGenericGetter(flattenedTriangle2DClass, "l2");
		
		assertEquals(getX.invoke(getFlatSt.invoke(getl2t.invoke(triangle))), x2);
		assertEquals(getX.invoke(getFlatEn.invoke(getl2t.invoke(triangle))), x3);
		assertEquals(getY.invoke(getFlatSt.invoke(getl2t.invoke(triangle))), y2);
		assertEquals(getY.invoke(getFlatEn.invoke(getl2t.invoke(triangle))), y3);
		
		Class valLongClass = ValueTypeGenerator.generateValueClass("ValLong", new String[] {"l:J"});
		MethodHandle makeValLong = lookup.findStatic(valLongClass, "makeValue", MethodType.methodType(valLongClass, long.class));
		
		Class long256Class = ValueTypeGenerator.generateValueClass("Long256", new String[] {"l:QValLong;", "l:QValLong;","l:QValLong;","l:QValLong;"});
		MethodHandle makeLong256 = lookup.findStatic(long256Class, "makeValueGeneric", MethodType.methodType(long256Class, Object.class, Object.class, Object.class, Object.class));
		
	}

	static void checkValue(Object ... o) {
		com.ibm.jvm.Dump.SystemDump();
	}
	

//	/*
//	 * Test with nested values
//	 * 
//	 * value InvalidField {
//	 * 	flattened Point2D st;
//	 * 	flattened Invalid x;
//	 * }
//	 * 
//	 */
//	@Test(priority=3)
//	static public void testInvalidNestedField() throws Throwable {
//		String fields[] = {"st:QPoint2D;:value", "x:QInvalid;:value"};
//
//		try {
//			Class<?> invalidField = ValueTypeGenerator.generateValueClass("InvalidField", fields);
//			Assert.fail("should throw error. Nested class doesn't exist!");
//		} catch (NoClassDefFoundError e) {}
//	}
//	
//	/*
//	 * Test with none value Qtype
//	 * 
//	 * value NoneValueQType {
//	 * 	flattened Point2D st;
//	 * 	flattened Object o;
//	 * }
//	 * 
//	 */
//	@Test(priority=3)
//	static public void testNoneValueQTypeAsNestedField() throws Throwable {
//		String fields[] = {"st:QPoint2D;:value", "o:Qjava/lang/Object;:value"};
//		try {
//			Class<?> noneValueQType = ValueTypeGenerator.generateValueClass("NoneValueQType", fields);
//			Assert.fail("should throw error. j.l.Object is not a qtype!");
//		} catch (IncompatibleClassChangeError e) {}
//	}
//	
//	/*
//	 * Test defaultValue with ref type
//	 * 
//	 * class DefaultValueWithNoneValueType {
//	 * 	Object f1;
//	 * 	Object f1;
//	 * }
//	 * 
//	 */
//	@Test(priority=3)
//	static public void testDefaultValueWithNonValueType() throws Throwable {
//		String fields[] = {"f1:Ljava/lang/Object;:value", "f2:Ljava/lang/Object;:value"};
//		Class<?> defaultValueWithNonValueType = ValueTypeGenerator.generateRefClass("DefaultValueWithNonValueType", fields);
//		MethodHandle makeDefaultValueWithNonValueType = lookup.findStatic(defaultValueWithNonValueType, "makeValue", MethodType.methodType(defaultValueWithNonValueType, Object.class, Object.class));
//		try {
//			makeDefaultValueWithNonValueType.invoke(null, null);
//			Assert.fail("should throw error. Default value must be used with ValueType");
//		} catch (IncompatibleClassChangeError e) {}
//	}
//	
//	/*
//	 * Test withField on non Value Type
//	 * 
//	 * class TestWithFieldOnNonValueType {
//	 *  long longField
//	 * }
//	 */
//	@Test(priority=1)
//	static public void testWithFieldOnNonValueType() throws Throwable {
//		String fields[] = {"longField:J"};
//		Class<?> testWithFieldOnNonValueType = ValueTypeGenerator.generateRefClass("TestWithFieldOnNonValueType", fields);
//		MethodHandle withFieldOnNonValueType = lookup.findStatic(testWithFieldOnNonValueType, "testWithFieldOnNonValueType", MethodType.methodType(Object.class));
//		try {
//			withFieldOnNonValueType.invoke();
//			Assert.fail("should throw error. WithField must be used with ValueType");
//		} catch (IncompatibleClassChangeError e) {}
//	}
//	
//	/*
//	 * Test withField on non Null type
//	 * 
//	 * class TestWithFieldOnNull {
//	 *  long longField
//	 * }
//	 */
//	@Test(priority=1)
//	static public void testWithFieldOnNull() throws Throwable {
//		String fields[] = {"longField:J"};
//		Class<?> testWithFieldOnNull = ValueTypeGenerator.generateRefClass("TestWithFieldOnNull", fields);
//		
//		MethodHandle withFieldOnNull = lookup.findStatic(testWithFieldOnNull, "testWithFieldOnNull", MethodType.methodType(Object.class));
//		try {
//			withFieldOnNull.invoke();
//			Assert.fail("should throw error. Objectref cannot be null");
//		} catch (NullPointerException e) {}
//	}
//	
//	/*
//	 * Test withField on non existent class
//	 * 
//	 * class TestWithFieldOnNonExistentClass {
//	 *  long longField
//	 * }
//	 */
//	@Test(priority=1)
//	static public void testWithFieldOnNonExistentClass() throws Throwable {
//		String fields[] = {"longField:J"};
//		Class<?> testWithFieldOnNonExistentClass = ValueTypeGenerator.generateRefClass("TestWithFieldOnNonExistentClass", fields);
//		MethodHandle withFieldOnNonExistentClass = lookup.findStatic(testWithFieldOnNonExistentClass, "testWithFieldOnNonExistentClass", MethodType.methodType(Object.class));
//		try {
//			withFieldOnNonExistentClass.invoke();
//			Assert.fail("should throw error. Class does not exist");
//		} catch (NoClassDefFoundError e) {}
//	}

	/*
	 * TODO: behaviour of the test between two valueTypes will depend on the new spec(not finialized)
	 * 
	 * Test ifacmp on value class
	 * 
	 * class TestIfacmpOnValueClass {}
	 *
	 *
	 *	@Test(priority=2)
	 *	static public void TestIfacmpOnValueClass() throws Throwable {
	 *	int x = 0;
	 *	int y = 0;
	 *
	 *	Object valueType = makePoint2D.invoke(x, y);
	 *	Object refType = (Object) x;
	 *
	 *	Assert.assertFalse((valueType == refType), "An identity (==) comparison that contains a valueType should always return false");
	 *
	 *	Assert.assertFalse((refType == valueType), "An identity (==) comparison that contains a valueType should always return false");
	 *
	 *	Assert.assertFalse((valueType == valueType), "An identity (==) comparison that contains a valueType should always return false");
	 *
	 *	Assert.assertTrue((refType == refType), "An identity (==) comparison on the same refType should always return true");
	 *
	 *	Assert.assertTrue((valueType != refType), "An identity (!=) comparison that contains a valueType should always return true");
	 *
	 *	Assert.assertTrue((refType != valueType), "An identity (!=) comparison that contains a valueType should always return true");
	 *
	 *	Assert.assertTrue((valueType != valueType), "An identity (!=) comparison that contains a valueType should always return true");
	 *
	 *	Assert.assertFalse((refType != refType), "An identity (!=) comparison on the same refType should always return false");
	 *	}
	 */	
	
	static MethodHandle generateGetter(Class<?> clazz, String fieldName, Class<?> fieldType) {
		try {
			return lookup.findVirtual(clazz, "get"+fieldName, MethodType.methodType(fieldType));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	static MethodHandle generateGenericGetter(Class<?> clazz, String fieldName) {
		try {
			return lookup.findVirtual(clazz, "getGeneric"+fieldName, MethodType.methodType(Object.class));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	static MethodHandle generateSetter(Class clazz, String fieldName, Class fieldType) {
		try {
			return lookup.findVirtual(clazz, "set"+fieldName, MethodType.methodType(void.class, fieldType));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	static MethodHandle generateGenericSetter(Class clazz, String fieldName) {
		try {
			return lookup.findVirtual(clazz, "setGeneric"+fieldName, MethodType.methodType(void.class, Object.class));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	static MethodHandle generateWither(Class clazz, String fieldName, Class fieldType) {
		try {
			return lookup.findVirtual(clazz, "with"+fieldName, MethodType.methodType(clazz, fieldType));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}

	static MethodHandle generateGenericWither(Class clazz, String fieldName) {
		try {
			return lookup.findVirtual(clazz, "withGeneric"+fieldName, MethodType.methodType(clazz, Object.class));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	static long getFieldOffset(Class clazz, String field) {
		try {
			Field f = clazz.getDeclaredField(field);
			return unsafe.objectFieldOffset(f);
		} catch (Throwable t) {
			throw new RuntimeException(t);
		}
	}

}
