#include "../include/features.h"
#include "recipe_05_01.h"
#include "recipe_05_05.h"
#include "recipe_05_06.h"
#include "recipe_05_07.h"

int main()
{
    std::cout << "[recipe_05_01]" << std::endl << std::endl;
    recipe_05_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_05_05]" << std::endl << std::endl;
    recipe_05_05::execute();
    std::cout << std::endl;

    std::cout << "[recipe_05_06]" << std::endl << std::endl;
    recipe_05_06::execute();
    std::cout << std::endl;

    std::cout << "[recipe_05_07]" << std::endl << std::endl;
    recipe_05_07::execute();
    std::cout << std::endl;

    return 0;
}
