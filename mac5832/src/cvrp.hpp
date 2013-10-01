#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#define _USE_MATH_DEFINES

using namespace std;

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
     vector<struct customer> _customers;
     unsigned _depot;
     vector<vector<unsigned>> _population;

     vector<pair<long, long>> rel_xy();
     vector<pair<double, unsigned>> polar_coords(vector<pair<long, long>>);

     void truckenize(vector<unsigned>&);
     void untruckenize(vector<unsigned>&);

     double dist(struct customer&, struct customer&);
     double fitness(vector<unsigned>&);

     void swap_genes(vector<unsigned>&, unsigned, unsigned);
     pair<vector<unsigned>, vector<unsigned>> pmx(vector<unsigned>&, vector<unsigned>&);
     vector<unsigned> random_swap(vector<unsigned>&);
     vector<unsigned> reverse(vector<unsigned>&);

     void create_population();

     void evolve();

     bool validate_solution(vector<unsigned>&);

public:
     cvrp(const string&);
     ~cvrp() {};
     void print_info();
     double best();

     void solve();
};
