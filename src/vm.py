


FLAG = "N0_0n3_R3411y_Kn0w5_175_4_S3cr37"
BYTE_OFFSET = 0x42
INT_XOR     = 0x55

# Registers
RE_ACC = 1
RE_TMP = 2
RE_GP  = 3
RE_SP  = 4
RE_UBP = 5

# Opcodes
OP_PUSH     = 0x01 # Push Imm
OP_POP      = 0x02 # Pop -> ACC
OP_XOR      = 0x03 # Stack Top ^ Imm
OP_CMP      = 0x04 # CMP ACC, Imm
OP_INC      = 0x05 # INC Reg
OP_ASSERT   = 0x06 # Assert ZF==1
OP_SWAP     = 0x07 # Rotate Stack (Bottom -> Top)
OP_LOAD     = 0x08 # Stack Top -> Reg (No Pop)
OP_PRINT    = 0x09 # Print
OP_ROL      = 0x10 # Rotate ACC (Arg: bit 3=Right/Left, 0-2=Amount)
OP_PUI      = 0x11 # UI[UBP] -> Stack, UBP++
OP_MOV_R    = 0x12 # ACC -> Reg
OP_PUSH_R   = 0x13 # Reg -> Stack
OP_XOR_R    = 0x14 # Stack Top ^ Reg
OP_PUSHU    = 0x31 # Imm -> UI[USP], USP++

code = []

def rol8(v, s):
    s = s & 7
    return ((v << s) & 0xFF) | (v >> (8 - s))

def ror8(v, s):
    s = s & 7
    return ((v >> s) | (v << (8 - s))) & 0xFF

def poison_byte(val): 
    return (ord(val) + BYTE_OFFSET) & 0xFF

def emit_byte(val):
    if val <= 127:
        return val ^ INT_XOR
    else:
        return val

def emit(op, arg=0):
    code.append(emit_byte(op))
    code.append(emit_byte(arg))


MAGIC_BYTE = 0xAA
emit(OP_PUSHU, MAGIC_BYTE) 

for i, char in enumerate(FLAG):
    expected = poison_byte(char)
    

    if i < 4:
        emit(OP_PUI, 0)          # Stack: [Enc(Char)]
        emit(OP_LOAD, RE_ACC)    # ACC = Enc(Char)
        emit(OP_CMP, expected)   # CMP ACC, Expected
        emit(OP_ASSERT, 0)       # Die if false
        emit(OP_POP, 0)          # Clear Stack

    elif i < 8:
        # Logic: Enc(Char) ^ 0x1F == Expected ^ 0x1F
        xor_key = 0x1F
        target = expected ^ xor_key
        
        emit(OP_PUI, 0)          # Stack: [Enc(Char)]
        emit(OP_XOR, xor_key)    # Stack: [Enc(Char) ^ 0x1F]
        emit(OP_POP, 0)          # ACC = Stack.Pop()
        emit(OP_CMP, target)
        emit(OP_ASSERT, 0)

    elif i < 12:
        # We alternate between ROL and ROR to verify opcode usage
        emit(OP_PUI, 0)
        emit(OP_LOAD, RE_ACC)
        
        if i % 2 == 0:
            # Test ROL (Bit 3 is 0)
            shift = 3
            emit(OP_ROL, shift) 
            target = rol8(expected, shift)
        else:
            # Test ROR (Bit 3 is 1 -> 8 | shift)
            shift = 2
            emit(OP_ROL, 8 | shift) # 8 sets the ROR flag in the VM
            target = ror8(expected, shift)
            
        emit(OP_CMP, target)
        emit(OP_ASSERT, 0)
        emit(OP_POP, 0)

    elif i < 16:
        # Logic: verify (Enc(char) ^ GP) where GP is manipulated
        
        emit(OP_PUI, 0)          # Stack: [Enc(Char)]
        
        # Prepare Register GP
        emit(OP_PUSH, 0x10)      # Push initial value
        emit(OP_POP, 0)          # ACC = 0x10
        emit(OP_MOV_R, RE_GP)    # GP = 0x10
        emit(OP_INC, RE_GP)      # GP = 0x11
        
        # XOR Stack with GP
        emit(OP_XOR_R, RE_GP)    # Stack ^= GP
        
        target = expected ^ 0x11
        emit(OP_POP, 0)          # ACC = Result
        emit(OP_CMP, target)
        emit(OP_ASSERT, 0)

    else:
        emit(OP_PUI, 0)
        emit(OP_LOAD, RE_ACC)
        emit(OP_CMP, expected)
        emit(OP_ASSERT, 0)

remaining_stack_depth = len(FLAG) - 16
if remaining_stack_depth > 1:
    # Rotate the stack verify the item that bubbled up
    # The bottom item was FLAG[16]
    bottom_char_idx = 16
    expected_bottom = poison_byte(FLAG[bottom_char_idx])
    
    emit(OP_SWAP, 0)         # Bottom moves to Top
    emit(OP_LOAD, RE_ACC)    # Peek Top
    emit(OP_CMP, expected_bottom)
    emit(OP_ASSERT, 0)
    
    # Clear the stack
    for _ in range(remaining_stack_depth):
        emit(OP_POP, 0)
else:
    # Just clear if not enough items
    emit(OP_POP, 0)


emit(OP_PUI, 0)
emit(OP_LOAD, RE_ACC)
emit(OP_CMP, MAGIC_BYTE)
emit(OP_ASSERT, 0)
emit(OP_POP, 0)


# Prints "Correct! Flag: {Input}"
success_ops = [
    (OP_PUSH,0x24), (OP_XOR, 0x67), # C
    (OP_PUSH,0x8),  (OP_XOR, 0x67), # o
    (OP_PUSH,0x15), (OP_XOR, 0x67), # r
    (OP_PUSH,0x15), (OP_XOR, 0x67), # r
    (OP_PUSH,0x2),  (OP_XOR, 0x67), # e
    (OP_PUSH,0x4),  (OP_XOR, 0x67), # c
    (OP_PUSH,0x13), (OP_XOR, 0x67), # t
    (OP_PUSH,0x46), (OP_XOR, 0x67), # !
    (OP_PUSH,0x47), (OP_XOR, 0x67), # space
    (OP_PUSH,0x21), (OP_XOR, 0x67), # F
    (OP_PUSH,0xb),  (OP_XOR, 0x67), # l
    (OP_PUSH,0x6),  (OP_XOR, 0x67), # a
    (OP_PUSH,0x0),  (OP_XOR, 0x67), # g
    (OP_PUSH,0x5d), (OP_XOR, 0x67), # :
    (OP_PUSH,0x47), (OP_XOR, 0x67), # space
    (OP_PRINT, 0),  # Print Stack ("Correct! Flag: ")
    (OP_PRINT, 1),  # Print Decoded UI Buffer
]

for op, arg in success_ops:
    emit(op, arg)

print(f"\n// Total Bytes: {len(code)}")
print("unsigned char VM_bytecode[] = {")
line = "    "
for k, byte_val in enumerate(code):
    line += f"0x{byte_val:02X}, "
    if (k+1) % 12 == 0:
        print(line)
        line = "    "
print(line.rstrip(", "))
print("};")