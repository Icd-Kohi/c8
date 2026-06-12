#include "../../Core/Source/Core/Core.h"
#include "../../Core/Source/Core/Chip8.h"
#include "../../Core/Source/Core/Types.h"

int Core::main(){
    int thing = 0;
    return thing;
};

u8 hey();

int main()
{
	//Core::Printing();
    
    Chip8::Chip8* thing;
    Chip8::Chip8 thing2;

    std::cout << "Checking...\n";

    return Core::main();
}

void process(char *s){
    for(int i = 0; i < strlen(s); ++i){
        s[i] = toupper(s[i]);
    }
}
