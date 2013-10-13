#include <iostream>
#include "cvrp.hpp"

int main() {
     srand(time(NULL));
     for (unsigned j = 0; j < 20; ++j) {
         cvrp c("../VRPs/A-n39-k5.vrp");
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
