#include "error.h"
#include <stdnoreturn.h>

noreturn void error(const char *error_str, error_t error_type) {
    fprintf(stderr, "Error: %s\n", error_str);
    ifj15_free_all();
    exit(error_type);
}

void warning(const char *warn_str) {
    fprintf(stderr, "Warning: %s\n", warn_str);
}
