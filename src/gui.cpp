// #include "ftree/gui/gui.h"
#include "ftree/gui.h"

void gui_show(GUI *gui) {

}

void gui_top(GUI *gui) {
    int *argc = (int *) malloc(sizeof(int));
    char **argv = (char **) malloc((sizeof(char *) * 10));
    while (true) {
        int cmd_index = gui_input_cmd(argc, argv);
        if (cmd_index == -1) {
            printf("Unrecognizable commands!\n");
            continue;
        }
        int flag = sys_cmds[cmd_index](argc, argv);
        if (flag == -2) break;
    }
}

int gui_input_cmd(int *argc, char **argv) {
    char cmd_buffer[128];
    char input_cmd[100];
    // printf("%s>", file_system->cur_path.c_str());
    std::cout << file_system->cur_path << ">";
    fgets(cmd_buffer, 128, stdin);
    get_cmd(cmd_buffer, input_cmd);
    int cmd_index = get_cmd_index(input_cmd);
    str_to_arr(cmd_buffer, argc, argv);
    return cmd_index;
}
