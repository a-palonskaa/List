#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "define_colors.h"
#include "logger.h"
#include "list.h"

const char* LOGGER_FILE = "logs/logger2.html";

signed int compare_ints(void* elem1, void* elem2);

int main() {
    LoggerSetFile(stdout);
    LoggerSetLevel(INFO);

    list_t list{};
    list.ctor();

    FILE* ostream = fopen(LOGGER_FILE, "wb");
    if (ostream == nullptr) {
        LOG(ERROR, "Failed to open logger file\n" STRERROR(errno));
    }

    list.set_dump_ostream(ostream);

    list.insert_after(0, 666);
    list.insert_after(1, 333);
    list.insert_after(2, 200);
    list.dump();

    list.insert_after(1, 1);
    list.delete_at(2);
    list.dump();

    list.dtor();
}

signed int compare_ints(void* elem1, void* elem2) {
    return *((int*) elem1) - *((int*) elem2);
}
