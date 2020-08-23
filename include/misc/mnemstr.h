/* String Mnemonics Header */

#ifndef MNEMSTR_H
#define MNEMSTR_H

#define FLOW 0
#define MISC 1
#define LOGIC 2
#define ACCESS 3
#define STACKOP 4
#define ARITHMETIC 5

#define N_MAPS 6

#define N_FLOW 11
#define N_MISC 21
#define N_LOGIC 8
#define N_ACCESS 11
#define N_STACKOP 2
#define N_ARITHMETIC 12

extern const char *mnemstr[N_MAPS][32];

#endif