/* Debug Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <inttypes.h>

// Project Headers
#include "cli/debugcommands.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"
#include "disassembler/decoder.hpp"
#include "system/sys.hpp"
#include "system/mcu.hpp"
#include "table/table.hpp"
#include "misc/stringmanip.hpp"
#include "misc/filemanip.hpp"
#include "misc/stdmsg.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table) {

    do {

        if(table->is_break() == true)
            break;

        sys->put_sys(dwin);
        sys->step();

        sleep_for(milliseconds(100));

    } while(sys->is_terminated() == false);

    dwin->write(OUTPUT_PANEL, BREAK_REACHED, G);
}

void examine_data(DebugWindow *dwin, Sys *sys, string mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > RAM_END) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_data(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}

void examine_eeprom(DebugWindow *dwin, Sys *sys, string mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > EEPROM_SIZE - 1) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_eeprom(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}

void examine_eeprom_char(DebugWindow *dwin, Sys *sys, string mem_cell, string range) {

    int cell = hex_to_dec(mem_cell);
    int offs = to_dec(range);

    if(offs < 0 || (cell + offs) < 0 || (cell + offs) > EEPROM_SIZE - 1) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    string ascii = "";

    for(int i = cell; i < (cell + offs); i++)
        ascii += (char) sys->read_eeprom(i);

    dwin->write(OUTPUT_PANEL, val_of_eep(mem_cell, range, ascii), DEF);
}

void load_eep_hex(DebugWindow *dwin, Sys *sys, string file) {

    vector <struct data> d;

    if(file_exists(file) == false) {

        dwin->write(OUTPUT_PANEL, file_invalid(file), DEF);
        return;
    }

    decode_eep_hex(file).swap(d);

    if(d.size() == 0) {

        dwin->write(OUTPUT_PANEL, EEP_DECODE_ERR, DEF);
        return;
    }

    for(int i = 0; i < d.size(); i++)
        sys->write_eeprom(d[i].addr, (int8_t) d[i].value);

    dwin->write(OUTPUT_PANEL, eep_success(file), G);
}