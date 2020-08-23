/* Mnemonic Generator Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "disassembler/mnemonics.h"
#include "system/mcudef.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/queue.h"

#define TAB 26

/* Forward Declaration of static Functions */

static int extract(const int opcode, const int from, const int to, const int offs);

/* Extern Mnemonic Functions */

char* mnem_nop(const int opcode) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 1, "nop");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 2, fill, "; no operation");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_movw(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) * 2;
    const int src = extract(opcode, 0, 4, 0) * 2;

    char *dstr = get_str(dest);
    char *dstrn = get_str(dest + 1);

    char *sstr = get_str(src);
    char *sstrn = get_str(src + 1);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "movw r", dstrn, ":r", dstr);
    queue_put(stream, 4, ", r", sstrn, ":r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 5, fill, "; R", dstrn, ":R", dstr);
    queue_put(stream, 4, " <- R", sstrn, ":R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(5, dstr, dstrn, sstr, sstrn, fill);

    return mnemonic;
}

char* mnem_muls(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0);

    char *dstr = get_str(dest + 16);
    char *sstr = get_str(src + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "muls r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 5, fill, "; R1:R0 <- R", dstr, " * R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_mulsu(const int opcode) {

    const int dest = extract(opcode, 4, 7, 0);
    const int src = extract(opcode, 0, 3, 0);

    char *dstr = get_str(dest + 16);
    char *sstr = get_str(src + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "mulsu r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 5, fill, "; R1:R0 <- ", dstr, " * R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_fmul(const int opcode) {

    /* in progress */
    return "";
}

char* mnem_ldi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    char vstr[3];
    to_hex(src, vstr);

    char *dstr = get_str(dest + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "ldi r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 5, fill, "; R", dstr, " <- 0x", vstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_rjmp(const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign[2] = "+";

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "rjmp ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
} 

char* mnem_mov(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "mov r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 5, fill, "; R", dstr, " <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_dec(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "dec r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " <- R", dstr, " - 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_inc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "inc r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " <- R", dstr, " + 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_add(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "add r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " + R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_adc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "adc r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- R");
    queue_put(stream, 4, dstr, " + R", sstr, " + CF");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_sub(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "sub r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " - R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_subi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    char vstr[3];
    to_hex(src, vstr);

    char *dstr = get_str(dest + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "subi r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " - ", vstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_sbc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "sbc r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- R");
    queue_put(stream, 4, dstr, " - R", sstr, " - CF");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_push(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "push r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[SP] <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, sstr, fill);

    return mnemonic;
}

char* mnem_pop(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
        
    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "pop r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[--SP]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_in(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);

    char vstr[3];
    to_hex(src, vstr);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "in r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[addr]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_out(const int opcode) {

    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    const int src = extract(opcode, 4, 9, 0);

    char vstr[3];
    to_hex(dest, vstr);

    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "out 0x", vstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[addr] <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, sstr, fill);

    return mnemonic;
}

char* mnem_clr(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "clr r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " xor ", "R", dstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_x(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", X");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[X]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_xi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", X+");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[X+]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_dx(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", -X");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[-X]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_y(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", Y");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[Y]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_yi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", Y+");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[Y+]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ld_dy(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", -Y");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[-Y]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ldd_yq(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    char *dstr = get_str(dest);
    char *sstr = get_str(disp);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "ldd r", dstr, ", Y+", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " <- DATA[Y+", sstr, "]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_ld_z(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "ld r", dstr, ", Z");

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- DATA[Z]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_st_x(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "st X, r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[X] <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, sstr, fill);

    return mnemonic;
}

char* mnem_st_xi(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "st X+, r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[X+] <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, sstr, fill);

    return mnemonic;
}

char* mnem_sts(const int opcode) {

    const int src = extract(opcode, 4, 8, 0) + 16;
    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);

    int addr = (~(0x10 & dest) << 3) | ((0x10 & dest) << 2) | 
                ((0x40 & dest) >> 1) | ((0x20 & dest) >> 1) |
                ((0x0f & opcode));

    char *dstr = itoh(addr);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "sts 0x", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[addr] <- R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_xch(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "xch Z, r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; DATA[Z] <- R", sstr);
    queue_put(stream, 3, ", R", sstr, " <- DATA[Z]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, sstr, fill);

    return mnemonic;
}

char* mnem_brne(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brne ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (Z = 0): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_breq(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "breq ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (Z = 1): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brge(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brge ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (N ^ V = 0): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brpl(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brpl ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (N = 0): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brlo(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brlo ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (C = 1): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brlt(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brlt ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (N ^ V = 1): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brcc(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brcc ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (C = 0): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_brcs(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "brcs ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; (C = 1): PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_rcall(const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign[2] = "+";

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sign[0] = '-';
    }

    char *offstr = get_str(offs);
    char *hexoffs = itoh(offs);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "rcall ", sign, offstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; PC <- PC ", sign);
    queue_put(stream, 3, " 0x", hexoffs, " + 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, offstr, hexoffs, fill);

    return mnemonic;
}

char* mnem_ret(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "ret", fill, "; PC <- DATA[SP]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_cp(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "cp r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 2, " - R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;

}

char* mnem_cpi(const int opcode) {

    const int reg = extract(opcode, 4, 8, 0);
    const uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    char vstr[3];
    to_hex(comp, vstr);

    char *dstr = get_str(reg + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "cpi r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 2, " - 0x", vstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_cpc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "cpc r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " - R", sstr, " - CF");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_lsr(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "lsr r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " <- R", dstr, " > 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_asr(const int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "asr r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 3, " <- R", dstr, " > 1");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_swap(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "swap r", dstr);

    const int len = queue_size(stream);
    char *fill = strfill(' ', len, TAB);

    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, "[7:4] <- R", dstr, "[3:0],", " R");
    queue_put(stream, 4, dstr, "[3:0] <- R", dstr, "[7:4]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_ori(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const uint8_t val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    char vstr[3];
    to_hex(val, vstr);

    char *dstr = get_str(dest + 16);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "ori r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 4, fill, "; R", dstr, " <- ");
    queue_put(stream, 4, "R", dstr, " | 0x", vstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_or_asm(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "or r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " | R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_and_asm(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    char *dstr = get_str(dest);
    char *sstr = get_str(src);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "and r", dstr, ", r", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " & R", sstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_andi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    char vstr[3];
    to_hex(value, vstr);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "andi r", dstr, ", 0x", vstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, " <- R", dstr, " & 0x", vstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic; 
}

char* mnem_com(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    char *dstr = get_str(dest);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, "com r", dstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 2, " <- 0xff - R", dstr);

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_bld(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    char *dstr = get_str(dest);
    char *sstr = get_str(bpos);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "bld r", dstr, ", ", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; R", dstr);
    queue_put(stream, 4, "[", sstr, "]", " <- TF");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_bst(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    char *dstr = get_str(dest);
    char *sstr = get_str(bpos);

    queue_t *stream = queue_ctor();
    queue_put(stream, 4, "bst r", dstr, ", ", sstr);

    const int len = queue_size(stream);

    char *fill = strfill(' ', len, TAB);
    queue_put(stream, 3, fill, "; TF <- R", dstr);
    queue_put(stream, 3, "[", sstr, "]");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(3, dstr, sstr, fill);

    return mnemonic;
}

char* mnem_ses(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "ses", fill, "; SF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_set(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "set", fill, "; TF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_sev(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "sev", fill, "; VF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_sez(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "sez", fill, "; ZF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_seh(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "seh", fill, "; HF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_sec(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "sec", fill, "; CF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_sei(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "sei", fill, "; IF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_sen(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "sen", fill, "; NF <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_cls(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "cls", fill, "; SF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_clt(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "clt", fill, "; TF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_clv(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "clv", fill, "; VF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_clz(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "clz", fill, "; ZF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_clh(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "clh", fill, "; HF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_clc(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "clc", fill, "; CF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_cli(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "cli", fill, "; IF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_cln(const int opcode) {

    queue_t *stream = queue_ctor();

    char *fill = strfill(' ', 3, TAB);
    queue_put(stream, 3, "cln", fill, "; NF <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    free(fill);

    return mnemonic;
}

char* mnem_bclr(const int opcode) {

    const int s_bit = extract(opcode, 4, 7, 0);

    char *dstr = get_str(s_bit);
    char *fill = strfill(' ', 9, TAB);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "bclr 0x0", dstr, fill);
    queue_put(stream, 3, "; ", flags[s_bit], " <- 0x00");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

char* mnem_bset(const int opcode) {

    const int s_bit = extract(opcode, 4, 7, 0);

    char *dstr = get_str(s_bit);
    char *fill = strfill(' ', 9, TAB);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "bset 0x0", dstr, fill);
    queue_put(stream, 3, "; ", flags[s_bit], " <- 0x01");

    char *mnemonic = queue_str(stream);

    queue_dtor(stream);
    nfree(2, dstr, fill);

    return mnemonic;
}

/* Static Mnemonic Functions */

static int extract(const int opcode, int from, int to, int offs) {

    int res = 0;

    for(int i = from; i < to; i++) {

        const int bit = (((0x01 << i) & opcode) >> i);
        res += (bit << (i - from) + offs);
    }

    return res;
}

char* (*mnemonics[INSTR_MAX]) (const int opcode) = { mnem_nop, mnem_movw, mnem_muls, mnem_mulsu, mnem_fmul, mnem_ldi, mnem_rjmp, mnem_mov, 
                                                     mnem_dec, mnem_inc, mnem_add, mnem_adc, mnem_sub, mnem_subi, mnem_sbc, mnem_push, mnem_pop, 
                                                     mnem_in, mnem_out, mnem_clr, mnem_ld_x, mnem_ld_xi, mnem_ld_dx, mnem_ld_y, mnem_ld_yi, mnem_ld_dy, mnem_ldd_yq, 
                                                     mnem_ld_z, mnem_st_x, mnem_st_xi, mnem_sts, mnem_xch, mnem_brne, mnem_breq, mnem_brge, mnem_brpl, 
                                                     mnem_brlo, mnem_brlt, mnem_brcc, mnem_brcs, mnem_rcall, mnem_ret, mnem_cp, mnem_cpi, mnem_cpc, mnem_lsr, mnem_asr, 
                                                     mnem_swap, mnem_ori, mnem_or_asm, mnem_and_asm, mnem_andi, mnem_com, mnem_bld, mnem_bst, mnem_ses, 
                                                     mnem_set, mnem_sev, mnem_sez, mnem_seh, mnem_sec, mnem_sei, mnem_sen, mnem_cls, mnem_clt, mnem_clv, 
                                                     mnem_clz, mnem_clh, mnem_clc, mnem_cli, mnem_cln, mnem_bclr, mnem_bset };
