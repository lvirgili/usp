#include <iostream>
#include "cvrp.hpp"

int main() {
     srand(time(NULL));
     //cvrp c("../VRPs/small.vrp");
     for (unsigned j = 0; j < 30; ++j) {
          cvrp c("../VRPs/A-n32-k5.vrp");
          double min, best = 10000;
          for (unsigned i = 0; i < 10; ++i) {
               min = 100000;
               c.solve();
               double fit = c.best();
               if (fit < min) {
                    min = fit;
                    //cout << i << ": " << min << endl;
               }
               if (fit < best) {
                    best = fit;
                    //cout << i << ": " << best << endl;
               }
          }
          cout << best << endl;
     }
     return 0;
}
