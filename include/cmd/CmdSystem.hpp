//
// Created by Ellen7ions on 2021/12/20.
//

#ifndef COMPLEX_FILE_SYSTEM_CMDSYSTEM_HPP
#define COMPLEX_FILE_SYSTEM_CMDSYSTEM_HPP

#include "VirtualDisk.hpp"

class CmdSystem {
private:
    // void *(*func[10])(CmdSystem *cmd, int argc, char *args);

    VirtualDisk *disk;
    std::string curPath;

public:
    CmdSystem(std::string filename, uint32_t size, uint8_t mode);

    void cmdRun();

    void *ls(int argc, char **args);
};


#endif //COMPLEX_FILE_SYSTEM_CMDSYSTEM_HPP
