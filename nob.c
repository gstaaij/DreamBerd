#define NOB_IMPLEMENTATION
#include "src/dreamberd/nob.h"

void cFlags(Nob_Cmd* cmd) {
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
}

void cFiles(Nob_Cmd* cmd) {
    static const char* files[] = {
        "src/main.c",
        "src/dreamberd/dreamberd.c",
        "src/dreamberd/lexer.c",
    };
    
    for (size_t i = 0; i < NOB_ARRAY_LEN(files); ++i) {
        nob_cmd_append(cmd, files[i]);
    }
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

void logUsage(Nob_Log_Level level, const char* program) {
    nob_log(level, "Usage: %s [options]", program);
    nob_log(level, "Available options:");
    nob_log(level, "    --help      Display this message");
    nob_log(level, "    --nowin     Don't build for Windows");
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    bool nowin = false;

    const char* program = nob_shift_args(&argc, &argv);
    while (argc > 0) {
        const char* option = nob_shift_args(&argc, &argv);
        if (strcmp(option, "--help") == 0) {
            logUsage(NOB_INFO, program);
            return 0;
        } else if (strcmp(option, "--nowin") == 0) {
            nowin = true;
        } else {
            nob_log(NOB_ERROR, "Invalid option '%s'", option);
            logUsage(NOB_ERROR, program);
            return 1;
        }
    }

    nob_mkdir_if_not_exists("./build");
    Nob_Cmd cmd = {0};

    if (!nowin) buildWindows(&cmd);
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
