#ifndef MENU_H
#define MENU_H

#include <command.h>
#include <stdint.h>


const char * menuExecuteCommand(char *commandNameWithArg);
void menuAddItem(Command *command);

static struct {
    const char * (*executeCommand)(char *commandNameWithArg);
    void (*addItem)(Command *command);
} const Menu = {
        menuExecuteCommand,
        menuAddItem
};

#endif //MENU_H
