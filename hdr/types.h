#ifndef TYPES_H
#define TYPES_H

typedef char  i8;
typedef short i16;
typedef int   i32;
typedef long  i64;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

// a simple enum to reflects the app's state.
enum
{
    ERROR = -1,  // should always exit with a debug message
    EXIT  = 0,
    OK    = 1
};

#endif
