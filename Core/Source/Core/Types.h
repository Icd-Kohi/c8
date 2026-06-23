#ifndef TYPES
#define TYPES

#include <iostream>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <fstream>

#define SIZE(obj) sizeof(obj)/sizeof(obj[0])
typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

const u8 FONTSET_SIZE = 80;
const u16 START_ADDRESS = 0x200;
const u8 VIDEO_WIDTH = 64;
const u8 VIDEO_HEIGHT = 32;

// Initialize PC with a constructor
const u8 FONTSET_START_ADDRESS = 0x50;


#endif
