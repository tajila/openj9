package org.openj9.test.lw1;

import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;

import jdk.internal.misc.Unsafe;

import static org.objectweb.asm.Opcodes.*;

import java.lang.reflect.Field;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.List;

public class ValueTypeGenerator {
	private static Unsafe unsafe;

	/* workaround till the new ASM is released */
	public static final int DEFAULTVALUE = 204;
	public static final int WITHFIELD = 203;
	private static final int ACC_VALUE_TYPE = 0x100;
	private static final int ACC_FLATTENABLE = 0x100;
	
	static {
		try {
			Field f = Unsafe.class.getDeclaredField("theUnsafe");
			f.setAccessible(true);
			unsafe = (Unsafe) f.get(null);
		} catch (NoSuchFieldException | SecurityException | IllegalArgumentException | IllegalAccessException e) {
			e.printStackTrace();
		}	
	}
	
	public static byte[] generateValue(String valueName, String[] fields) {
		ArrayList<String> valueTypesList = new ArrayList<String>();
		ClassWriter cw = new ClassWriter(0);
		FieldVisitor fv;
		MethodVisitor mv;

		cw.visit(V1_8, ACC_PUBLIC + ACC_FINAL + ACC_SUPER + ACC_VALUE_TYPE, valueName, null, "java/lang/Object", null);

		cw.visitSource(valueName + ".java", null);
		
		valueTypesList.add("L" + valueName + ";");

		String makeValueSig = "";
		for (String s : fields) {
			String nameAndSigValue[] = s.split(":");
			int fieldModifiers = ACC_PUBLIC + ACC_FINAL;
			if (nameAndSigValue.length > 2 && nameAndSigValue[2].equals("value")) {
				fieldModifiers += ACC_FLATTENABLE;
				valueTypesList.add(nameAndSigValue[1]);
			}
			fv = cw.visitField(fieldModifiers, nameAndSigValue[0], nameAndSigValue[1], null, null);
			fv.visitEnd();
			makeValueSig += nameAndSigValue[1];
			
			generateGetter(cw, nameAndSigValue, valueName);
			
			generateSetter(cw, nameAndSigValue, valueName);
			
			generateWither(cw, nameAndSigValue, valueName);
			
		}
		
		/* create valueTypes attribute */
		cw.visitAttribute(new ValueTypesAttribute(valueTypesList));
		cw.visitEnd();
		
		{
			boolean doubleDetected = false;
			mv = cw.visitMethod(ACC_PUBLIC  + ACC_STATIC, "makeValue", "(" + makeValueSig + ")L" + valueName + ";", null, null);
			mv.visitCode();
			mv.visitTypeInsn(DEFAULTVALUE, valueName);
			for (int i = 0; i <  fields.length; i++) {
				String nameAndSig[] = fields[i].split(":");
				switch (nameAndSig[1]) {
				case "D":
					doubleDetected = true;
					mv.visitVarInsn(DLOAD, i);
					break;
				case "I":
				case "Z":
				case "B":
				case "C":
				case "S":
					mv.visitVarInsn(ILOAD, i);
					break;
				case "F":
					mv.visitVarInsn(FLOAD, i);
					break;
				case "J":
					doubleDetected = true;
					mv.visitVarInsn(LLOAD, i);
					break;
				default:
					mv.visitVarInsn(ALOAD, i);
					break;
				}
				mv.visitFieldInsn(WITHFIELD, valueName, nameAndSig[0], nameAndSig[1]);
			}
			mv.visitInsn(ARETURN);
			
			int maxStackAndLocals = (doubleDetected) ? 3 : 2;
			mv.visitMaxs(maxStackAndLocals, maxStackAndLocals);
			mv.visitEnd();
		}

		cw.visitEnd();

		return cw.toByteArray();
	}
	
	public static byte[] generateRefObject(String className, String[] fields) {
		ArrayList<String> valueTypesList = new ArrayList<String>();
		ClassWriter cw = new ClassWriter(0);
		FieldVisitor fv;
		MethodVisitor mv;

		cw.visit(V1_8, ACC_PUBLIC + ACC_FINAL + ACC_SUPER, className, null, "java/lang/Object", null);

		cw.visitSource("Value.java", null);

		String makeValueSig = "";
		int makeMaxLocal = 0;
		for (String s : fields) {
			String nameAndSigValue[] = s.split(":");
			int fieldModifiers = ACC_PUBLIC;
			if (nameAndSigValue.length > 2 && nameAndSigValue[2].equals("value")) {
				fieldModifiers += ACC_FLATTENABLE;
				valueTypesList.add(nameAndSigValue[1]);
			}
			fv = cw.visitField(fieldModifiers, nameAndSigValue[0], nameAndSigValue[1], null, null);
			fv.visitEnd();
			makeValueSig += nameAndSigValue[1];
			if (nameAndSigValue[1].equals("J") || nameAndSigValue[1].equals("D")) {
				makeMaxLocal += 2;
			} else {
				makeMaxLocal += 1;
			}
			
			
			generateGetter(cw, nameAndSigValue, className);
			
			generateSetter(cw, nameAndSigValue, className);
		}
		
		/* create valueTypes attribute */
		cw.visitAttribute(new ValueTypesAttribute(valueTypesList));
		cw.visitEnd();
		
		{
			mv = cw.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
			mv.visitCode();
			mv.visitVarInsn(ALOAD, 0);
			mv.visitMethodInsn(INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
			mv.visitInsn(RETURN);
			mv.visitMaxs(1, 1);
			mv.visitEnd();
		}
		
		{
			boolean doubleDetected = false;
			
			mv = cw.visitMethod(ACC_PUBLIC  + ACC_STATIC, "makeRef", "(" + makeValueSig + ")L" + className + ";", null, null);
			mv.visitCode();
			mv.visitTypeInsn(NEW, className);
			mv.visitInsn(DUP);
			mv.visitMethodInsn(INVOKESPECIAL, className, "<init>", "()V", false);
			mv.visitVarInsn(ASTORE, fields.length);
			for (int i = 0; i <  fields.length; i++) {
				mv.visitVarInsn(ALOAD, fields.length);
				String nameAndSigValue[] = fields[i].split(":");
				switch (nameAndSigValue[1]) {
				case "D":
					doubleDetected = true;
					mv.visitVarInsn(DLOAD, i);
					break;
				case "I":
				case "Z":
				case "B":
				case "C":
				case "S":
					mv.visitVarInsn(ILOAD, i);
					break;
				case "F":
					mv.visitVarInsn(FLOAD, i);
					break;
				case "J":
					doubleDetected = true;
					mv.visitVarInsn(LLOAD, i);
					break;
				default:
					mv.visitVarInsn(ALOAD, i);
					break;
				}
				mv.visitFieldInsn(PUTFIELD, className, nameAndSigValue[0], nameAndSigValue[1]);
			}
			mv.visitVarInsn(ALOAD, fields.length);
			mv.visitInsn(ARETURN);
			
			int maxStack = (doubleDetected) ? 3 : 2;
			mv.visitMaxs(maxStack, makeMaxLocal + 1);
			mv.visitEnd();
		}

		cw.visitEnd();

		return cw.toByteArray();
	}

	private static void generateSetter(ClassWriter cw, String[] nameAndSigValue, String className) {
		/* generate setter */
		MethodVisitor mv = cw.visitMethod(ACC_PUBLIC, "set" + nameAndSigValue[0], "(" + nameAndSigValue[1] + ")V", null, null);
		mv.visitCode();
		mv.visitVarInsn(ALOAD, 0);
		switch (nameAndSigValue[1]) {
		case "D":
			mv.visitVarInsn(DLOAD, 1);
			break;
		case "I":
		case "Z":
		case "B":
		case "C":
		case "S":
			mv.visitVarInsn(ILOAD, 1);
			break;
		case "F":
			mv.visitVarInsn(FLOAD, 1);
			break;
		case "J":
			mv.visitVarInsn(LLOAD, 1);
			break;
		default:
			mv.visitVarInsn(ALOAD, 1);
			break;
		}
		mv.visitFieldInsn(PUTFIELD, className, nameAndSigValue[0], nameAndSigValue[1]);
		mv.visitInsn(RETURN);
		mv.visitMaxs(2, 2);
		mv.visitEnd();
	}
	
	private static void generateWither(ClassWriter cw, String[] nameAndSigValue, String className) {
		/* generate setter */
		MethodVisitor mv = cw.visitMethod(ACC_PUBLIC, "set" + nameAndSigValue[0], "(" + nameAndSigValue[1] + ")L" + className + ";", null, null);
		mv.visitCode();
		mv.visitVarInsn(ALOAD, 0);
		switch (nameAndSigValue[1]) {
		case "D":
			mv.visitVarInsn(DLOAD, 1);
			break;
		case "I":
		case "Z":
		case "B":
		case "C":
		case "S":
			mv.visitVarInsn(ILOAD, 1);
			break;
		case "F":
			mv.visitVarInsn(FLOAD, 1);
			break;
		case "J":
			mv.visitVarInsn(LLOAD, 1);
			break;
		default:
			mv.visitVarInsn(ALOAD, 1);
			break;
		}
		mv.visitFieldInsn(WITHFIELD, className, nameAndSigValue[0], nameAndSigValue[1]);
		mv.visitInsn(RETURN);
		mv.visitMaxs(2, 2);
		mv.visitEnd();
	}
	
	private static void generateGetter(ClassWriter cw, String[] nameAndSigValue, String className) {
		MethodVisitor mv = cw.visitMethod(ACC_PUBLIC, "get" + nameAndSigValue[0], "()" + nameAndSigValue[1], null, null);
		mv.visitCode();
		mv.visitVarInsn(ALOAD, 0);
		mv.visitFieldInsn(GETFIELD, className, nameAndSigValue[0], nameAndSigValue[1]);
		switch (nameAndSigValue[1]) {
		case "D":
			mv.visitInsn(DRETURN);
			break;
		case "I":
		case "Z":
		case "B":
		case "C":
		case "S":
			mv.visitInsn(IRETURN);
			break;
		case "F":
			mv.visitInsn(FRETURN);
			break;
		case "J":
			mv.visitInsn(LRETURN);
			break;
		default:
			mv.visitInsn(ARETURN);
			break;
		}
		mv.visitMaxs(1, 1);
		mv.visitEnd();
	}
	
	static public Class<?> defineClass(String className, byte[] bytes, ClassLoader loader, ProtectionDomain pD) throws Throwable {
		Class<?> newClass = unsafe.defineClass(className, bytes, 0, bytes.length, loader, pD);
		return newClass;
	}
}
