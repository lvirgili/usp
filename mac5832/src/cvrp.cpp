#include "cvrp.hpp"

cvrp::cvrp(const std::string& file_name) {
     std::ifstream f(file_name);
     if (f.is_open() == false) {
          std::cout << "Error reading CVRP file named " << file_name << std::endl
                    << "Aborting...\n";
          exit(-1);
     } else {
          std::string line;
          while (getline(f, line)) {
               if (line.find("NAME") != std::string::npos ||
                   line.find("COMMENT") != std::string::npos ||
                   line.find("TYPE") != std::string::npos ||
                   line.find("EDGE_WEIGHT_TYPE") != std::string::npos) {
                    // Nothing to do here...
                    continue;
               }
               if (line.find("DIMENSION") != std::string::npos) {
                    std::stringstream ss(line);
                    std::string aux;
                    char ch;
                    ss >> aux >> ch >> this->_dimension;
                    continue;
               }
               if (line.find("CAPACITY") != std::string::npos) {
                    std::stringstream ss(line);
                    std::string aux;
                    char ch;
                    ss >> aux >> ch >> this->_capacity;
                    continue;
               }
               if (line.find("NODE_COORD_SECTION") != std::string::npos) {
                    for (long i = 0; i < this->_dimension; ++i) {
                         std::string numbers;
                         long id, x, y;
                         getline(f, numbers);
                         std::stringstream ss(numbers);
                         ss >> id >> x >> y;
                         customer c(x, y, 0);
                         _customers.push_back(c);
                    }
               }
               if (line.find("DEMAND_SECTION") != std::string::npos) {
                    for (long i = 0; i < this->_dimension; ++i) {
                         std::string numbers;
                         long id, demand;
                         getline(f, numbers);
                         std::stringstream ss(numbers);
                         ss >> id >> demand;
                         _customers[id - 1].demand = demand;
                    }
               }
               if (line.find("DEPOT_SECTION") != std::string::npos) {
                    std::string aux;
                    getline(f, aux);
                    std::stringstream ss(aux);
                    ss >> _depot;
                    --_depot;
                    return;
               }
          }
     }
}

void cvrp::print_info() {
     std::cout << "capacity: " << _capacity << std::endl;
     std::cout << "dimension: " << _dimension << std::endl;
     std::cout << "depot: " << _depot << std::endl;
     std::cout << "customers: " << std::endl;
     for (unsigned i = 0; i < _customers.size(); ++i) {
          std::cout << _customers[i].x << ' ' << _customers[i].y << ' ' << _customers[i].demand << std::endl;
     }
     std::cout << "Population:\n";
     for (unsigned i = 0; i < _population.size(); ++i) {
          std::cout << i << ": " << fitness(_population[i]) << std::endl << validate_solution(_population[i]) << std::endl;
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               std::cout << _population[i][j] << ' ';
          }
          std::cout << std::endl;
          truckenize(_population[i]);
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               std::cout << _population[i][j] << ' ';
          }
          std::cout << std::endl;
          untruckenize(_population[i]);
          for (unsigned j = 0; j < _population[i].size(); ++j) {
               std::cout << _population[i][j] << ' ';
          }
          std::cout << std::endl;
     }
}

double cvrp::dist(customer& a, customer& b) {
     return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

void cvrp::truckenize(std::vector<unsigned>& gene) {
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

void cvrp::untruckenize(std::vector<unsigned>& gene) {
     if (gene[0] != 0) {
          return;
     }
     std::vector<unsigned>::iterator it = std::remove(gene.begin(), gene.end(), 0);
     gene.erase(it, gene.end());
}

std::vector<std::pair<long, long>> cvrp::rel_xy() {
     std::vector<std::pair<long, long>> rel(_customers.size());
     long x0 = _customers[0].x,
          y0 = _customers[0].y;
     for (unsigned i = 1; i < _customers.size(); ++i) {
          long xi = _customers[i].x - x0,
               yi = _customers[i].y - y0;
          rel[i] = std::make_pair(xi, yi);
     }
     return rel;
}

struct pred {
     bool operator() (const std::pair<double, long>& a, const std::pair<double, long>& b) {
          return a.first < b.first;
     }
};

std::vector<std::pair<double, unsigned>> cvrp::polar_coords(std::vector<std::pair<long, long>> xy_pairs) {
     std::vector<std::pair<double, unsigned>> polar;
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
          polar.push_back(std::make_pair(theta, i));
     }

     std::sort(polar.begin(), polar.end(), pred());
     return polar;
}

double cvrp::fitness(std::vector<unsigned>& gene) {
     truckenize(gene);
     double fit = 0;
     for (unsigned i = 0; i < gene.size() - 1; ++i) {
          struct customer a = _customers[gene[i]];
          struct customer b = _customers[gene[i + 1]];
          fit += dist(a, b);
     }
     untruckenize(gene);
     return fit;
}

void cvrp::create_population() {
     std::vector<std::pair<long, long>> xy = rel_xy();
     std::vector<std::pair<double, unsigned>> thetas = polar_coords(xy);

     for (unsigned i = 0; i < thetas.size(); ++i) {
          std::vector<unsigned> gene;
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

void cvrp::solve() {
     create_population();
}

bool cvrp::validate_solution(std::vector<unsigned>& gene) {
     untruckenize(gene);
     std::vector<unsigned> s(_dimension, 0);
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
