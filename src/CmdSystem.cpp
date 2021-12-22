//
// Created by Ellen7ions on 2021/12/20.
//

#include "cmd/CmdSystem.hpp"

CmdSystem::CmdSystem(std::string filename, uint32_t size, uint8_t mode) {
    this->disk = new VirtualDisk(filename, size, mode);
    this->curPath = "/";
}

void CmdSystem::cmdRun() {
    while (true) {
        std::string cmd;
        std::cout << this->curPath + ">";
        std::cin >> cmd;

        if (cmd == "ls")
            ls(0, nullptr);

        if (cmd == "q") {
            this->disk->diskClose();
            break;
        }
    }
}

void *CmdSystem::ls(int argc, char **args) {
    for (int i = 0; i < 10; i++) {


        if (strlen(this->disk->dirItems[i].filename) == 0) break;
        uint8_t type = this->disk->indexItems[this->disk->dirItems[i].index_i].fileType;
        if (type == 'd')
            std::cout << "/";
        std::cout << this->disk->dirItems[i].filename << std::endl;
    }
    return nullptr;
}
