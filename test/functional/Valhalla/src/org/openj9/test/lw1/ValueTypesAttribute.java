package org.openj9.test.lw1;

import java.util.List;

import org.objectweb.asm.Attribute;
import org.objectweb.asm.ByteVector;
import org.objectweb.asm.ClassWriter;

public class ValueTypesAttribute extends Attribute {
	private List<String> valueTypes;
	
	protected ValueTypesAttribute(List<String> valueTypes) {
		super("ValueTypes");
		this.valueTypes = valueTypes;
	}
	
	public boolean isCodeAttribute(){
		return false;
	}
	
	public boolean isUnknown() {
		return false;
	}
	
	public ByteVector write(ClassWriter cw, byte[] code, 	int len, int maxStack, int maxLocals) {

		ByteVector vector = new ByteVector();
		
		vector.putShort(valueTypes.size());
		
		int nestMemberIndex;
		for (String value : valueTypes) {
			nestMemberIndex = cw.newClass(value);
			vector.putShort(nestMemberIndex);
		}
		
		return vector;
	}

}
