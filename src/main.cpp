#include "parser.hpp"

#include <regex>
#include <fstream>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\033[31mNo input file.\n\033[0m");
        return -1;
    }

    std::ifstream f{argv[1], std::ios::binary | std::ios::ate};

    if (!f.is_open()) {
        printf("\033[31mCouldn't open file.\n\033[0m");
        return -2;
    }

    size_t size = f.tellg();
    std::string buffer;
    buffer.reserve(size);
    f.seekg(0);
    f.read(buffer.data(), size);
    f.close();
    std::string tmp = std::string(buffer.c_str());

    tmp = std::regex_replace(tmp, std::regex("\\(|\\{|\\)|\\}|;|<=|>=|==|\\+|-|\\*|\\/|=|>|<"), " $& ");
    tmp = std::regex_replace(tmp, std::regex("\n"), " ");
    tmp = std::regex_replace(tmp, std::regex(" {2,}"), " ");

    if(tmp == "") {
        printf("\033[31mEmpty input.\n\033[0m");
        return -3;
    }

    Parser p;

    std::string token;
    std::stringstream ss(tmp);
    while(std::getline(ss, token, ' ')) {
        p.input.push_back({p.determine_symbol(token), token});
    }
    p.input.push_back({END, ""});
    
    if(!p.parse()) {
        return -4;
    }


    return 0;
}