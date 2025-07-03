#include "zensep.h"
#include <vector>
#include <string>

int main() {
    zensep();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    zensep_print_vector(vec);
}
