#include "../include/features.h"
#include "recipe_04_01.h"
#include "recipe_04_02.h"
#include "recipe_04_03.h"
#include "recipe_04_04.h"
#include "recipe_04_05.h"
#include "recipe_04_06.h"

int main()
{
    std::cout << "[recipe_04_01]" << std::endl << std::endl;
    recipe_04_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_04_02]" << std::endl << std::endl;
    recipe_04_02::execute();
    std::cout << std::endl;

    std::cout << "[recipe_04_03]" << std::endl << std::endl;
    recipe_04_03::execute();
    std::cout << std::endl;

    std::cout << "[recipe_04_04]" << std::endl << std::endl;
    recipe_04_04::execute();
    std::cout << std::endl;

    std::cout << "[recipe_04_05]" << std::endl << std::endl;
    recipe_04_05::execute();
    std::cout << std::endl;

    std::cout << "[recipe_04_06]" << std::endl << std::endl;
    recipe_04_06::execute();
    std::cout << std::endl;

    return 0;
}
