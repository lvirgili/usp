#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <algorithm>
#include <set>
#define _USE_MATH_DEFINES

struct customer {
     long x, y, demand;

     customer() : x(0), y(0), demand(0) {};

     customer(long x, long y, long demand) {
          this->x = x;
          this->y = y;
          this->demand = demand;
     };
};

class cvrp {
private:
     unsigned _capacity, _dimension;
     std::vector<struct customer> _customers;
     unsigned _depot;
     std::vector<std::vector<unsigned>> _population;

     std::vector<std::pair<long, long>> rel_xy();
     std::vector<std::pair<double, unsigned>> polar_coords(std::vector<std::pair<long, long>>);

     void truckenize(std::vector<unsigned>&);
     void untruckenize(std::vector<unsigned>&);

     double dist(struct customer&, struct customer&);
     double fitness(std::vector<unsigned>&);

     void create_population();

     bool validate_solution(std::vector<unsigned>&);

public:
     cvrp(const std::string&);
     ~cvrp() {};
     void print_info();

     void solve();
};
