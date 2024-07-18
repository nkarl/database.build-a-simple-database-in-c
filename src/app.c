/*
 * GOAL: The goal of this project is to build a simple SQL-like database.
 *  - The project should be kept to be small and simple.
 *  - It strives for correctness but not completeness.
 *    - supports a small set of commands as proof of concepts.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "actions.h"
#include "macros.h"
#include "types.h"

/*#define _DEBUG                 1*/

#define DEFAULT_COMMAND_LENGTH 2048  // 2048 characters
#define COMMAND_TABLE_SIZE     16

typedef struct {
    size_t  buf_size;
    ssize_t cmd_len;
    /*char   *cmdline;*/
    char cmdline[DEFAULT_COMMAND_LENGTH];
} CmdBuffer;  // the buffer too hold the command/query statement

// The symbol table for supported commands
char const *commands[COMMAND_TABLE_SIZE] = {
    ".exit",
};

// The symbol table for supported query keyword
char const *start_keywords[COMMAND_TABLE_SIZE] = {
    "insert",
    "select",
};

/*
 * The function table for the coresponding commands
 */
int (*exec_cmd[COMMAND_TABLE_SIZE])(int _, ...) = {
    cmd_exit,
};

/*
 * The function table for the coresponding start_keywords
 */
int (*exec_statement[COMMAND_TABLE_SIZE])(int _, ...) = {
    func_insert,
    func_select,
};

/**
 * Read and store the number of bytes (newline terminated) read from the console in a buffer.
 */
int read_cmdline(CmdBuffer *buf) {
    // read the number of characters, including the '\n' at the end
    char   *cmdline  = buf->cmdline;
    size_t  buf_size = buf->buf_size;
    ssize_t bytes    = getline(&cmdline, &buf_size, stdin);
    if (bytes <= 0) {
        debug("Error reading input\n");
        return ERROR;
    }
    buf->cmd_len            = bytes - 1;
    buf->cmdline[bytes - 1] = 0;
#ifdef _DEBUG
    for (ssize_t i = 0; i < bytes; ++i) {
        char c = buf->cmdline[i];
        if (c == '\0') {
            putchar('\\');
            putchar('0');
        }
        else
            putchar(c);
    }
    putchar('\n');
#endif
    return OK;
}

// command
int search_commands(char *const cmdline) {
    if (strcmp(cmdline, ".exit") == 0) {
        return EXIT;
    }
    return ERROR;
}

// query
int search_queries(char *const cmdline) {
    int a = 0;
    for (size_t i = 0; start_keywords[i]; ++i) {
#ifdef _DEBUG
        debug("cmdline=%s start_keywords[%ld]=%s\n", cmdline, i, start_keywords[i]);
#endif
        if (strcmp(cmdline, start_keywords[i]) == 0) {
            a = (*exec_statement[i])(OK);

#ifdef _DEBUG
            char const *msg = a == ERROR  ? VAR_STRING(ERROR)
                              : a == EXIT ? VAR_STRING(EXIT)
                                          : VAR_STRING(OK);
            debug("a=%s\n", msg);
#endif
            return a;
        }
    }
    return ERROR;
}

// filter between meta-commands and query statements
int filter_cmdline(char *const cmdline) {
    int exec = 0;
    // 1. search in the command table
    exec = search_commands(cmdline);
    if (exec > ERROR) {
        // do command thing
        return exec;
    }
    // 2. search in the query table
    exec = search_queries(cmdline);
    if (exec > ERROR) {
        // do query thing
        return exec;
    }
    // otherwise, command/query not supported
    debug("unrecognized command...\n");
    return OK;
}

/**
 * NOTE: app entry.
 */
int main(void) {
    CmdBuffer buf = {
        .buf_size = DEFAULT_COMMAND_LENGTH,
        .cmd_len  = 0,
        /*.cmdline  = malloc(DEFAULT_COMMAND_LENGTH * sizeof(char)),*/
    };

#ifdef _DEBUG
    SHOW_SUPPORTED_CMDS();
#endif

    int app_state = OK;
    // live app loop
    while (app_state >= OK) {
        PRINT_PROMPT();
        app_state = read_cmdline(&buf);
        app_state = filter_cmdline(buf.cmdline);
    }

    /*free(buf.cmdline);*/
    return app_state;
}

/*
 * TODO:
 *  - [x] set up the test suite and runner
 *  - [ ] look into variadic arguments for functions
 *    - REF: https://www.gnu.org/software/libc/manual/html_node/How-Variadic.html
 *  - [ ] look into how to build a parser for the query statements
 *    - how does _parsing_ a SQL statement work?
 *    - how does the _syntax_ and _semantics_ of the language apply?
 *    - REF: https://www.sqlite.org/arch.html
 */
