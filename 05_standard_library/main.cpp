#include "../include/features.h"
#include "recipe_05_01.h"
#include "recipe_05_05.h"

int main()
{
    std::cout << "[recipe_05_01]" << std::endl << std::endl;
    recipe_05_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_05_05]" << std::endl << std::endl;
    recipe_05_05::execute();
    std::cout << std::endl;

    return 0;
}
