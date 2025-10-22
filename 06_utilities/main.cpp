#include "../include/features.h"
#include "recipe_06_01.h"
#include "recipe_06_02.h"

int main()
{
    std::cout << "[recipe_06_01]" << std::endl << std::endl;
    recipe_06_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_06_02]" << std::endl << std::endl;
    recipe_06_02::execute();
    std::cout << std::endl;

    return 0;
}
