#include <stdio.h>
#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "verify_class.h"
#include <string.h>


#define DEBUG_VM 0

static uint8_t byteArray[] = {
    0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41
};
static uint8_t integerArray[] = {
    0x55,0x54,0x57,0x56,0x51,0x50,0x53,0x52,0x5d,0x5c,0x5f,0x5e,0x59,0x58,0x5b,0x5a,0x45,0x44,0x47,0x46,0x41,0x40,0x43,0x42,0x4d,0x4c,0x4f,0x4e,0x49,0x48,0x4b,0x4a,0x75,0x74,0x77,0x76,0x71,0x70,0x73,0x72,0x7d,0x7c,0x7f,0x7e,0x79,0x78,0x7b,0x7a,0x65,0x64,0x67,0x66,0x61,0x60,0x63,0x62,0x6d,0x6c,0x6f,0x6e,0x69,0x68,0x6b,0x6a,0x15,0x14,0x17,0x16,0x11,0x10,0x13,0x12,0x1d,0x1c,0x1f,0x1e,0x19,0x18,0x1b,0x1a,0x5,0x4,0x7,0x6,0x1,0x0,0x3,0x2,0xd,0xc,0xf,0xe,0x9,0x8,0xb,0xa,0x35,0x34,0x37,0x36,0x31,0x30,0x33,0x32,0x3d,0x3c,0x3f,0x3e,0x39,0x38,0x3b,0x3a,0x25,0x24,0x27,0x26,0x21,0x20,0x23,0x22,0x2d,0x2c,0x2f,0x2e,0x29,0x28,0x2b,0x2a,0xd5,0xd4,0xd7,0xd6,0xd1,0xd0,0xd3,0xd2,0xdd,0xdc,0xdf,0xde,0xd9,0xd8,0xdb,0xda,0xc5,0xc4,0xc7,0xc6,0xc1,0xc0,0xc3,0xc2,0xcd,0xcc,0xcf,0xce,0xc9,0xc8,0xcb,0xca,0xf5,0xf4,0xf7,0xf6,0xf1,0xf0,0xf3,0xf2,0xfd,0xfc,0xff,0xfe,0xf9,0xf8,0xfb,0xfa,0xe5,0xe4,0xe7,0xe6,0xe1,0xe0,0xe3,0xe2,0xed,0xec,0xef,0xee,0xe9,0xe8,0xeb,0xea,0x95,0x94,0x97,0x96,0x91,0x90,0x93,0x92,0x9d,0x9c,0x9f,0x9e,0x99,0x98,0x9b,0x9a,0x85,0x84,0x87,0x86,0x81,0x80,0x83,0x82,0x8d,0x8c,0x8f,0x8e,0x89,0x88,0x8b,0x8a,0xb5,0xb4,0xb7,0xb6,0xb1,0xb0,0xb3,0xb2,0xbd,0xbc,0xbf,0xbe,0xb9,0xb8,0xbb,0xba,0xa5,0xa4,0xa7,0xa6,0xa1,0xa0,0xa3,0xa2,0xad,0xac,0xaf,0xae,0xa9,0xa8,0xab,0xaa
};
static unsigned char VM_bytecode[] = {
    0x64, 0xAA, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x90, 0x53, 0x55, 0x57, 0x55, 
    0x44, 0x55, 0x5D, 0x54, 0x51, 0x27, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0xA1, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 0x5D, 0x54, 
    0x51, 0x27, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 0x56, 0x4A, 0x57, 0x55, 
    0x51, 0xAF, 0x53, 0x55, 0x44, 0x55, 0x56, 0x4A, 0x57, 0x55, 0x51, 0x3F, 
    0x53, 0x55, 0x44, 0x55, 0x56, 0x4A, 0x57, 0x55, 0x51, 0xBE, 0x53, 0x55, 
    0x44, 0x55, 0x56, 0x4A, 0x57, 0x55, 0x51, 0x8B, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x45, 0x56, 0x51, 0xAB, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x45, 0x5F, 0x51, 0x9D, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x45, 0x56, 0x51, 0x9B, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x45, 0x5F, 0x51, 0xDC, 0x53, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x54, 0x45, 0x57, 0x55, 0x47, 0x56, 0x50, 0x56, 0x41, 0x56, 0x57, 0x55, 
    0x51, 0xAA, 0x53, 0x55, 0x44, 0x55, 0x54, 0x45, 0x57, 0x55, 0x47, 0x56, 
    0x50, 0x56, 0x41, 0x56, 0x57, 0x55, 0x51, 0xB0, 0x53, 0x55, 0x44, 0x55, 
    0x54, 0x45, 0x57, 0x55, 0x47, 0x56, 0x50, 0x56, 0x41, 0x56, 0x57, 0x55, 
    0x51, 0x9C, 0x53, 0x55, 0x44, 0x55, 0x54, 0x45, 0x57, 0x55, 0x47, 0x56, 
    0x50, 0x56, 0x41, 0x56, 0x57, 0x55, 0x51, 0xA1, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0x27, 0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0xB9, 
    0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x22, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0xA1, 0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x26, 
    0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x2C, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0x22, 0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0xA1, 
    0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x23, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0xA1, 0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x95, 
    0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x20, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0xA5, 0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0xB4, 
    0x53, 0x55, 0x44, 0x55, 0x5D, 0x54, 0x51, 0x20, 0x53, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0x2C, 0x53, 0x55, 0x52, 0x55, 0x5D, 0x54, 0x51, 0x27, 
    0x53, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 
    0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 
    0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x57, 0x55, 0x44, 0x55, 
    0x5D, 0x54, 0x51, 0xAA, 0x53, 0x55, 0x57, 0x55, 0x54, 0x71, 0x56, 0x32, 
    0x54, 0x5D, 0x56, 0x32, 0x54, 0x40, 0x56, 0x32, 0x54, 0x40, 0x56, 0x32, 
    0x54, 0x57, 0x56, 0x32, 0x54, 0x51, 0x56, 0x32, 0x54, 0x46, 0x56, 0x32, 
    0x54, 0x13, 0x56, 0x32, 0x54, 0x12, 0x56, 0x32, 0x54, 0x74, 0x56, 0x32, 
    0x54, 0x5E, 0x56, 0x32, 0x54, 0x53, 0x56, 0x32, 0x54, 0x55, 0x56, 0x32, 
    0x54, 0x08, 0x56, 0x32, 0x54, 0x12, 0x56, 0x32, 0x5C, 0x55, 0x5C, 0x54
};

typedef struct {
    uint8_t stack[256];
    uint8_t  UI[64]; // User Input
    uint8_t ACC;      // ACC (accumulator)
    uint8_t TMP;       // temp
    uint8_t GP;        // General purpuse
    uint8_t ZF;        // Zero flag
    uint8_t SP;      // SP (stack pointer)
    uint8_t USP; //User SP (stack pointer)
    uint8_t UBP;  // User BP (base pointer)

    uint8_t pc;           // program counter
    uint8_t running;      // execution flag
} VM;

static VM vm = {
    .SP = 0,
    .UBP = 0,
    .USP = 0,
    .pc = 0,
    .running = 1
};

void javaPrintln(JNIEnv *env, const char *string) {
    jclass clsSystem = (*env)->FindClass(env, "java/lang/System");
    jfieldID fidOut = (*env)->GetStaticFieldID(
        env,
        clsSystem,
        "out",
        "Ljava/io/PrintStream;"
    );
    jobject out = (*env)->GetStaticObjectField(env, clsSystem, fidOut);
    jclass clsPrintStream = (*env)->FindClass(env, "java/io/PrintStream");


    jmethodID midPrintln = (*env)->GetMethodID(
        env,
        clsPrintStream,
        "println",
        "(Ljava/lang/String;)V"
    );

    jstring jstr = (*env)->NewStringUTF(env, string);

    (*env)->CallVoidMethod(env, out, midPrintln, jstr);
    return;
}
static inline uint8_t rol8(uint8_t v, uint8_t s) {
    s &= 7;
    return (v << s) | (v >> (8 - s));
}

static inline uint8_t ror8(uint8_t v, uint8_t s) {
    s &= 7;
    return (v >> s) | (v << (8 - s));
}
uint8_t *getRegisterValue(int arg) {
    if (arg == 1) return &vm.ACC;
    else if (arg == 2)return &vm.TMP;
    else if (arg == 3)return &vm.GP;
    else if (arg == 4)return &vm.SP;
    else if (arg == 5)return &vm.UBP;
    else {
        vm.running = 0;
        exit(EXIT_FAILURE);
    }
}
void vm_exec(uint8_t op, uint8_t arg) {
    switch (op) {

        case 0x01: // PUSH imm
            #if DEBUG_VM
                printf("[PUSH] arg=%u -> stack[%u]\n", arg, vm.SP);
            #endif
            vm.stack[vm.SP++] = arg;
            break;

        case 0x02: // POP -> ACC
            vm.ACC = vm.stack[--vm.SP];
            #if DEBUG_VM
                printf("[POP] ACC=%u, SP=%u\n", vm.ACC, vm.SP);
            #endif
            break;

        case 0x03: // XOR imm
            #if DEBUG_VM
                printf("[XOR imm] before=%u, arg=%u\n", vm.stack[vm.SP-1], arg);
            #endif
            vm.stack[vm.SP-1] ^= arg;
            #if DEBUG_VM
                printf("[XOR imm] after=%u\n", vm.stack[vm.SP-1]);
            #endif
            break;

        case 0x04: // CMP ACC, imm
            vm.ZF = (vm.ACC == arg);
            #if DEBUG_VM
                printf("[CMP] ACC=%u, ARG=%u, ZF=%u\n", vm.ACC, arg, vm.ZF);
            #endif
            break;

        case 0x05: // INC reg
            #if DEBUG_VM
                printf("[INC] reg=%u before=%u\n", arg, *getRegisterValue(arg));
            #endif
            (*getRegisterValue(arg))++;
            #if DEBUG_VM
                printf("[INC] reg=%u after=%u\n", arg, *getRegisterValue(arg));
            #endif
            break;

        case 0x06: // ASSERT ZF
            #if DEBUG_VM
                printf("[ASSERT] ZF=%u\n", vm.ZF);
            #endif
            if (!vm.ZF) {
                #if DEBUG_VM
                    printf("[ASSERT FAILED] halting VM\n");
                #endif
                vm.running = 0;
                exit(0);
            }
            break;

        case 0x07: // SWAP stack
            #if DEBUG_VM
                printf("[SWAP] SP=%u\n", vm.SP);
            #endif
            {
                uint8_t first = vm.stack[0];
                for (int i = 0; i < vm.SP - 1; i++)
                    vm.stack[i] = vm.stack[i + 1];
                vm.stack[vm.SP - 1] = first;
            }
            break;

        case 0x08: // LOAD top -> Register
            (*getRegisterValue(arg)) = vm.stack[vm.SP - 1];
            #if DEBUG_VM
                printf("[LOAD] ACC=%u\n", vm.ACC);
            #endif
            break;

        case 0x09: // PRINT stack
            #if DEBUG_VM
                printf("[PRINT] SP=%u | ", vm.SP);
            #endif
            if (arg == 0) {
                for (int i = 0; i < vm.SP; i++)
                    putchar(vm.stack[i]);
            }
            else if (arg == 1) {
                for (int i=0; i<vm.UBP-1 ; i++)
                    putchar(vm.UI[i]-0x42);
                putchar('\n');
            }
            break;

        case 0x10: // ROL / ROR ACC
            if ((arg & 0x8) == 0) {
                #if DEBUG_VM
                    printf("[ROL] ACC=%u by %u\n", vm.ACC, arg & 7);
                #endif
                vm.ACC = rol8(vm.ACC, arg & 7);
            } else {
                #if DEBUG_VM
                    printf("[ROR] ACC=%u by %u\n", vm.ACC, arg & 7);
                #endif
                vm.ACC = ror8(vm.ACC, arg & 7);
            }
            #if DEBUG_VM
                printf("[SHIFT RESULT] ACC=%u\n", vm.ACC);
            #endif
            break;

        case 0x11: // POP_UI -> PUSH_STACK
            #if DEBUG_VM
                printf("[PUI] UI[%u]=%u -> stack[%u]\n",
                    vm.UBP, vm.UI[vm.UBP], vm.SP);
            #endif
            vm.stack[vm.SP++] = vm.UI[vm.UBP++];
            break;

        case 0x12: // MOV ACC -> REGISTER
            #if DEBUG_VM
                printf("[MOV] ACC=%u -> reg=%u\n", vm.ACC, arg);
            #endif
            (*getRegisterValue(arg)) = vm.ACC;
            break;

        case 0x13: // PUSH REGISTER
            #if DEBUG_VM
                printf("[PUSHR] reg=%u val=%u -> stack[%u]\n",
                    arg, *getRegisterValue(arg), vm.SP);
            #endif
            vm.stack[vm.SP++] = (*getRegisterValue(arg));
            break;

        case 0x14: // XOR REGISTER
            #if DEBUG_VM
                printf("[XORR] stack_top=%u ^ reg(%u)=%u\n",
                    vm.stack[vm.SP-1], arg, *getRegisterValue(arg));
            #endif
            vm.stack[vm.SP-1] ^= (*getRegisterValue(arg));
            #if DEBUG_VM
                printf("[XORR RESULT] stack_top=%u\n", vm.stack[vm.SP-1]);
            #endif
            break;

        case 0x31: // PUSH UI
            #if DEBUG_VM
                printf("[PUSHU] UI[%u]=%u\n", vm.USP, arg);
            #endif
            vm.UI[vm.USP++] = arg;
            break;

        default:
            #if DEBUG_VM
                printf("[UNKNOWN OPCODE] 0x%02X arg=%u\n", op, arg);
            #endif
            vm.running = 0;
            break;
    }
}
void hijackCacheArrays(JNIEnv *env) {
    jclass clsInteger = (*env)->FindClass(env, "java/lang/Integer");
    if (clsInteger == NULL) {
        fprintf(stderr, "Failed to find Integer class\n");
        return;
    }
    jmethodID midInteger = (*env)->GetStaticMethodID(env, clsInteger, "valueOf", "(I)Ljava/lang/Integer;");
    jfieldID fidInteger = (*env)->GetFieldID(env, clsInteger, "value", "I");

    jclass clsByte = (*env)->FindClass(env, "java/lang/Byte");
    if (clsByte == NULL) {
        fprintf(stderr, "Failed to find Byte class\n");
        return;
    }
    jmethodID midByte = (*env)->GetStaticMethodID(env, clsByte, "valueOf", "(B)Ljava/lang/Byte;");
    jfieldID fidByte = (*env)->GetFieldID(env, clsByte, "value", "B");
    
    for (int i = 0; i < 256; i++) {
        jobject objInteger = (*env)->CallStaticObjectMethod(env, clsInteger, midInteger, (jint)i);
        if (objInteger != NULL) {
            (*env)->SetIntField(env, objInteger, fidInteger, integerArray[i]);
        }
        jbyte signedByte = (jbyte)i; 
        
        jobject objByte = (*env)->CallStaticObjectMethod(env, clsByte, midByte, signedByte);
        if (objByte != NULL) {
            (*env)->SetByteField(env, objByte, fidByte, byteArray[i]);
        }
    }
}
JNIEXPORT jobject JNICALL hijacked_intern(JNIEnv *env, jobject thisStr) {
    if (!vm.running) {
        return thisStr;
    }
    
    // Get length to ensure we don't read out of bounds
    jsize len = (*env)->GetStringLength(env, thisStr);

    // Get pointer to raw UTF-16 characters
    const jchar *chars = (*env)->GetStringChars(env, thisStr, NULL);
    if (chars == NULL) {
        return thisStr;
    }

    if (len >= 2) {
        // Cast jchar (uint16_t) to uint8_t directly
        uint8_t opcode = (uint8_t)chars[0];
        uint8_t operand = (uint8_t)chars[1];
        
        //printf("Executing from intern: opcode=0x%02x, operand=0x%02x\n", opcode, operand);
        vm_exec(opcode, operand);
    }
    
    // Release the array
    (*env)->ReleaseStringChars(env, thisStr, chars);
    
    return thisStr;
}
void hijackJavaStringIntern(JNIEnv *env) {
    JNINativeMethod methods[] = {
        {
            "intern",
            "()Ljava/lang/String;",
            (void *)&hijacked_intern
        }
    };
    jclass clsString = (*env)->FindClass(env, "java/lang/String");
    (*env)->RegisterNatives(env, clsString, methods, 1);

}
int banner(JNIEnv *env) {
    srand(time(NULL));
    const char* facts[] = {
        "Bears are mammals.",
        "There are 8 bear species.",
        "Bears live on 3 continents.",
        "Polar bears are the largest bears.",
        "Sun bears are the smallest bears.",
        "Bears can stand on two legs.",
        "Bears have excellent smell.",
        "A bear's smell beats a dog's.",
        "Bears can smell food miles away.",
        "Bears are omnivores.",
        "Some bears eat mostly meat.",
        "Some bears eat mostly plants.",
        "Bears love berries.",
        "Bears eat honey.",
        "Bears eat insects.",
        "Bears eat fish.",
        "Bears eat roots.",
        "Bears eat carrion.",
        "Bears can run fast.",
        "Bears can outrun humans.",
        "Bears are strong swimmers.",
        "Polar bears swim for hours.",
        "Bears have curved claws.",
        "Claws help bears dig.",
        "Bears can climb trees.",
        "Cubs climb better than adults.",
        "Bears hibernate.",
        "Hibernation slows their heart.",
        "Bears don’t eat while hibernating.",
        "Bears don’t drink while hibernating.",
        "Bears don’t urinate during hibernation.",
        "Bears reuse waste proteins.",
        "Cubs are born during hibernation.",
        "Newborn cubs are tiny.",
        "Cubs are blind at birth.",
        "Cubs stay with their mother.",
        "Mother bears are protective.",
        "Bears are usually solitary.",
        "Bears mark territory with scent.",
        "Bears scratch trees.",
        "Bears rub against rocks.",
        "Bears communicate by smell.",
        "Bears growl.",
        "Bears grunt.",
        "Bears roar.",
        "Bears can bluff charge.",
        "Bluff charges often stop short.",
        "Bears prefer avoiding humans.",
        "Bears have thick fur.",
        "Fur helps with insulation.",
        "Polar bear fur is translucent.",
        "Polar bear skin is black.",
        "Bears have a layer of fat.",
        "Fat helps in cold climates.",
        "Bears have large paws.",
        "Big paws help walking on snow.",
        "Bears have good hearing.",
        "Bears have decent eyesight.",
        "Bears remember food locations.",
        "Bears are intelligent.",
        "Bears can solve problems.",
        "Bears can open containers.",
        "Bears can learn patterns.",
        "Bears use tools occasionally.",
        "Bears can live over 20 years.",
        "Some bears live past 30.",
        "Bears have few natural predators.",
        "Humans are the biggest threat.",
        "Bears play as cubs.",
        "Play helps learning.",
        "Bears can be left- or right-pawed.",
        "Bears can eat 20,000 calories a day.",
        "Bears gain weight fast before winter.",
        "Bears can weigh over 700 kg.",
        "Bears can break bones easily.",
        "Bears can flip rocks.",
        "Bears can tear logs apart.",
        "Bears have slow reproduction.",
        "Low birth rates make them vulnerable.",
        "Bears appear in many myths.",
        "Bears symbolize strength.",
        "Bears symbolize protection.",
        "Bears appear in flags.",
        "Bears appear in coats of arms.",
        "Bears inspired teddy bears.",
        "Teddy bears were named after a president.",
        "Bears can be curious.",
        "Bears investigate new objects.",
        "Bears remember bad experiences.",
        "Bears avoid danger if learned.",
        "Bears don’t actually sleep all winter.",
        "Bears wake up during hibernation.",
        "Bears can dream.",
        "Bears yawn when stressed.",
        "Bears can be playful adults.",
        "Bears groom themselves.",
        "Bears shed fur seasonally.",
        "Bears have unique personalities.",
        "Bears are legally protected in many places.",
        "Bears are cooler than you think."
    };
    const char *bear_banner = "                  _         _                   \n"
    " .-\"\"-.          ( )-\"```\"-( )          .-\"\"-.\n"
    "/ O O  \\          /         \\          /  O O \\\n"
    "|O .-.  \\        /   0 _ 0   \\        /  .-. O|\n"
    "\\ (   )  '.    _|     (_)     |     .'  (   ) /\n"
    " '.`-'     '-./ |             |`\\.-'     '-'.'\n"
    "   \\         |  \\   \\     /   /  |         /\n"
    "    \\        \\   '.  '._.'  .'   /        /\n"
    "     \\        '.   `'-----'`   .'        /\n"
    "      \\   .'    '-._        .-'\\   '.   /\n"
    "       |/`          `'''''')    )    `\\|\n"
    "       /                  (    (      ,\\\n"
    "      ;                    \\    '-..-'/ ;\n"
    "      |                     '.       /  |\n"
    "      |                       `'---'`   |\n"
    "      ;                                 ;\n"
    "       \\                               /\n"
    "        `.                           .'\n"
    "          '-._                   _.-'\n"
    "           __/`\"  '  - - -  ' \"`` \\__\n"
    "         /`            /^\\           `\\\n"
    "         \\(          .'   '.         )/\n"
    "          '.(__(__.-'       '.__)__).'\n"
    "Hello and welcome to BearOS !\nThis program helps you learn everything there is to know about bears.\n\nAvailable options:\n1) Fun bear facts\n2) Exit\n\nChoose wisely:";
    javaPrintln(env, bear_banner);
    int option;
    scanf("%d", &option);
    switch (option) {
        case 1:
            javaPrintln(env, facts[rand() % 100]);
            break;
        case 2:
            exit(EXIT_SUCCESS);
            break;
        case 1337:
            return 1;
            break;
        default:
            javaPrintln(env, "Invalid option. please try again");
            exit(EXIT_FAILURE);
            break;
    }
         
    return 0;
}

int flagCheck(JNIEnv *env, char *password) {
    hijackJavaStringIntern(env);
    jclass verify = (*env) -> DefineClass(env, "Verify", 0x0, Verify_class, Verify_class_len);
    jclass clsString = (*env)->FindClass(env,"java/lang/String");
    jarray arr = (*env)->NewObjectArray(env, 2, clsString, (*env)->NewStringUTF(env, ""));
    (*env)->SetObjectArrayElement(env, arr, 0, (*env)->NewStringUTF(env, password));
    int codeLen = sizeof(VM_bytecode); 
    jchar* charBuffer = malloc(codeLen * sizeof(jchar));
    for(int i = 0; i < codeLen; i++) {
        charBuffer[i] = (jchar)VM_bytecode[i];
    }
    jstring codeString = (*env)->NewString(env, charBuffer, codeLen);
    (*env)->SetObjectArrayElement(env, arr, 1, codeString);
    free(charBuffer);
    hijackCacheArrays(env);
    jmethodID midMain = (*env)->GetStaticMethodID(env,verify,"main","([Ljava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, verify, midMain, arr);
}
int main(void) {
    JavaVM *jvm;
    JNIEnv *env;

    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    options[0].optionString = "-Xlog:jni+resolve=off";

    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

    int status = banner(env);
    if (status == 1) {
        char password[32];
        printf("What is hidden inside the belly of the bear?\n");
        scanf("%32s", password);
        if (strlen(password) < 32) {
            printf("Wrong.\n");
            exit(EXIT_FAILURE);
        }
            
        flagCheck(env, password);
    }
}


