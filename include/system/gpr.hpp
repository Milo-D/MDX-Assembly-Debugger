/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_HPP
#define GPR_HPP

// C++
#include <vector>

// C Headers
#include <inttypes.h>

class Gpr {

public:
    Gpr(void);                                  // gpr constructor
    ~Gpr(void);                                 // default gpr destructor

    int8_t read(int rx);                        // reading value from gpr 'rx'
    void write(int rx, int8_t data);            // writing 'data' to gpr 'rx'

    void scale(int offs);                       // scaling GPR file
    std::string to_str(void);                   // returning printable gpr string

private:
    int8_t *reg;                                // gpr file
    unsigned int size;                          // total size of gpr file (size * 8-bit)

    int cursor;                                 // GPR file cursor
    std::vector <std::string> color;            // associated gpr colors

    void clear_color(void);                     // clearing gpr output colors
};

#endif