#define NOB_IMPLEMENTATION
#include "dreamberd/nob.h"

#include "dreamberd/dreamberd.h"

void logUsage(Nob_Log_Level level, const char* program) {
    nob_log(level, "Usage: %s <file>", program);
}

int main(int argc, char** argv) {
    const char* program = nob_shift_args(&argc, &argv);

    if (argc < 1) {
        nob_log(NOB_ERROR, "No DreamBerd file provided!");
        logUsage(NOB_ERROR, program);
        return 1;
    }

    const char* path = nob_shift_args(&argc, &argv);
    Nob_String_Builder text = {0};
    if (!nob_read_entire_file(path, &text)) return 1;
    nob_sb_append_null(&text);

    if (!dbRunProgram(text.items)) return 1;
    
    return 0;
}
