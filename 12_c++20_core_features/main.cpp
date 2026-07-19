#include "../include/features.h"
#include "recipe_12_05.h"
#include "recipe_12_06.h"
#include "recipe_12_07.h"
#include "recipe_12_07_k07.h"
#include "recipe_12_08.h"

int main()
{
    std::cout << "[recipe_12_05]" << std::endl << std::endl;
    recipe_12_05::execute();
    std::cout << std::endl;

    std::cout << "[recipe_12_06]" << std::endl << std::endl;
    recipe_12_06::execute();
    std::cout << std::endl;

    std::cout << "[recipe_12_07]" << std::endl << std::endl;
    recipe_12_07::execute();
    std::cout << std::endl;

    std::cout << "[recipe_12_07_k07]" << std::endl << std::endl;
    recipe_12_07_k07::execute();
    std::cout << std::endl;

    std::cout << "[recipe_12_08]" << std::endl << std::endl;
    recipe_12_08::execute();
    std::cout << std::endl;

    return 0;
}
