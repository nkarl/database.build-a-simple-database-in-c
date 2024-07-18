#include "actions.h"

#include "macros.h"
#include "types.h"

// command: exit app
int cmd_exit(int _, ...) {
    return OK;
}

// query keyword: isnert
int func_insert(int _, ...) {
    debug("insert data...\n");
    return OK;
}

// query keyword: select
int func_select(int _, ...) {
    debug("select data...\n");
    return OK;
}
