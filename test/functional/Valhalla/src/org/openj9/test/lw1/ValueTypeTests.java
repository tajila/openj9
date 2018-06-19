package org.openj9.test.lw1;

import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.invoke.MethodType;
import java.lang.reflect.Method;
import org.testng.Assert;
import static org.testng.Assert.*;
import org.testng.annotations.Test;

@Test(groups = { "level.sanity" })
public class ValueTypeTests {

	static Lookup lookup = MethodHandles.lookup();
	static Class point2DClass = null;
	static Method makePoint2D = null;
	static Class line2DRefClass = null;
	static Method makeLine2D = null;
	static MethodHandle getX = null;
	static MethodHandle getY = null;
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
		point2DClass = ValueTypeGenerator.defineClass("Point2D", ValueTypeGenerator.generateValue("Point2D", fields), ValueTypeTests.class.getClassLoader(), ValueTypeTests.class.getProtectionDomain());
		makePoint2D = point2DClass.getDeclaredMethod("makeValue", new Class[] {int.class, int.class});
		
		getX = generateGetter(point2DClass, "x", int.class);
		MethodHandle setX = generateSetter(point2DClass, "x", int.class);
		getY = generateGetter(point2DClass, "y", int.class);
		MethodHandle setY = generateSetter(point2DClass, "y", int.class);
		
		Object point2D = makePoint2D.invoke(null, 2, 4);
		
		assertEquals(2, getX.invoke(point2D));
		assertEquals(4, getY.invoke(point2D));
		
		setX.invoke(point2D, 1);
		setY.invoke(point2D, 3);
		
		assertEquals(1, getX.invoke(point2D));
		assertEquals(3, getY.invoke(point2D));
	}
	
	/*
	 * Test with nested values
	 * 
	 * class Line2DRef {
	 * 	Point2D st;
	 * 	Point2D en;
	 * }
	 * 
	 */
	@Test(priority=2)
	static public void testCreateLine2D() throws Throwable {
		String fields[] = {"st:LPoint2D;:value", "en:LPoint2D;:value"};
		line2DRefClass = ValueTypeGenerator.defineClass("Line2DRef", ValueTypeGenerator.generateRefObject("Line2DRef", fields), ValueTypeTests.class.getClassLoader(), ValueTypeTests.class.getProtectionDomain());
		makeLine2D = line2DRefClass.getDeclaredMethod("makeRef", new Class[] {point2DClass, point2DClass});
		MethodHandle getSt = generateGetter(line2DRefClass, "st", point2DClass);
		MethodHandle setSt = generateSetter(line2DRefClass, "st", point2DClass);
		MethodHandle getEn = generateGetter(line2DRefClass, "en", point2DClass);
		MethodHandle setEn = generateSetter(line2DRefClass, "en", point2DClass);
		
		Object st = makePoint2D.invoke(null, 1 ,1);
		Object en = makePoint2D.invoke(null, 4 ,4);
		
		Object line2D = makeLine2D.invoke(null, st, en);
				
		assertEquals(getX.invoke(st), getX.invoke(getSt.invoke(line2D)));
		assertEquals(getY.invoke(st), getY.invoke(getSt.invoke(line2D)));
		assertEquals(getX.invoke(en), getX.invoke(getEn.invoke(line2D)));
		assertEquals(getY.invoke(en), getY.invoke(getEn.invoke(line2D)));
		
		Object st2 = makePoint2D.invoke(null, 2 ,2);
		Object en2 = makePoint2D.invoke(null, 3 ,3);
		
		setSt.invoke(line2D, st2);
		setEn.invoke(line2D, en2);
		
		assertEquals(getX.invoke(st2), getX.invoke(getSt.invoke(line2D)));
		assertEquals(getY.invoke(st2), getY.invoke(getSt.invoke(line2D)));
		assertEquals(getX.invoke(en2), getX.invoke(getEn.invoke(line2D)));
		assertEquals(getY.invoke(en2), getY.invoke(getEn.invoke(line2D)));
		
	}
	
	private static void checkValues(Object o, Object o2) {
		com.ibm.jvm.Dump.SystemDump();		
	}
	
	private static void checkValues(Object o) {
		com.ibm.jvm.Dump.SystemDump();		
	}
	static MethodHandle generateGetter(Class<?> clazz, String fieldName, Class<?> fieldType) {
		try {
			return lookup.findVirtual(clazz, "get"+fieldName, MethodType.methodType(fieldType));
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

}
