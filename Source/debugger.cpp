/* Implementation of Debugger Functions */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

// Project Headers
#include "debugger.hpp"
#include "ehandling.hpp"
#include "debugparser.hpp"
#include "menus.hpp"
#include "table.hpp"
#include "sys.hpp"

#define TIP_UNDEF -1
#define TIP_DEF 0

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

void step_forward(Sys *sys, Table *table);
void jump_forward(Sys *sys, Table *table, int cursor);

void debug(Table *table) {

    Sys sys( table->src() );
    DebugParser parser;

    int cursor = 0;
    string last_select, select;
	
    table->set_tip(TIP_DEF);

    if(table->has_break() == true)
        jump_forward(&sys, table, cursor);
	
    do {

        debug_menu(&sys, table, cursor);
        getline(cin, select);

        if(table->size() <= 0)
            continue;
		
        if(select != "")
            last_select = select;

        switch(parser.index_of(last_select)) {

            case 0: step_forward(&sys, table); break;
            case 1: movec(&cursor, +1, 4); break;
            case 2: sys.scale_data(+1); break;
            case 3: sys.scale_data(-1); break;
            case 4: jump_forward(&sys, table, cursor); break;
            case 5: /* Exit */ break;

            default: /* Ignoring invalid Input */ break;
        } 
		
    } while(select != "e");

    table->set_tip(TIP_UNDEF);
}

void step_forward(Sys *sys, Table *table) {

    if(table->executable() == true)
        sys->step();

    if(table->step() < 0)
        sys->kill();
}

void jump_forward(Sys *sys, Table *table, int cursor) {

    int line;

    do {

        line = table->get_tip();

        if(table->is_break(line) == true)
            break;

        debug_menu(sys, table, cursor);
        step_forward(sys, table);

        sleep_for(milliseconds(100));

    } while(sys->is_terminated() == false);
}


