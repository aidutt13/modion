#include <iostream>
#include "Modrinth.hpp"

int32_t main()
{
    auto mods = Modion::Modrinth::search("create");

    std::cout << Modion::Modrinth::get_mod(mods[0].id).title << std::endl;


    return 0;
}