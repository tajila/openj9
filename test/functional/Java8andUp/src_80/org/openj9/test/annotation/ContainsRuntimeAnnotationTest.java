package org.openj9.test.annotation;

/*******************************************************************************
 * Copyright (c) 2021, 2021 IBM Corp. and others
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

import org.testng.Assert;
import org.testng.AssertJUnit;
import org.testng.annotations.Test;
import org.testng.log4testng.Logger;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.annotation.Annotation;
import java.lang.annotation.ElementType;

import java.util.Arrays;

import sun.misc.SharedSecrets;
import sun.reflect.ConstantPool;

@Test(groups = { "level.sanity" })
public class ContainsRuntimeAnnotationTest {
	private static final String THIS_CLASS_NAME = "org/openj9/test/annotation/ContainsRuntimeAnnotationTest";
	private final ConstantPool thisClassConstantPool;

	static {
		try {
			System.loadLibrary("anntests");
		} catch (UnsatisfiedLinkError e) {
			Assert.fail(e.getMessage() + "\nlibrary path = " + System.getProperty("java.library.path"));
		}
	}

	@MyFieldAnnotation
	int myField = 0;

	@MyMethodAnnotation
	void myMethod() {}
	
	int myField2 = 0;
	
	@MyFieldAnnotation
	@MyFieldAnnotation2
	@Stable
	@MyFieldAnnotation3
	@MyFieldAnnotation4
	int myField3 = 0;

	void myMethod2() {}

	public ContainsRuntimeAnnotationTest() {
		thisClassConstantPool = SharedSecrets.getJavaLangAccess().getConstantPool(this.getClass());
	}

	private static native boolean containsRuntimeAnnotation(Class clazz, int cpIndex, String annotationName, boolean isField, boolean isStatic);

	@Test
	public void test_field_annotation() throws Exception {
		boolean annotationFound = false;
		
		/* resolve fields */
		int field1 = myField;
		int field2 = myField2;
		int field3 = myField3;
		
		int cpIndex = getMemberCPIndex(thisClassConstantPool, THIS_CLASS_NAME, "myField", "I", true);
		Assert.assertTrue(-1 != cpIndex, "could fine myField");
		final String annotationName = "Lorg/openj9/test/annotation/MyFieldAnnotation;";
		final String stableAnnotation = "Lorg/openj9/test/annotation/MyStableAnnotation;";
		annotationFound = containsRuntimeAnnotation(ContainsRuntimeAnnotationTest.class, cpIndex, annotationName, true, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(thisClassConstantPool, THIS_CLASS_NAME, "myField2", "I", true);
		Assert.assertTrue(-1 != cpIndex, "could fine myField2");
		
		annotationFound = containsRuntimeAnnotation(ContainsRuntimeAnnotationTest.class, cpIndex, annotationName, true, false);
		Assert.assertFalse(annotationFound, "detected non-existing annatation");
		
		cpIndex = getMemberCPIndex(thisClassConstantPool, THIS_CLASS_NAME, "myField3", "I", true);
		Assert.assertTrue(-1 != cpIndex, "could fine myField3");
		
		annotationFound = containsRuntimeAnnotation(ContainsRuntimeAnnotationTest.class, cpIndex, stableAnnotation, true, false);
		Assert.assertTrue(annotationFound, "didn't find @stable");
	}

	@Test
	public void test_method_annotation() throws Exception {
		boolean annotationFound = false;

		/* resolve methods */
		myMethod();
		myMethod2();

		int cpIndex = getMemberCPIndex(thisClassConstantPool, THIS_CLASS_NAME, "myMethod", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method myMethod()V");
		
		final String annotationName = "Lorg/openj9/test/annotation/MyMethodAnnotation;";
		annotationFound = containsRuntimeAnnotation(ContainsRuntimeAnnotationTest.class, cpIndex, annotationName, false, false);
		Assert.assertTrue(annotationFound, "didnt detect annotation");
		
		cpIndex = getMemberCPIndex(thisClassConstantPool, THIS_CLASS_NAME, "myMethod2", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method myMethod2()V");
		annotationFound = containsRuntimeAnnotation(ContainsRuntimeAnnotationTest.class, cpIndex, annotationName, false, false);
		Assert.assertFalse(annotationFound, "detected non-existing annatation");
	}

	@Test
	public void test_field_in_external_class() throws Exception {
		boolean annotationFound = false;
		
		/* resolve fields */
		C c = new C();
		c.method1(new B());
		
		ConstantPool constantPool = SharedSecrets.getJavaLangAccess().getConstantPool(C.class);
		final String cClassName = "org/openj9/test/annotation/C";
		final String bClassName = "org/openj9/test/annotation/B";
		
		int cpIndex = getMemberCPIndex(constantPool, bClassName, "field1", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find field1 fref");
		
		final String annotationName = "Lorg/openj9/test/annotation/MyFieldAnnotation;";
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, bClassName, "field2", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find field2 fref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, cClassName, "field3", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find field3 fref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
	}
	
	@Test
	public void test_static_field_in_external_class() throws Exception {
		boolean annotationFound = false;
		
		/* resolve fields */
		C.sMethod1();
		
		ConstantPool constantPool = SharedSecrets.getJavaLangAccess().getConstantPool(C.class);
		final String cClassName = "org/openj9/test/annotation/C";
		final String bClassName = "org/openj9/test/annotation/B";
		
		int cpIndex = getMemberCPIndex(constantPool, bClassName, "sfield1", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find sfield1 fref");
		
		final String annotationName = "Lorg/openj9/test/annotation/MyFieldAnnotation;";
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, bClassName, "sfield2", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find sfield2 fref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, cClassName, "sfield3", "I", true);
		Assert.assertTrue(-1 != cpIndex, "couldn't find sfield3 fref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, true, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
	}
	
	@Test
	public void test_method_in_external_class() throws Exception {
		boolean annotationFound = false;
		
		/* resolve methods */
		C c = new C();
		c.method4(new B());
		
		ConstantPool constantPool = SharedSecrets.getJavaLangAccess().getConstantPool(C.class);
		final String cClassName = "org/openj9/test/annotation/C";
		final String bClassName = "org/openj9/test/annotation/B";
		
		int cpIndex = getMemberCPIndex(constantPool, bClassName, "method1", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method1 mref");
		
		final String annotationName = "Lorg/openj9/test/annotation/MyMethodAnnotation;";
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, bClassName, "method2", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method2 mref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, cClassName, "method3", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method3 mref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, false);
		Assert.assertTrue(annotationFound, "did not detect annotation");
	}
	
	@Test
	public void test_static_method_in_external_class() throws Exception {
		boolean annotationFound = false;
		
		/* resolve methods */
		C.sMethod4();
		
		ConstantPool constantPool = SharedSecrets.getJavaLangAccess().getConstantPool(C.class);
		final String cClassName = "org/openj9/test/annotation/C";
		final String bClassName = "org/openj9/test/annotation/B";
		
		int cpIndex = getMemberCPIndex(constantPool, bClassName, "sMethod1", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method1 mref");
		
		final String annotationName = "Lorg/openj9/test/annotation/MyMethodAnnotation;";
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, bClassName, "sMethod2", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method2 mref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
		
		cpIndex = getMemberCPIndex(constantPool, cClassName, "sMethod3", "()V", false);
		Assert.assertTrue(-1 != cpIndex, "couldn't find method3 mref");
		
		annotationFound = containsRuntimeAnnotation(C.class, cpIndex, annotationName, false, true);
		Assert.assertTrue(annotationFound, "did not detect annotation");
	}
	
	private int getMemberCPIndex(ConstantPool constantPool, String className, String memberName, String memberType, boolean isField) {
		int cpIndex = -1;
		int size = constantPool.getSize();
		Assert.assertTrue(size > 1, "error with constantPool");
		
		for (int i = size - 1; i > 0; i--) {
			try {
				/* Returns 3-element array of class name, member name and type */
				String [] cpMemberInfo = constantPool.getMemberRefInfoAt(i);
				if (className.equals(cpMemberInfo[0])
					&& memberName.equals(cpMemberInfo[1])
					&& memberType.equals(cpMemberInfo[2])
				) {
					cpIndex = i;
					break;
				}
			} catch (Throwable ignored) {
				/* Ignore errors if the constant pool entry doesn't exist */
			}
		}

		return cpIndex;
	}
}
class A {
	A() {}
	
	@MyFieldAnnotation
	int field1;
	
	@MyFieldAnnotation
	static int sfield1;
	
	@MyMethodAnnotation
	void method1() {}
	
	@MyMethodAnnotation
	static void sMethod1() {}
}

class B extends A {
	B() {}
	
	@MyFieldAnnotation
	int field2;
	
	@MyFieldAnnotation
	static int sfield2;
	
	@MyMethodAnnotation
	void method2() {}
	
	@MyMethodAnnotation
	static void sMethod2() {}
}

class C {
	@MyFieldAnnotation
	int field3;
	
	@MyFieldAnnotation
	static int sfield3;
	
	@MyMethodAnnotation
	void method3() {}
	
	@MyMethodAnnotation
	static void sMethod3() {}
	
	C() {}
	
	void method1(B b) {
		int field1 = b.field1;
		int field2 = b.field2;
		int field3 = this.field3;
	}
	
	static void sMethod1() {
		int field1 = B.sfield1;
		int field2 = B.sfield2;
		int field3 = C.sfield3;
	}
	
	static void sMethod4() {
		B.sMethod1();
		B.sMethod2();
		C.sMethod3();
	}
	
	void method4(B b) {
		b.method1();
		b.method2();
		this.method3();
	}
}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@interface MyFieldAnnotation {}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@interface MyFieldAnnotation2 {}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@interface MyFieldAnnotation3 {}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@interface MyFieldAnnotation4 {}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@interface MyMethodAnnotation {}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@interface MyStableAnnotation {}

