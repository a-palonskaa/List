#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "define_colors.h"
#include "logger.h"
#include "list.h"

const char* LOGGER_FILE = "logs/logger.html";

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

    list.insert_after(0, 1);
    list.dump();

    list.insert_after(1, 2);
    list.dump();

    list.insert_after(1, 3);
    list.dump();

    list.push_front(100);
    list.dump();

    list.push_front(1000);
    list.dump();

    list.pop_front();
    list.dump();

    fclose(ostream);
    list.dtor();
}

signed int compare_ints(void* elem1, void* elem2) {
    return *((int*) elem1) - *((int*) elem2);
}
