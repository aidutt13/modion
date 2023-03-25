#include <iostream>
#include "Modrinth.hpp"

int32_t main()
{
    std::cout << Modrinth::raw_search("Create") << std::endl;
    return 0;
}