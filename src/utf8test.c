#define NOB_IMPLEMENTATION
#include "nob.h"

int main() {
    Nob_String_Builder sb = {0};
    if (!nob_read_entire_file("./utf8test.txt", &sb)) return 1;
    nob_sb_append_null(&sb);
    printf("%s\n", sb.items);
    return 0;
}
