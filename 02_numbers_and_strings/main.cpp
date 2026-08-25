#include "recipe_02_03.h"
#include "recipe_02_04.h"
#include "recipe_02_05.h"
#include "recipe_02_12.h"

int main()
{
    std::cout << "[recipe_02_03]" << std::endl << std::endl;
    recipe_02_03::execute();
    std::cout << std::endl;

    std::cout << "[recipe_02_04]" << std::endl << std::endl;
    recipe_02_04::execute();
    std::cout << std::endl;

    std::cout << "[recipe_02_05]" << std::endl << std::endl;
    recipe_02_05::execute();
    std::cout << std::endl;

    std::cout << "[recipe_02_12]" << std::endl << std::endl;
    recipe_02_12::execute();
    std::cout << std::endl;

    return 0;
}
