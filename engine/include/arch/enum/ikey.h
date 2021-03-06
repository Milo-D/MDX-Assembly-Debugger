/* Instruction-Key Header */

#ifndef VMCU_IKEY_H
#define VMCU_IKEY_H

typedef enum {

    VMCU_IKEY_DATA = -1,

    VMCU_IKEY_NOP,
    VMCU_IKEY_MOVW,
    VMCU_IKEY_MUL,
    VMCU_IKEY_MULS,
    VMCU_IKEY_MULSU,
    VMCU_IKEY_FMUL,
    VMCU_IKEY_FMULS,
    VMCU_IKEY_FMULSU,
    VMCU_IKEY_LDI,
    VMCU_IKEY_RJMP,
    VMCU_IKEY_JMP,
    VMCU_IKEY_IJMP,
    VMCU_IKEY_MOV,
    VMCU_IKEY_DEC,
    VMCU_IKEY_INC,
    VMCU_IKEY_ADD,
    VMCU_IKEY_ADC,
    VMCU_IKEY_ADIW,
    VMCU_IKEY_SUB,
    VMCU_IKEY_SUBI,
    VMCU_IKEY_SBC,
    VMCU_IKEY_SBCI,
    VMCU_IKEY_SBIW,
    VMCU_IKEY_PUSH,
    VMCU_IKEY_POP,
    VMCU_IKEY_IN,
    VMCU_IKEY_OUT,
    VMCU_IKEY_SBIS,
    VMCU_IKEY_SBIC,
    VMCU_IKEY_SBRC,
    VMCU_IKEY_SBRS,
    VMCU_IKEY_CPSE,
    VMCU_IKEY_EOR,
    VMCU_IKEY_LDX,
    VMCU_IKEY_LDXI,
    VMCU_IKEY_LDDX,
    VMCU_IKEY_LDY,
    VMCU_IKEY_LDYI,
    VMCU_IKEY_LDDY,
    VMCU_IKEY_LDDYQ,
    VMCU_IKEY_LDDZQ,
    VMCU_IKEY_LDZ,
    VMCU_IKEY_LDZI,
    VMCU_IKEY_LDDZ,
    VMCU_IKEY_STX,
    VMCU_IKEY_STXI,
    VMCU_IKEY_STDX,
    VMCU_IKEY_STY,
    VMCU_IKEY_STYI,
    VMCU_IKEY_STDY,
    VMCU_IKEY_STDYQ,
    VMCU_IKEY_STZ,
    VMCU_IKEY_STZI,
    VMCU_IKEY_STDZ,
    VMCU_IKEY_STDZQ,
    VMCU_IKEY_STS,
    VMCU_IKEY_STS32,
    VMCU_IKEY_LDS,
    VMCU_IKEY_LDS32,
    VMCU_IKEY_XCH,
    VMCU_IKEY_BRNE,
    VMCU_IKEY_BREQ,
    VMCU_IKEY_BRGE,
    VMCU_IKEY_BRPL,
    VMCU_IKEY_BRLO,
    VMCU_IKEY_BRLT,
    VMCU_IKEY_BRCC,
    VMCU_IKEY_BRVS,
    VMCU_IKEY_BRTS,
    VMCU_IKEY_BRTC,
    VMCU_IKEY_BRMI,
    VMCU_IKEY_BRHC,
    VMCU_IKEY_BRHS,
    VMCU_IKEY_BRID,
    VMCU_IKEY_BRIE,
    VMCU_IKEY_BRVC,
    VMCU_IKEY_RCALL,
    VMCU_IKEY_RET,
    VMCU_IKEY_RETI,
    VMCU_IKEY_ICALL,
    VMCU_IKEY_CALL,
    VMCU_IKEY_CP,
    VMCU_IKEY_CPI,
    VMCU_IKEY_CPC,
    VMCU_IKEY_LSR,
    VMCU_IKEY_ASR,
    VMCU_IKEY_ROR,
    VMCU_IKEY_SWAP,
    VMCU_IKEY_ORI,
    VMCU_IKEY_OR,
    VMCU_IKEY_AND,
    VMCU_IKEY_ANDI,
    VMCU_IKEY_LAS,
    VMCU_IKEY_LAC,
    VMCU_IKEY_LAT,
    VMCU_IKEY_COM,
    VMCU_IKEY_NEG,
    VMCU_IKEY_BLD,
    VMCU_IKEY_BST,
    VMCU_IKEY_SBI,
    VMCU_IKEY_CBI,
    VMCU_IKEY_LPM,
    VMCU_IKEY_LPMZ,
    VMCU_IKEY_LPMZI,
    VMCU_IKEY_EICALL,
    VMCU_IKEY_EIJMP,
    VMCU_IKEY_ELPM,
    VMCU_IKEY_ELPMZ,
    VMCU_IKEY_ELPMZI,
    VMCU_IKEY_DES,
    VMCU_IKEY_SLEEP,
    VMCU_IKEY_WDR,
    VMCU_IKEY_BREAK,
    VMCU_IKEY_SPM,
    VMCU_IKEY_SPMZI,
    VMCU_IKEY_SES,
    VMCU_IKEY_SET,
    VMCU_IKEY_SEV,
    VMCU_IKEY_SEZ,
    VMCU_IKEY_SEH,
    VMCU_IKEY_SEC,
    VMCU_IKEY_SEI,
    VMCU_IKEY_SEN,
    VMCU_IKEY_CLS,
    VMCU_IKEY_CLT,
    VMCU_IKEY_CLV,
    VMCU_IKEY_CLZ,
    VMCU_IKEY_CLH,
    VMCU_IKEY_CLC,
    VMCU_IKEY_CLI,
    VMCU_IKEY_CLN

} VMCU_IKEY;

#endif