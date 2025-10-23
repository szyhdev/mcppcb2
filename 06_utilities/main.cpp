#include "../include/features.h"
#include "recipe_06_01.h"
#include "recipe_06_02.h"
#include "recipe_06_03.h"
#include "recipe_06_04.h"

int main()
{
    std::cout << "[recipe_06_01]" << std::endl << std::endl;
    recipe_06_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_06_02]" << std::endl << std::endl;
    recipe_06_02::execute();
    std::cout << std::endl;

    std::cout << "[recipe_06_03]" << std::endl << std::endl;
    recipe_06_03::execute();
    std::cout << std::endl;

    std::cout << "[recipe_06_04]" << std::endl << std::endl;
    recipe_06_04::execute();
    std::cout << std::endl;

    return 0;
}
