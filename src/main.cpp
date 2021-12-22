#include <iostream>
#include "VirtualDisk.hpp"
#include "ftree/gui.h"


int main(int argc, char **argv) {

    // file_system = new FileSystem("test.bin", DISK_SIZE, CREATE);
    uint8_t mode;
    if (argc == 1) {
        mode = READ;
    } else {
        mode = atoi(argv[1]);
    }
    file_system = new FileSystem("test.bin", DISK_SIZE, mode);

    gui_top(nullptr);

    return 0;
}
