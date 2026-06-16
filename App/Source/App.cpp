#include "../../Core/Source/Core/Chip8.h"
#include "../../Core/Source/Core/Types.h"

u8 hey();

int main()
{
    
    Chip8::Chip8* thing;
    Chip8::Chip8 thing2;

    std::cout << "Checking...\n";

    return 0;
}

void process(char *s){
    for(int i = 0; i < strlen(s); ++i){
        s[i] = toupper(s[i]);
    }
}
