#include <iostream>
#include "cvrp.hpp"

using namespace std;

int main() {
     cvrp c("../VRPs/A-n32-k5.vrp");
     c.solve();
     c.print_info();
     return 0;
}
