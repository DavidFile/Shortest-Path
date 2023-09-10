#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <sstream>
#include <algorithm>

class City {
public:
  int id;
  std::string code;
  std::string name;
  int population;
  int elevation;

  City() : id(0), population(0), elevation(0) {}  // Default constructor                                                                                                                                    

  City(int _id, const std::string& _code, const std::string& _name, int _population, int _elevation)
    : id(_id), code(_code), name(_name), population(_population), elevation(_elevation) {}
};

class Graph {
private:
  std::vector<std::vector<std::pair<int, int>>> adjList;  // pair<destination, weight>                                                                                                                      
  std::unordered_map<std::string, City> cities;  // City code to City map                                                                                                                                   

public:
  Graph(int size) {
    adjList.resize(size);
  }

  void addEdge(int src, int dest, int weight) {
    adjList[src].push_back({dest, weight});
  }

  void addCity(const City& city) {
    cities[city.code] = city;
  }

  City getCityByCode(const std::string &code) const {
    return cities.at(code);
  }

  City getCityById(int id) const {
    for (const auto &entry : cities) {
      if (entry.second.id == id) {
        return entry.second;
      }
    }
    throw std::runtime_error("City ID not found");
  }
  std::pair<int, std::vector<int>> shortestPath(int src, int dest) {
    std::vector<int> distance(adjList.size(), INT_MAX);
    std::vector<int> previous(adjList.size(), -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    distance[src] = 0;
    pq.push({0, src});

while (!pq.empty()) {
      int curr = pq.top().second;
      pq.pop();

      for (auto &edge : adjList[curr]) {
        int next = edge.first;
        int weight = edge.second;

        if (distance[curr] + weight < distance[next]) {
          distance[next] = distance[curr] + weight;
          pq.push({distance[next], next});
          previous[next] = curr;
        }
      }
    }

    std::vector<int> path;
    for (int v = dest; v != -1; v = previous[v])
      path.push_back(v);

    reverse(path.begin(), path.end());

    return {distance[dest], path};
  }
};

int main(int argc, char *argv[]) {
  if(argc != 3) {
    std::cerr << "Usage: ./prog8 <from_city_code> <to_city_code>" << std::endl;
    return 1;
  }

  std::string fromCityCode = argv[1];
  std::string toCityCode = argv[2];

  Graph graph(20);

  std::ifstream cityFile("city.txt");
  std::string line;
  while(getline(cityFile, line)) {
    std::istringstream ss(line);
    int id, population, elevation;
    std::string code, name;
    ss >> id >> code >> name >> population >> elevation;
    graph.addCity(City(id, code, name, population, elevation));
  }

  std::ifstream roadFile("road.txt");
  while(getline(roadFile, line)) {
    std::istringstream ss(line);
    int src, dest, weight;
    ss >> src >> dest >> weight;
    graph.addEdge(src, dest, weight);
  }
City fromCity = graph.getCityByCode(fromCityCode);
  City toCity = graph.getCityByCode(toCityCode);

  std::cout << "From City: " << fromCity.name << ", population " << fromCity.population << ", elevation " << fromCity.elevation << std::endl;
  std::cout << "To City: " << toCity.name << ", population " << toCity.population << ", elevation " << toCity.elevation << std::endl;

  auto result = graph.shortestPath(fromCity.id, toCity.id);
  if(result.second.size() == 1 && result.second[0] == toCity.id) {
    std::cout << "No route from " << fromCity.name << " to " << toCity.name << std::endl;
  } else {
    std::cout << "The shortest distance from " << fromCity.name << " to " << toCity.name << " is " << result.first << std::endl;
    std::cout << "through the route: ";
    for(size_t i = 0; i < result.second.size(); ++i) {
      // Using getCityById here instead of getCityByCode                                                                                                                                                    
      std::cout << graph.getCityById(result.second[i]).name;
      if(i != result.second.size() - 1) {
        std::cout << "->";
      }
    }
    std::cout << std::endl;

    return 0;
  }
}
