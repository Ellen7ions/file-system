#include <iostream>
#include "VirtualDisk.hpp"
#include "ftree/gui.h"


int main() {

    // file_system = new FileSystem("test.bin", DISK_SIZE, CREATE);
    file_system = new FileSystem("test.bin", DISK_SIZE, READ);

    gui_top(nullptr);

    return 0;
}
