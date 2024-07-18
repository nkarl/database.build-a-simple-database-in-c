#ifndef MACROS_H
#define MACROS_H

#define debug(...) fprintf(stderr, __VA_ARGS__);

#define PRINT_PROMPT() printf("db > ");

#define VAR_STRING(VAR) #VAR

#define SHOW_SUPPORTED_CMDS()                    \
    debug("supported commands:\n");              \
    for (size_t i = 0; commands[i]; ++i) {       \
        debug("\t\t\t%s\n", commands[i]);        \
    }                                            \
    debug("supported start_keywords :\n");       \
    for (size_t i = 0; start_keywords[i]; ++i) { \
        debug("\t\t\t%s\n", start_keywords[i]);  \
    }

#endif
