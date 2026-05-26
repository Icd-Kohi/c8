#include "../../Core/Source/Core/Core.h"
#include <cctype>
#include <cstdint>
#include <cstring>
#include <sys/types.h>

#define SIZE(obj) sizeof(obj)/sizeof(obj[0])
typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

// modeling memory layout
// the padding of a struct is based on the largest member of the struct, so until we have a 6 byte member being the largest, the struct will increase the memory based on multiple of 6.
// in the below case, we have the largest member being 8 bytes total, so the struct will prefer to allocate multiples of 8 bytes in the memory.
typedef struct {
    float   afloat;    // 4bytes
    char    chars[6];  // 6bytes
    i8      negative;  // 1byte
    u8      positive;  // 1 + 4bytes
    void    *myptr;    // 8 bytes
    bool    mebool;    // 1 + 7bytes
                       // TOTAL : 25bytes because of memory padding and alignment
}my_struct_t;

my_struct_t g_mystruct {
    .afloat     = 0.0 ,
    .chars      = "hi",
    .negative   = -1  ,
    .positive   = 4   ,
    .myptr      = NULL,
    .mebool     = false
};

// __attribute__((packed)) tells the compiler to not add padding to the struct, so even if the memory layout wasn't naturally aligned, it would forcefully pack it anyways.

int Core::main(){
    int thing = 0;
    return thing;
};

int main()
{
	//Core::Printing();
        
    return Core::main();
}

void process(char *s){
    for(int i = 0; i < strlen(s); ++i){
        s[i] = toupper(i);
    }
}
