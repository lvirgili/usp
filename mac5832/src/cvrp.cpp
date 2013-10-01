#include "cvrp.hpp"

cvrp::cvrp(const string& file_name) {
     ifstream f(file_name);
     if (f.is_open() == false) {
          cout << "Error reading CVRP file named " << file_name << endl
                    << "Aborting...\n";
          exit(-1);
     } else {
          string line;
          while (getline(f, line)) {
               if (line.find("NAME") != string::npos ||
                   line.find("COMMENT") != string::npos ||
                   line.find("TYPE") != string::npos ||
                   line.find("EDGE_WEIGHT_TYPE") != string::npos) {
                    // Nothing to do here...
                    continue;
               }
               if (line.find("DIMENSION") != string::npos) {
                    stringstream ss(line);
                    string aux;
                    char ch;
                    ss >> aux >> ch >> this->_dimension;
                    continue;
               }
               if (line.find("CAPACITY") != string::npos) {
                    stringstream ss(line);
                    string aux;
                    char ch;
                    ss >> aux >> ch >> this->_capacity;
                    continue;
               }
               if (line.find("NODE_COORD_SECTION") != string::npos) {
                    for (long i = 0; i < this->_dimension; ++i) {
                         string numbers;
                         long id, x, y;
                         getline(f, numbers);
                         stringstream ss(numbers);
                         ss >> id >> x >> y;
                         customer c(x, y, 0);
                         _customers.push_back(c);
                    }
               }
               if (line.find("DEMAND_SECTION") != string::npos) {
                    for (long i = 0; i < this->_dimension; ++i) {
                         string numbers;
                         long id, demand;
                         getline(f, numbers);
                         stringstream ss(numbers);
                         ss >> id >> demand;
                         _customers[id - 1].demand = demand;
                    }
               }
               if (line.find("DEPOT_SECTION") != string::npos) {
                    string aux;
                    getline(f, aux);
                    stringstream ss(aux);
                    ss >> _depot;
                    --_depot;
                    return;
               }
          }
     }
}

void cvrp::print_info() {
     cout << "capacity: " << _capacity << endl;
     cout << "dimension: " << _dimension << endl;
     cout << "depot: " << _depot << endl;
     cout << "customers: " << endl;
     for (unsigned i = 0; i < _customers.size(); ++i) {
          cout << _customers[i].x << ' ' << _customers[i].y << ' ' << _customers[i].demand << endl;
     }
     cout << "Population:\n";
     for (unsigned i = 0; i < _population.size(); ++i) {
          cout << i << ": " << fitness(_population[i]) << endl << validate_solution(_population[i]) << endl;
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               cout << _population[i][j] << ' ';
          }
          cout << endl;
          truckenize(_population[i]);
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               cout << _population[i][j] << ' ';
          }
          cout << endl;
          untruckenize(_population[i]);
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               cout << _population[i][j] << ' ';
          }
          cout << endl;
     }
}

double cvrp::dist(customer& a, customer& b) {
     return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

void cvrp::truckenize(vector<unsigned>& gene) {
     if (gene[0] == 0) {
          return;
     }
     gene.push_back(0);
     gene.insert(gene.begin(), 0);
     unsigned i = 1;
     unsigned total_demand = 0;
     while (gene[i] != 0) {
          unsigned c = gene[i]; // customer index
          total_demand += _customers[c].demand;
          if (total_demand > _capacity) {
               gene.insert(gene.begin() + i, 0);
               total_demand = 0;
          }
          ++i;
     }
}

void cvrp::untruckenize(vector<unsigned>& gene) {
     if (gene[0] != 0) {
          return;
     }
     vector<unsigned>::iterator it = remove(gene.begin(), gene.end(), 0);
     gene.erase(it, gene.end());
}

vector<pair<long, long>> cvrp::rel_xy() {
     vector<pair<long, long>> rel(_customers.size());
     long x0 = _customers[0].x,
          y0 = _customers[0].y;
     for (unsigned i = 1; i < _customers.size(); ++i) {
          long xi = _customers[i].x - x0,
               yi = _customers[i].y - y0;
          rel[i] = make_pair(xi, yi);
     }
     return rel;
}

struct pred {
     bool operator() (const pair<double, long>& a, const pair<double, long>& b) {
          return a.first < b.first;
     }
};

vector<pair<double, unsigned>> cvrp::polar_coords(vector<pair<long, long>> xy_pairs) {
     vector<pair<double, unsigned>> polar;
     for (unsigned i = 1; i < xy_pairs.size(); ++i) {
          double theta;
          long x  = xy_pairs[i].first,
               y = xy_pairs[i].second;
          if (x > 0 && y > 0) {
               theta = atan(1.0 * y / x);
          } else if (x < 0) {
               theta = atan(1.0 * y / x) + M_PI;
          } else {
               if (x != 0) {
                    theta = atan(1.0 * y / x) + 2 * M_PI;
               } else if (y < 0) {
                    theta = 1.5 * M_PI;
               } else {
                    theta = 2.5 * M_PI;
               }
          }
          polar.push_back(make_pair(theta, i));
     }

     sort(polar.begin(), polar.end(), pred());
     return polar;
}

double cvrp::fitness(vector<unsigned>& gene) {
     vector<unsigned> copy(gene);
     truckenize(copy);
     double fit = 0;
     for (unsigned i = 0; i < copy.size() - 1; ++i) {
          struct customer a = _customers[copy[i]];
          struct customer b = _customers[copy[i + 1]];
          fit += dist(a, b);
     }
     return fit;
}

void cvrp::create_population() {
     vector<pair<long, long>> xy = rel_xy();
     vector<pair<double, unsigned>> thetas = polar_coords(xy);

     for (unsigned i = 0; i < thetas.size(); ++i) {
          vector<unsigned> gene;
          //gene.push_back(0);
          gene.push_back(thetas[i].second);
          for (unsigned j = i + 1; j < thetas.size(); ++j) {
               gene.push_back(thetas[j].second);
          }
          for (unsigned j = 0; j < i; ++j) {
               gene.push_back(thetas[j].second);
          }
          //gene.push_back(0);
          _population.push_back(gene);
     }
}

void cvrp::swap_genes(vector<unsigned>& c, unsigned g1, unsigned g2) {
     unsigned p_g1, p_g2;
     for (unsigned i = 0; i < c.size(); ++i) {
          if (c[i] == g1) {
               p_g1 = i;
          }
          if (c[i] == g2) {
               p_g2 = i;
          }
     }
     swap(c[p_g1], c[p_g2]);
}

pair<vector<unsigned>, vector<unsigned>>
cvrp::pmx(vector<unsigned>& mother, vector<unsigned>& father) {
     vector<unsigned> baby1(mother), baby2(father);
     unsigned begin = (unsigned)rand() % mother.size(),
          end = (unsigned)rand() % mother.size();
     if (begin > end) {
          swap(begin, end);
     }
     for (unsigned i = begin; i <= end; ++i) {
          unsigned g1 = mother[i],
               g2 = father[i];
          swap_genes(baby1, g1, g2);
          swap_genes(baby2, g1, g2);
     }
     return make_pair(baby1, baby2);
}

vector<unsigned> cvrp::random_swap(vector<unsigned>& parent) {
     vector<unsigned> child(parent);
     unsigned i = 0, j = 0;
     while (i == j) {
          i = rand() % parent.size();
          j = rand() % parent.size();
     }
     swap(child[i], child[j]);
     return child;
}

vector<unsigned> cvrp::reverse(vector<unsigned>& parent) {
     vector<unsigned> child(parent);
     unsigned beg = 0, end = 0;
     while (beg == end) {
          beg = rand() % child.size();
          end = rand() % child.size();
     }
     if (beg > end) {
          swap(beg, end);
     }
     unsigned j = end;
     for (unsigned i = beg; i <= end; ++i) {
          swap(child[i], child[j]);
          --j;
     }
     return child;
}

void cvrp::evolve() {
     vector<vector<unsigned>> new_population;
     //sort(_population.begin(), _population.end(), [this](vector<unsigned> a, vector<unsigned> b) { return fitness(a) <= fitness(b); });
     vector<vector<unsigned>>::iterator it = max_element(_population.begin(), _population.end(), [this](vector<unsigned> a, vector<unsigned> b) { return fitness(a) > fitness(b); });
     vector<unsigned> mother = *it;
      while (new_population.size() < _population.size()) {
          // unsigned mother = rand() % (_population.size());
          // unsigned father = rand() % (_population.size());
          // pair<vector<unsigned>, vector<unsigned>> babies = pmx(_population[mother], _population[father]);
          // new_population.push_back(babies.first);
          // new_population.push_back(babies.second);
//           unsigned mother = rand() % (_population.size() / 2);
           //vector<unsigned> child = random_swap(mother);
           vector<unsigned> child = reverse(mother);
           new_population.push_back(child);
      }
     _population = new_population;
}

double cvrp::best() {
     //sort(_population.begin(), _population.end(), [this](vector<unsigned> a, vector<unsigned> b) { return fitness(a) <= fitness(b); });
     return fitness(*max_element(_population.begin(), _population.end(),
                                [this](vector<unsigned> a, vector<unsigned> b) { return fitness(a) > fitness(b); }));
}

void cvrp::solve() {
     create_population();
     //print_info();
     for (int i = 0; i < 1000; ++i) {
          evolve();
     }

     //pair<vector<unsigned>, vector<unsigned>> children = pmx(_population[0], _population[1]);
     // for (unsigned i = 0; i < _population[0].size(); ++i) {
     //      cout << _population[0][i] << ' ';
     // }
     // cout << endl;
     // for (unsigned i = 0; i < _population[1].size(); ++i) {
     //      cout << _population[1][i] << ' ';
     // }
     // cout << endl;
     // for (unsigned i = 0; i < children.first.size(); ++i) {
     //      cout << children.first[i] << ' ';
     // }
     // cout << endl;
     // for (unsigned i = 0; i < children.second.size(); ++i) {
     //      cout << children.second[i] << ' ';
     // }
     // cout << endl;

}

bool cvrp::validate_solution(vector<unsigned>& gene) {
     untruckenize(gene);
     vector<unsigned> s(_dimension, 0);
     for (unsigned i = 0; i < gene.size(); ++i) {
          ++s[gene[i]];
     }
     for (unsigned i = 1; i < s.size(); ++i) {
          if (s[i] != 1) {
               return false;
          }
     }
     return true;
}
