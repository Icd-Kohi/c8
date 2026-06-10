#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#define SIZE(n) sizeof(n)/sizeof(n[0]);
int main(){
    std::ifstream file("./file.txt");
    std::ofstream receipt("./file2.txt");
    std::copy(std::istreambuf_iterator<char>(file),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator(receipt));
   return 0; 
}
