#include "dreamberd.h"
#include "nob.h"

bool dbRunProgram(const char* text) {
    (void) text;
    nob_log(NOB_ERROR, "Hello, World!");
    return false;
}
