#ifndef TYPES
#define TYPES

#include <iostream>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <fstream>

#define SIZE(obj) sizeof(obj)/sizeof(obj[0])
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

const u8 FONTSET_SIZE = 80;
const u16 START_ADDRESS = 0x200;
const u8 VIDEO_WIDTH = 64;
const u8 VIDEO_HEIGHT = 32;

// Initialize PC with a constructor
const unsigned int FONTSET_START_ADDRESS = 0x50;
#endif
