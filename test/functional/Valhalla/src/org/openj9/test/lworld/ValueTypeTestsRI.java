/*******************************************************************************
 * Copyright (c) 2018, 2020 IBM Corp. and others
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
import java.lang.reflect.Method;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.ArrayList;
import sun.misc.Unsafe;
import org.testng.Assert;
import static org.testng.Assert.*;
import org.testng.annotations.Test;
import org.testng.annotations.BeforeClass;

/*
 * Instructions to run this test:
 * 
 * 1)  Build the JDK with the '--enable-inline-types' configure flag
 * 2)  cd [openj9-openjdk-dir]/openj9/test
 * 3)  git clone https://github.com/AdoptOpenJDK/TKG.git
 * 4)  cd TKG
 * 5)  export TEST_JDK_HOME=[openj9-openjdk-dir]/build/linux-x86_64-server-release/images/jdk
 * 6)  export JDK_VERSION=Valhalla
 * 7)  export SPEC=linux_x86-64_cmprssptrs
 * 8)  export BUILD_LIST=functional/Valhalla
 * 9)  export AUTO_DETECT=false
 * 10) export JDK_IMPL=openj9 
 * 11) make -f run_configure.mk && make compile && make _sanity
 */

@Test(groups = { "level.sanity" })
public class ValueTypeTestsRI {
	static Lookup lookup = MethodHandles.lookup();
	static Unsafe myUnsafe = null;
	/* point2D */
	static Class point2DClass = null;
	static MethodHandle makePoint2D = null;
	static MethodHandle getX = null;
	static MethodHandle getY = null;
	static MethodHandle withX = null;
	static MethodHandle withY = null;
	/* line2D */
	static Class line2DClass = null;
	static MethodHandle makeLine2D = null;
	/* flattenedLine2D */
	static Class flattenedLine2DClass = null;
	static MethodHandle makeFlattenedLine2D = null;
	static MethodHandle getFlatSt = null;
	static MethodHandle withFlatSt = null;
 	static MethodHandle getFlatEn = null;
 	static MethodHandle withFlatEn = null;
	/* triangle2D */
	static Class triangle2DClass = null;
	static MethodHandle makeTriangle2D = null;
	static MethodHandle getV1 = null;
	static MethodHandle getV2 = null;
	static MethodHandle getV3 = null;
	/* valueLong */
	static Class valueLongClass = null;
	static MethodHandle makeValueLong = null;
	static MethodHandle getLong = null;
	static MethodHandle withLong = null;
	/* valueInt */
	static Class valueIntClass = null;
	static MethodHandle makeValueInt = null;
	static MethodHandle getInt = null;
	static MethodHandle withInt = null;
	/* valueDouble */
	static Class valueDoubleClass = null;
	static MethodHandle makeValueDouble = null;
	static MethodHandle getDouble = null;
	static MethodHandle withDouble = null;
	/* valueFloat */
	static Class valueFloatClass = null;
	static MethodHandle makeValueFloat = null;
	static MethodHandle getFloat = null;
	static MethodHandle withFloat = null;
	/* valueObject */
	static Class valueObjectClass = null;
	static MethodHandle makeValueObject = null;
	static MethodHandle getObject = null;
	static MethodHandle withObject = null;
	/* largeObject */
	static Class largeObjectValueClass = null;
	static MethodHandle makeLargeObjectValue = null;
	static MethodHandle[] getObjects = null;
	/* megaObject */
	static Class megaObjectValueClass = null;
	static MethodHandle makeMegaObjectValue = null;
	/* assortedRefWithLongAlignment */
	static Class assortedRefWithLongAlignmentClass = null;
	static MethodHandle makeAssortedRefWithLongAlignment = null;
	static MethodHandle[][] assortedRefWithLongAlignmentGetterAndSetter = null;
	/* assortedRefWithObjectAlignment */
	static Class assortedRefWithObjectAlignmentClass = null;
	static MethodHandle makeAssortedRefWithObjectAlignment = null;
	static MethodHandle[][] assortedRefWithObjectAlignmentGetterAndWither = null;
	/* assortedRefWithSingleAlignment */
	static Class assortedRefWithSingleAlignmentClass = null;
	static MethodHandle makeAssortedRefWithSingleAlignment = null;
	static MethodHandle[][] assortedRefWithSingleAlignmentGetterAndWither = null;
	/* assortedValueWithLongAlignment */
	static Class assortedValueWithLongAlignmentClass = null;
	static MethodHandle makeAssortedValueWithLongAlignment = null;
	static MethodHandle[][] assortedValueWithLongAlignmentGetterAndWither = null;
	static Class classWithOnlyStaticFieldsWithLongAlignment = null;
	static MethodHandle[][] staticFieldsWithLongAlignmentGenericGetterAndSetter = null;
	/* assortedValueWithObjectAlignment */
	static Class assortedValueWithObjectAlignmentClass = null;
	static MethodHandle makeAssortedValueWithObjectAlignment = null;
	static MethodHandle[][] assortedValueWithObjectAlignmentGetterAndWither = null;
	static Class classWithOnlyStaticFieldsWithObjectAlignment = null;
	static MethodHandle[][] staticFieldsWithObjectAlignmentGenericGetterAndSetter = null;
	/* assortedValueWithSingleAlignment */
	static Class assortedValueWithSingleAlignmentClass = null;
	static MethodHandle makeAssortedValueWithSingleAlignment = null;
	static MethodHandle[][] assortedValueWithSingleAlignmentGetterAndWither = null;
	static Class classWithOnlyStaticFieldsWithSingleAlignment = null;
	static MethodHandle[][] staticFieldsWithSingleAlignmentGenericGetterAndSetter = null;
	
	/* fields */
	static String typeWithSingleAlignmentFields[] = {
		"tri:QTriangle2D;:value",
		"point:QPoint2D;:value",
		"line:QFlattenedLine2D;:value",
		"i:QValueInt;:value",
		"f:QValueFloat;:value",
		"tri2:QTriangle2D;:value"
	};
	static String typeWithObjectAlignmentFields[] = {
		"tri:QTriangle2D;:value",
		"point:QPoint2D;:value",
		"line:QFlattenedLine2D;:value",
		"o:QValueObject;:value",
		"i:QValueInt;:value",
		"f:QValueFloat;:value",
		"tri2:QTriangle2D;:value"
	};
	static String typeWithLongAlignmentFields[] = {
		"point:QPoint2D;:value",
		"line:QFlattenedLine2D;:value",
		"o:QValueObject;:value",
		"l:QValueLong;:value",
		"d:QValueDouble;:value",
		"i:QValueInt;:value",
		"tri:QTriangle2D;:value"
	};
	
	/* default values */
	static int[] defaultPointPositions1 = {0xFFEEFFEE, 0xAABBAABB};
	static int[] defaultPointPositions2 = {0xCCDDCCDD, 0x33443344};
	static int[] defaultPointPositions3 = {0x43211234, 0xABCDDCBA};
	static int[] defaultPointPositionsEmpty = {0, 0};
	static int[][] defaultLinePositions1 = {defaultPointPositions1, defaultPointPositions2};
	static int[][] defaultLinePositions2 = {defaultPointPositions2, defaultPointPositions3};
	static int[][] defaultLinePositions3 = {defaultPointPositions1, defaultPointPositions3};
	static int[][] defaultLinePositionsEmpty = {defaultPointPositionsEmpty, defaultPointPositionsEmpty};
	static int[][][] defaultTrianglePositions = {defaultLinePositions1, defaultLinePositions2, defaultLinePositions3};
	static long defaultLong = Long.MAX_VALUE;
	static int defaultInt = Integer.MAX_VALUE;
	static double defaultDouble = Double.MAX_VALUE;
	static float defaultFloat = Float.MAX_VALUE;
	static Object defaultObject = (Object)0xEEFFEEFF;
	static int[] defaultPointPositionsNew = {0xFF112233, 0xFF332211};
	static int[][] defaultLinePositionsNew = {defaultPointPositionsNew, defaultPointPositions1};
	static int[][][] defaultTrianglePositionsNew = {defaultLinePositionsNew, defaultLinePositions3, defaultLinePositions1};
	static int[][][] defaultTrianglePositionsEmpty = {defaultLinePositionsEmpty, defaultLinePositionsEmpty, defaultLinePositionsEmpty};
	static long defaultLongNew = -1234123L;
	static int defaultIntNew = -1234123234;
	static double defaultDoubleNew = -123412341.21341234d;
	static float defaultFloatNew = -123423.12341234f;
	static Object defaultObjectNew = (Object)0xFFEEFFEE;
	/* miscellaneous constants */
	static final int genericArraySize = 10;
	static final int objectGCScanningIterationCount = 10000;

	@BeforeClass
	static public void testSetUp() throws RuntimeException {
		try {
			Field f = Unsafe.class.getDeclaredField("theUnsafe");
			f.setAccessible(true);
			myUnsafe = (Unsafe)f.get(null);
		} catch (NoSuchFieldException | SecurityException | IllegalArgumentException | IllegalAccessException e) {
			throw new RuntimeException(e);
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
		
		makePoint2D = lookup.findStatic(point2DClass, "makeValueGeneric", MethodType.methodType(Object.class, Object.class, Object.class));
		
		getX = generateGenericGetter(point2DClass, "x");
		withX = generateGenericWither(point2DClass, "x");
		getY = generateGenericGetter(point2DClass, "y");
		withY = generateGenericWither(point2DClass, "y");

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
	
	static MethodHandle generateStaticGenericGetter(Class<?> clazz, String fieldName) {
		try {
			return lookup.findStatic(clazz, "getStaticGeneric"+fieldName, MethodType.methodType(Object.class));
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

	static MethodHandle generateStaticGenericSetter(Class clazz, String fieldName) {
		try {
			return lookup.findStatic(clazz, "setStaticGeneric"+fieldName, MethodType.methodType(void.class, Object.class));
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
			return lookup.findVirtual(clazz, "withGeneric"+fieldName, MethodType.methodType(Object.class, Object.class));
		} catch (IllegalAccessException | SecurityException | NullPointerException | NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}

	static long getFieldOffset(Class clazz, String field) {
		try {
			Field f = clazz.getDeclaredField(field);
			return myUnsafe.objectFieldOffset(f);
		} catch (Throwable t) {
			throw new RuntimeException(t);
		}
	}

	static MethodHandle[][] generateGenericGetterAndWither(Class clazz, String[] fields) {
		MethodHandle[][] getterAndWither = new MethodHandle[fields.length][2];
		for (int i = 0; i < fields.length; i++) {
			String field = (fields[i].split(":"))[0];
			getterAndWither[i][0] = generateGenericGetter(clazz, field);
			getterAndWither[i][1] = generateGenericWither(clazz, field);
		}
		return getterAndWither;
	}

	static MethodHandle[][] generateGenericGetterAndSetter(Class clazz, String[] fields) {
		MethodHandle[][] getterAndSetter = new MethodHandle[fields.length][2];
		for (int i = 0; i < fields.length; i++) {
			String field = (fields[i].split(":"))[0];
			getterAndSetter[i][0] = generateGenericGetter(clazz, field);
			getterAndSetter[i][1] = generateGenericSetter(clazz, field);
		}
		return getterAndSetter;
	}

	static MethodHandle[][] generateStaticGenericGetterAndSetter(Class clazz, String[] fields) {
		MethodHandle[][] getterAndSetter = new MethodHandle[fields.length][2];
		for (int i = 0; i < fields.length; i++) {
			String field = (fields[i].split(":"))[0];
			getterAndSetter[i][0] = generateStaticGenericGetter(clazz, field);
			getterAndSetter[i][1] = generateStaticGenericSetter(clazz, field);
		}
		return getterAndSetter;
	}

	static Object createPoint2D(int[] positions) throws Throwable {
		return makePoint2D.invoke(positions[0], positions[1]);
	}

	static Object createFlattenedLine2D(int[][] positions) throws Throwable {
		return makeFlattenedLine2D.invoke(makePoint2D.invoke(positions[0][0], positions[0][1]),
				makePoint2D.invoke(positions[1][0], positions[1][1]));
	}

	static Object createTriangle2D(int[][][] positions) throws Throwable {
		return makeTriangle2D.invoke(
				makeFlattenedLine2D.invoke(makePoint2D.invoke(positions[0][0][0], positions[0][0][1]),
						makePoint2D.invoke(positions[0][1][0], positions[0][1][1])),
				makeFlattenedLine2D.invoke(makePoint2D.invoke(positions[1][0][0], positions[1][0][1]),
						makePoint2D.invoke(positions[1][1][0], positions[1][1][1])),
				makeFlattenedLine2D.invoke(makePoint2D.invoke(positions[2][0][0], positions[2][0][1]),
						makePoint2D.invoke(positions[2][1][0], positions[2][1][1])));
	}

	static Object createLargeObject(Object arg) throws Throwable {
		Object[] args = new Object[16];
		for(int i = 0; i < 16; i++) {
			Object valueObject = makeValueObject.invoke(arg);
			args[i] = valueObject;
		}
		return makeLargeObjectValue.invokeWithArguments(args);
	}

	static Object createMegaObject(Object arg) throws Throwable {
		Object[] args = new Object[16];
		for(int i = 0; i < 16; i++) {
			Object valueObject = createLargeObject(arg);
			args[i] = valueObject;
		}
		return makeMegaObjectValue.invokeWithArguments(args);
	}

	static Object createAssorted(MethodHandle makeMethod, String[] fields) throws Throwable {
		return createAssorted(makeMethod, fields, null);
	}
	static Object createAssorted(MethodHandle makeMethod, String[] fields, Object[] initFields) throws Throwable {
		Object[] args = new Object[fields.length];
		boolean useInitFields = initFields != null;
		for (int i = 0; i < fields.length; i++) {
			String nameAndSigValue[] = fields[i].split(":");
			String signature = nameAndSigValue[1];
			switch (signature) {
			case "QPoint2D;":
				args[i] = createPoint2D(useInitFields ? (int[])initFields[i] : defaultPointPositions1);
				break;
			case "QFlattenedLine2D;":
				args[i] = createFlattenedLine2D(useInitFields ? (int[][])initFields[i] : defaultLinePositions1);
				break;
			case "QTriangle2D;":
				args[i] = createTriangle2D(useInitFields ? (int[][][])initFields[i] : defaultTrianglePositions);
				break;
			case "QValueInt;":
				args[i] = makeValueInt.invoke(useInitFields ? (int)initFields[i] : defaultInt);
				break;
			case "QValueFloat;":
				args[i] = makeValueFloat.invoke(useInitFields ? (float)initFields[i] : defaultFloat);
				break;
			case "QValueDouble;":
				args[i] = makeValueDouble.invoke(useInitFields ? (double)initFields[i] : defaultDouble);
				break;
			case "QValueObject;":
				args[i] = makeValueObject.invoke(useInitFields ? (Object)initFields[i] : defaultObject);
				break;
			case "QValueLong;":
				args[i] = makeValueLong.invoke(useInitFields ? (long)initFields[i] : defaultLong);
				break;
			case "QLargeObject;":
				args[i] = createLargeObject(useInitFields ? (Object)initFields[i] : defaultObject);
				break;
			default:
				args[i] = null;
				break;
			}
		}
		return makeMethod.invokeWithArguments(args);
	}

	static void initializeStaticFields(Class clazz, MethodHandle[][] getterAndSetter, String[] fields) throws Throwable {
		Object defaultValue = null;
		for (int i = 0; i < fields.length; i++) {
			String signature = (fields[i].split(":"))[1];
			switch (signature) {
			case "QPoint2D;":
				defaultValue = createPoint2D(defaultPointPositions1);
				break;
			case "QFlattenedLine2D;":
				defaultValue = createFlattenedLine2D(defaultLinePositions1);
				break;
			case "QTriangle2D;":
				defaultValue = createTriangle2D(defaultTrianglePositions);
				break;
			case "QValueInt;":
				defaultValue = makeValueInt.invoke(defaultInt);
				break;
			case "QValueFloat;":
				defaultValue = makeValueFloat.invoke(defaultFloat);
				break;
			case "QValueDouble;":
				defaultValue = makeValueDouble.invoke(defaultDouble);
				break;
			case "QValueObject;":
				defaultValue = makeValueObject.invoke(defaultObject);
				break;
			case "QValueLong;":
				defaultValue = makeValueLong.invoke(defaultLong);
				break;
			case "QLargeObject;":
				defaultValue = createLargeObject(defaultObject);
				break;
			default:
				defaultValue = null;
				break;
			}
			getterAndSetter[i][1].invoke(defaultValue);
		}
	}

	static Object checkFieldAccessMHOfAssortedType(MethodHandle[][] fieldAccessMHs, Object instance, String[] fields,
			boolean ifValue)
			throws Throwable {
		for (int i = 0; i < fields.length; i++) {
			String nameAndSigValue[] = fields[i].split(":");
			String signature = nameAndSigValue[1];
			switch (signature) {
			case "QPoint2D;":
				checkEqualPoint2D(fieldAccessMHs[i][0].invoke(instance), defaultPointPositions1);
				Object pointNew = createPoint2D(defaultPointPositionsNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, pointNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, pointNew);
				}
				checkEqualPoint2D(fieldAccessMHs[i][0].invoke(instance), defaultPointPositionsNew);
				break;
			case "QFlattenedLine2D;":
				checkEqualFlattenedLine2D(fieldAccessMHs[i][0].invoke(instance), defaultLinePositions1);
				Object lineNew = createFlattenedLine2D(defaultLinePositionsNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, lineNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, lineNew);
				}
				checkEqualFlattenedLine2D(fieldAccessMHs[i][0].invoke(instance), defaultLinePositionsNew);
				break;
			case "QTriangle2D;":
				checkEqualTriangle2D(fieldAccessMHs[i][0].invoke(instance), defaultTrianglePositions);
				Object triNew = createTriangle2D(defaultTrianglePositionsNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, triNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, triNew);
				}
				checkEqualTriangle2D(fieldAccessMHs[i][0].invoke(instance), defaultTrianglePositionsNew);
				break;
			case "QValueInt;":
				assertEquals(getInt.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultInt);
				Object iNew = makeValueInt.invoke(defaultIntNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, iNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, iNew);
				}
				assertEquals(getInt.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultIntNew);
				break;
			case "QValueFloat;":
				assertEquals(getFloat.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultFloat);
				Object fNew = makeValueFloat.invoke(defaultFloatNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, fNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, fNew);
				}
				assertEquals(getFloat.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultFloatNew);
				break;
			case "QValueDouble;":
				assertEquals(getDouble.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultDouble);
				Object dNew = makeValueDouble.invoke(defaultDoubleNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, dNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, dNew);
				}
				assertEquals(getDouble.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultDoubleNew);
				break;
			case "QValueObject;":
				assertEquals(getObject.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultObject);
				Object oNew = makeValueObject.invoke(defaultObjectNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, oNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, oNew);
				}
				assertEquals(getObject.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultObjectNew);
				break;
			case "QValueLong;":
				assertEquals(getLong.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultLong);
				Object lNew = makeValueLong.invoke(defaultLongNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, lNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, lNew);
				}
				assertEquals(getLong.invoke(fieldAccessMHs[i][0].invoke(instance)), defaultLongNew);
				break;
			case "QLargeObject;":
				checkEqualLargeObject(fieldAccessMHs[i][0].invoke(instance), defaultObject);
				Object largeNew = createLargeObject(defaultObjectNew);
				if (ifValue) {
					instance = fieldAccessMHs[i][1].invoke(instance, largeNew);
				} else {
					fieldAccessMHs[i][1].invoke(instance, largeNew);
				}
				checkEqualLargeObject(fieldAccessMHs[i][0].invoke(instance), defaultObjectNew);
				break;
			default:
				break;
			}
		}
		return instance;
	}

	static void checkFieldAccessMHOfStaticType(MethodHandle[][] fieldAccessMHs, String[] fields)
			throws Throwable {
		for (int i = 0; i < fields.length; i++) {
			String nameAndSigValue[] = fields[i].split(":");
			String signature = nameAndSigValue[1];
			switch (signature) {
			case "QPoint2D;":
				checkEqualPoint2D(fieldAccessMHs[i][0].invoke(), defaultPointPositions1);
				Object pointNew = createPoint2D(defaultPointPositionsNew);
				fieldAccessMHs[i][1].invoke(pointNew);
				checkEqualPoint2D(fieldAccessMHs[i][0].invoke(), defaultPointPositionsNew);
				break;
			case "QFlattenedLine2D;":
				checkEqualFlattenedLine2D(fieldAccessMHs[i][0].invoke(), defaultLinePositions1);
				Object lineNew = createFlattenedLine2D(defaultLinePositionsNew);
				fieldAccessMHs[i][1].invoke(lineNew);
				checkEqualFlattenedLine2D(fieldAccessMHs[i][0].invoke(), defaultLinePositionsNew);
				break;
			case "QTriangle2D;":
				checkEqualTriangle2D(fieldAccessMHs[i][0].invoke(), defaultTrianglePositions);
				Object triNew = createTriangle2D(defaultTrianglePositionsNew);
				fieldAccessMHs[i][1].invoke(triNew);
				checkEqualTriangle2D(fieldAccessMHs[i][0].invoke(), defaultTrianglePositionsNew);
				break;
			case "QValueInt;":
				assertEquals(getInt.invoke(fieldAccessMHs[i][0].invoke()), defaultInt);
				Object iNew = makeValueInt.invoke(defaultIntNew);
				fieldAccessMHs[i][1].invoke(iNew);
				assertEquals(getInt.invoke(fieldAccessMHs[i][0].invoke()), defaultIntNew);
				break;
			case "QValueFloat;":
				assertEquals(getFloat.invoke(fieldAccessMHs[i][0].invoke()), defaultFloat);
				Object fNew = makeValueFloat.invoke(defaultFloatNew);
				fieldAccessMHs[i][1].invoke(fNew);
				assertEquals(getFloat.invoke(fieldAccessMHs[i][0].invoke()), defaultFloatNew);
				break;
			case "QValueDouble;":
				assertEquals(getDouble.invoke(fieldAccessMHs[i][0].invoke()), defaultDouble);
				Object dNew = makeValueDouble.invoke(defaultDoubleNew);
				fieldAccessMHs[i][1].invoke(dNew);
				assertEquals(getDouble.invoke(fieldAccessMHs[i][0].invoke()), defaultDoubleNew);
				break;
			case "QValueObject;":
				assertEquals(getObject.invoke(fieldAccessMHs[i][0].invoke()), defaultObject);
				Object oNew = makeValueObject.invoke(defaultObjectNew);
				fieldAccessMHs[i][1].invoke(oNew);
				assertEquals(getObject.invoke(fieldAccessMHs[i][0].invoke()), defaultObjectNew);
				break;
			case "QValueLong;":
				assertEquals(getLong.invoke(fieldAccessMHs[i][0].invoke()), defaultLong);
				Object lNew = makeValueLong.invoke(defaultLongNew);
				fieldAccessMHs[i][1].invoke(lNew);
				assertEquals(getLong.invoke(fieldAccessMHs[i][0].invoke()), defaultLongNew);
				break;
			case "QLargeObject;":
				checkEqualLargeObject(fieldAccessMHs[i][0].invoke(), defaultObject);
				Object largeNew = createLargeObject(defaultObjectNew);
				fieldAccessMHs[i][1].invoke(largeNew);
				checkEqualLargeObject(fieldAccessMHs[i][0].invoke(), defaultObjectNew);
				break;
			default:
				break;
			}
		}
	}

	static void checkEqualPoint2D(Object point, int[] positions) throws Throwable {
		assertEquals(getX.invoke(point), positions[0]);
		assertEquals(getY.invoke(point), positions[1]);
	}

	static void checkEqualFlattenedLine2D(Object line, int[][] positions) throws Throwable {
		checkEqualPoint2D(getFlatSt.invoke(line), positions[0]);
		checkEqualPoint2D(getFlatEn.invoke(line), positions[1]);
	}

	static void checkEqualTriangle2D(Object triangle, int[][][] positions) throws Throwable {
		checkEqualFlattenedLine2D(getV1.invoke(triangle), positions[0]);
		checkEqualFlattenedLine2D(getV2.invoke(triangle), positions[1]);
		checkEqualFlattenedLine2D(getV3.invoke(triangle), positions[2]);
	}

	static void checkEqualLargeObject(Object largeObject, Object contentObject) throws Throwable {
		for (int i = 0; i < 16; i++) {
			assertEquals(getObject.invoke(getObjects[i].invoke(largeObject)), contentObject);
		}
	}
	
	static void checkObject(Object ...objects) {
		com.ibm.jvm.Dump.SystemDump();
	}
}
