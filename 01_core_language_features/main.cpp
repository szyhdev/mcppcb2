#include "recipe_01_01.h"
#include "recipe_01_02.h"

int main()
{
    std::cout << "[recipe_01_01]" << std::endl << std::endl;
    recipe_01_01::execute();
    std::cout << std::endl;

    std::cout << "[recipe_01_02]" << std::endl << std::endl;
    recipe_01_02::execute();
    std::cout << std::endl;

    return 0;
}
