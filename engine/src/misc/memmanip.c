/* Memory Manipulation Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers (engine)
#include "engine/include/misc/memmanip.h"

void vmcu_nfree(const int n, ...) {

    va_list var;
    va_start(var, n);

    for(int i = 0; i < n; i++) {

        void *p = va_arg(var, void*);

        if(p != NULL)
            free(p);
    }

    va_end(var);
}