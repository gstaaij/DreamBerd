#define NOB_IMPLEMENTATION
#include "src/nob.h"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_mkdir_if_not_exists("./build");

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc", "-Wall", "-Wextra", "-ggdb");
    nob_cmd_append(&cmd, "-o", "build/utf8test");
    nob_cmd_append(&cmd, "src/utf8test.c");
    if (!nob_cmd_run_sync(cmd)) return 1;

    return 0;
}
