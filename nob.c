#define NOB_IMPLEMENTATION
#include "src/dreamberd/nob.h"

// Remove this line if you're on Linux and you don't want to install mingw64
#define BUILD_FOR_WINDOWS

void cFlags(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
}

void cFiles(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "src/main.c");
}

bool buildWindows(Nob_Cmd* cmd) {
    cmd->count = 0;
    nob_cmd_append(cmd, "x86_64-w64-mingw32-gcc");
    cFlags(cmd);
    nob_cmd_append(cmd, "-o", "build/dreamberd");
    cFiles(cmd);
    return nob_cmd_run_sync(*cmd);
}

bool buildLinux(Nob_Cmd* cmd) {
    cmd->count = 0;
    nob_cmd_append(cmd, "gcc");
    cFlags(cmd);
    nob_cmd_append(cmd, "-o", "build/dreamberd");
    cFiles(cmd);
    return nob_cmd_run_sync(*cmd);
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_mkdir_if_not_exists("./build");
    Nob_Cmd cmd = {0};

#ifdef BUILD_FOR_WINDOWS
    buildWindows(&cmd);
#endif
#ifndef _WIN32
    buildLinux(&cmd);
#endif

// Enable if you want the UTF-8 test program
#if 0
    cmd.count = 0;
    nob_cmd_append(&cmd, "gcc");
    cFlags(&cmd);
    nob_cmd_append(&cmd, "-o", "build/utf8test");
    nob_cmd_append(&cmd, "src/utf8test.c");
    if (!nob_cmd_run_sync(cmd)) return 1;
#endif

    return 0;
}
