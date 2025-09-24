#include "../include/features.h"
#include "recipe_08_01.h"
#include "recipe_08_02.h"
#include "recipe_08_06.h"
#include "recipe_08_07.h"
#include "recipe_08_08.h"

int main()
{
    std::cout << "[recipe_08_01]" << std::endl << std::endl;
    recipe_08_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_08_02]" << std::endl << std::endl;
    recipe_08_02::execute();
    std::cout << std::endl;

    std::cout << "[recipe_08_06]" << std::endl << std::endl;
    recipe_08_06::execute();
    std::cout << std::endl;

    std::cout << "[recipe_08_07]" << std::endl << std::endl;
    recipe_08_07::execute();
    std::cout << std::endl;

    std::cout << "[recipe_08_08]" << std::endl << std::endl;
    recipe_08_08::execute();
    std::cout << std::endl;

    return 0;
}
