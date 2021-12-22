#ifndef _GUI_H_
#define _GUI_H_

#include "cstring"
#include "cstdio"
#include "cstdlib"
#include "utils.h"
#include "cutils.h"
#include "cmd.h"

typedef struct GUI {
    int a;
} GUI;

void gui_top(GUI *gui);

int gui_input_cmd(int *argc, char **argv);

void gui_show(GUI *gui);

#endif