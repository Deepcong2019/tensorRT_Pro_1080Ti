#include <iostream>
using namespace std;

bool requires(const char* name);
int main(){
    if (!requires("ooo")) {
        std::cout << "CUDA not found!" << std::endl;
    }

    return 0;
}
