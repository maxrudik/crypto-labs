#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    std::string in(argv[1]);
    for (int i = 0; i < in.length(); i += 4) {
        in.insert(i, "\\x");
    }
    std::cout << in << std::endl;
    return 0;
}
