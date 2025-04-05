#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <ctime>
#include <sstream>
#include <functional>

#include <chrono>
using namespace std;
using namespace std::chrono;

mt19937 *g = 0;
random_device rd;

// get nodes from file
vector<pair<double, double>> getNodes(string filepath)
{
    // init
    vector<pair<double, double>> nodes;

    // open file
    ifstream inputFile(filepath);
    if (!inputFile)
    {
        cerr << "Error: Could not open file " << filepath << endl;
        return nodes;
    }

    // Skip metadata lines until we reach "NODE_COORD_SECTION"
    string line;
    while (getline(inputFile, line))
    {
        if (line == "NODE_COORD_SECTION")
            break;
    }

    // Read the coordinates until "EOF" is encountered
    while (getline(inputFile, line))
    {
        if (line == "EOF")
            break;
        stringstream ss(line);
        double index, x, y;
        ss >> index >> x >> y;
        nodes.push_back(make_pair(x, y));
    }

    // end
    inputFile.close();
    return nodes;
}

// return some unique random intergers within range
vector<int> randints(int min, int max, int len)
{
    if (g == 0)
        g = new mt19937(rd());
    uniform_int_distribution<> dist(min, max);
    vector<int> nums;
    while (nums.size() < len)
    {
        int num = dist(*g);
        if (find(nums.begin(), nums.end(), num) == nums.end())
            nums.push_back(num);
    }
    return nums;
}

// get random values from a vector
vector<int> randints_fromvector(vector<int> v, int len)
{
    vector<int> indices = randints(0, v.size() - 1, len);
    vector<int> ans;
    for (int i : indices)
        ans.push_back(v[i]);
    return ans;
}

// Euclidean Distance
double euclideanDistance(pair<double, double> p1, pair<double, double> p2)
{
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

// Nearest Neighbour Heuristic (NNH)
vector<int> nearestNeighbour(const vector<pair<double, double>> &points)
{
    // init
    int n = points.size();
    vector<bool> visited(n, false);
    vector<int> tour;
    vector<int> indices;
    for (int i = 0; i < n; i++)
        indices.push_back(i);

    // insert first node
    shuffle(indices.begin(), indices.end(), *g);
    tour.push_back(indices.back());
    visited[indices.back()] = true;
    indices.pop_back();

    for (int i = 1; i < n; ++i)
    {
        int last = tour.back();
        int nearest = -1;
        int index = -1;
        double nearestDist = numeric_limits<double>::max();

        shuffle(indices.begin(), indices.end(), *g);
        for (int k = indices.size() - 1; k >= max(0, (int)indices.size() - 5); k--)
        {
            int j = indices[k];
            if (!visited[j])
            {
                double dist = euclideanDistance(points[last], points[j]);
                if (dist < nearestDist)
                {
                    nearest = j;
                    nearestDist = dist;
                    index = k;
                }
            }
        }
        tour.push_back(nearest);
        indices.erase(indices.begin() + index);
        visited[nearest] = true;
    }

    // Returning to the starting point
    tour.push_back(tour[0]);
    return tour;
}

// Cheapest Insertion Heuristic
vector<int> cheapestInsertion(const vector<pair<double, double>> &points)
{
    // init
    int n = points.size();
    vector<int> indices;
    vector<bool> inTour(n, false);
    vector<int> tour;
    for (int i = 0; i < n; i++)
        indices.push_back(i);

    // insert first two nodes
    shuffle(indices.begin(), indices.end(), *g);
    tour.push_back(indices.back());
    inTour[indices.back()] = true;
    indices.pop_back();
    tour.push_back(indices.back());
    inTour[indices.back()] = true;
    indices.pop_back();
    tour.push_back(tour.front());

    while (tour.size() < n + 1)
    {
        int bestNode = -1, bestPos = -1, index = -1;
        double bestCost = numeric_limits<double>::max();

        // Find the cheapest node to insert
        shuffle(indices.begin(), indices.end(), *g);
        for (int k = indices.size() - 1; k >= max(0, (int)indices.size() - 5); k--)
        {
            // if (!inTour[i])
            {
                int i = indices[k];
                for (int j = 0; j < tour.size() - 1; ++j)
                {
                    double cost = euclideanDistance(points[tour[j]], points[i]) +
                                  euclideanDistance(points[i], points[tour[j + 1]]) -
                                  euclideanDistance(points[tour[j]], points[tour[j + 1]]);

                    if (cost < bestCost)
                    {
                        bestNode = i;
                        bestPos = j + 1;
                        bestCost = cost;
                        index = k;
                    }
                }
            }
        }

        // Insert the best node at the best position
        tour.insert(tour.begin() + bestPos, bestNode);
        indices.erase(indices.begin() + index);
        inTour[bestNode] = true;
    }

    return tour;
}

// Farthest Insertion Heuristic
vector<int> farthestInsertion(const vector<pair<double, double>> &points)
{
    // init
    int n = points.size();
    vector<int> indices;
    vector<bool> inTour(n, false);
    // vector<int> tour = {0};
    vector<int> tour;
    for (int i = 0; i < n; i++)
        indices.push_back(i);

    // insert first node
    // inTour[0] = true;
    shuffle(indices.begin(), indices.end(), *g);
    tour.push_back(indices.back());
    inTour[indices.back()] = true;
    indices.pop_back();

    // Find the farthest point from the starting point
    int farthest = -1;
    double maxDist = -1.0;
    int index = -1;
    for (int k = indices.size() - 1; k >= max(0, (int)indices.size() - 5); k--)
    {
        int i = indices[k];
        double dist = euclideanDistance(points[0], points[i]);
        if (dist > maxDist)
        {
            maxDist = dist;
            farthest = i;
            index = k;
        }
    }

    tour.push_back(farthest);
    indices.erase(indices.begin() + index);
    inTour[farthest] = true;
    tour.push_back(tour[0]);

    while (tour.size() < n + 1)
    {
        int bestNode = -1, bestPos = -1;
        double maxMinDist = -1.0;
        int index = -1;

        for (int k = indices.size() - 1; k >= max(0, (int)indices.size() - 5); k--)
        {
            int i = indices[k];
            // if (!inTour[i])
            {
                double minDist = numeric_limits<double>::max();
                for (int j = 0; j < tour.size() - 1; ++j)
                {
                    double dist = euclideanDistance(points[tour[j]], points[i]);
                    if (dist < minDist)
                        minDist = dist;
                }
                if (minDist > maxMinDist)
                {
                    maxMinDist = minDist;
                    bestNode = i;
                    index = k;
                }
            }
        }

        // Find the best position to insert
        double bestCost = numeric_limits<double>::max();
        for (int j = 0; j < tour.size() - 1; ++j)
        {
            double cost = euclideanDistance(points[tour[j]], points[bestNode]) +
                          euclideanDistance(points[bestNode], points[tour[j + 1]]) -
                          euclideanDistance(points[tour[j]], points[tour[j + 1]]);
            if (cost < bestCost)
            {
                bestPos = j + 1;
                bestCost = cost;
            }
        }

        indices.erase(indices.begin() + index);
        tour.insert(tour.begin() + bestPos, bestNode);
        inTour[bestNode] = true;
    }

    return tour;
}

// 2-opt Optimization
void twoOptSwap(vector<int> &tour, int i, int k)
{
    while (i < k)
    {
        swap(tour[i], tour[k]);
        i++;
        k--;
    }
}

// 2-opt Optimization
void twoOpt(vector<int> &tour, const vector<pair<double, double>> &points)
{
    int n = tour.size();
    bool improved = true;

    while (improved)
    {
        improved = false;
        for (int i = 1; i < n - 2; ++i)
        {
            for (int k = i + 1; k < n - 1; ++k)
            {
                double oldDist = euclideanDistance(points[tour[i - 1]], points[tour[i]]) +
                                 euclideanDistance(points[tour[k]], points[tour[k + 1]]);
                double newDist = euclideanDistance(points[tour[i - 1]], points[tour[k]]) +
                                 euclideanDistance(points[tour[i]], points[tour[k + 1]]);
                if (newDist < oldDist)
                {
                    twoOptSwap(tour, i, k);
                    improved = true;
                }
            }
        }
    }
}

// Calculate the total distance of a given tour
double calculateTourDistance(const vector<int> &tour, const vector<pair<double, double>> &points)
{
    double totalDistance = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i)
    {
        totalDistance += euclideanDistance(points[tour[i]], points[tour[i + 1]]);
    }
    return totalDistance;
}

// Node Swap Optimization
void nodeSwap(vector<int> &tour, const vector<pair<double, double>> &points)
{
    int n = tour.size();
    bool improved = true;
    double oldDist = calculateTourDistance(tour, points); // Calculate initial tour distance

    while (improved)
    {
        improved = false;
        for (int i = 1; i < n - 2; ++i)
        {
            for (int j = i + 1; j < n - 1; ++j)
            {
                // Swap two cities and calculate new distance
                swap(tour[i], tour[j]);
                double newDist = calculateTourDistance(tour, points);

                // Check if the swap improves the tour
                if (newDist < oldDist)
                {
                    improved = true;
                    oldDist = newDist;
                }
                else
                {
                    // Revert the swap if no improvement
                    swap(tour[i], tour[j]);
                }
            }
        }
    }
}

// Node Shift Optimization
void nodeShift(vector<int> &tour, const vector<pair<double, double>> &points)
{
    int n = tour.size();
    bool improved = true;
    double oldDist = calculateTourDistance(tour, points); // Calculate initial tour distance

    while (improved)
    {
        improved = false;
        for (int i = 1; i < n - 1; ++i)
        {
            int city = tour[i];
            tour.erase(tour.begin() + i); // Temporarily remove the node

            int bestPos = -1;
            double bestGain = 0;

            for (int j = 1; j < n - 1; ++j)
            {
                tour.insert(tour.begin() + j, city);
                double newDist = calculateTourDistance(tour, points);

                // Calculate the gain from the shift
                double gain = oldDist - newDist;
                if (gain > bestGain)
                {
                    bestGain = gain;
                    bestPos = j;
                    improved = true;
                }
                tour.erase(tour.begin() + j); // Remove city from this position to try the next one
            }

            // Insert the city in the best found position
            if (bestPos != -1)
            {
                tour.insert(tour.begin() + bestPos, city);
                oldDist -= bestGain; // Update oldDist after finding a better position
            }
            else
            {
                tour.insert(tour.begin() + i, city); // Revert to original position
            }
        }
    }
}

// log
void logTourResultsToCSV(const vector<pair<double, double>> &points, const vector<int> &tour, const string &filename, const string &comment)
{
    auto start = high_resolution_clock::now();
    double totalDistance = calculateTourDistance(tour, points);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();

    ofstream csvFile;
    csvFile.open(filename, ios::app);
    if (csvFile.tellp() == 0)
        csvFile << "Total Distance,Runtime (microseconds),Comment,Tour\n";
    csvFile << totalDistance << "," << duration << "," << comment << "\n";
    csvFile.close();
}

tuple<vector<int>,long long,int> taskunit1(vector<pair<double,double>> points, function<vector<int>(const vector<pair<double, double>> &)> func, string filename, string comment){
    auto start = high_resolution_clock::now();
    vector<int> tour = func(points);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();
    double totalDistance = calculateTourDistance(tour, points);

    ofstream csvFile;
    csvFile.open(filename, ios::app);
    csvFile << totalDistance << "," << duration << "," << comment << ",";
    for(int i=0;i<tour.size()-1;i++) csvFile<<tour[i]<<"-";
    csvFile<<tour[tour.size()-1]<<"\n";
    csvFile.close();
    return make_tuple(tour,duration,totalDistance);
}
// function<void (vector<int> &tour, const vector<pair<double, double>>&)>
tuple<vector<int>,long long,int> taskunit2(vector<int> tour, vector<pair<double,double>> points, function<void (vector<int> &tour, const vector<pair<double, double>>&)> func, string filename, string comment, int adddur){
    auto start = high_resolution_clock::now();
    func(tour,points);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();
    double totalDistance = calculateTourDistance(tour, points);

    ofstream csvFile;
    csvFile.open(filename, ios::app);
    csvFile << totalDistance << "," << duration << "," << comment << ",";
    for(int i=0;i<tour.size()-1;i++) csvFile<<tour[i]<<"-";
    csvFile<<tour[tour.size()-1]<<"\n";
    csvFile.close();
    return make_tuple(tour,duration,totalDistance);
}

// Main function to run the heuristics and optimization
int main()
{
    g = new mt19937(rd());
    vector<string> datapathlist = {
        "resources/TSP_assignment_task_benchmark_data/a280.tsp",
        "resources/TSP_assignment_task_benchmark_data/berlin52.tsp",
        "resources/TSP_assignment_task_benchmark_data/bier127.tsp",
        "resources/TSP_assignment_task_benchmark_data/ch130.tsp",
        "resources/TSP_assignment_task_benchmark_data/ch150.tsp",
        "resources/TSP_assignment_task_benchmark_data/eil51.tsp",
        "resources/TSP_assignment_task_benchmark_data/eil76.tsp",
        "resources/TSP_assignment_task_benchmark_data/eil101.tsp",
        "resources/TSP_assignment_task_benchmark_data/kroA100.tsp",
        "resources/TSP_assignment_task_benchmark_data/kroB100.tsp",
        "resources/TSP_assignment_task_benchmark_data/kroC100.tsp",
        "resources/TSP_assignment_task_benchmark_data/kroD100.tsp",
        "resources/TSP_assignment_task_benchmark_data/kroE100.tsp",
        "resources/TSP_assignment_task_benchmark_data/lin105.tsp",
        "resources/TSP_assignment_task_benchmark_data/lin318.tsp",
        "resources/TSP_assignment_task_benchmark_data/pr76.tsp",
        "resources/TSP_assignment_task_benchmark_data/pr124.tsp",
        "resources/TSP_assignment_task_benchmark_data/pr144.tsp",
        "resources/TSP_assignment_task_benchmark_data/rat99.tsp",
        "resources/TSP_assignment_task_benchmark_data/rat195.tsp",
        "resources/TSP_assignment_task_benchmark_data/st70.tsp",
    };
    vector<function<vector<int>(const vector<pair<double, double>> &)>> functionList = {
        farthestInsertion,
        cheapestInsertion,
        nearestNeighbour,
    };
    string filename = "log.txt";
    ofstream csvFile;
    csvFile.open(filename, ios::app);
    csvFile << "Total Distance,Runtime (microseconds),Comment\n";
    csvFile.close();
    for (int i=0;i<datapathlist.size();i++)
    {
        string datapath = datapathlist[i];
        vector<pair<double, double>> points = getNodes(datapath);
        for (int j=0;j<functionList.size();j++)
        {
            auto funct = functionList[j];
            auto ret0 = taskunit1(points,funct,filename,to_string(i)+"-"+to_string(j)+"-0");

            auto ret1 = taskunit2(get<0>(ret0),points,twoOpt,filename,to_string(i)+"-"+to_string(j)+"-1",get<1>(ret0));
            auto ret2 = taskunit2(get<0>(ret0),points,nodeSwap,filename,to_string(i)+"-"+to_string(j)+"-2",get<1>(ret0));
            auto ret3 = taskunit2(get<0>(ret0),points,nodeShift,filename,to_string(i)+"-"+to_string(j)+"-3",get<1>(ret0));
            
            auto ret4 = taskunit2(get<0>(ret3),points,twoOpt,filename,to_string(i)+"-"+to_string(j)+"-4",get<1>(ret3));
            auto ret5 = taskunit2(get<0>(ret1),points,nodeShift,filename,to_string(i)+"-"+to_string(j)+"-5",get<1>(ret1));
            auto ret6 = taskunit2(get<0>(ret1),points,nodeSwap,filename,to_string(i)+"-"+to_string(j)+"-6",get<1>(ret1));
            auto ret7 = taskunit2(get<0>(ret2),points,twoOpt,filename,to_string(i)+"-"+to_string(j)+"-7",get<1>(ret2));
            auto ret8 = taskunit2(get<0>(ret2),points,nodeShift,filename,to_string(i)+"-"+to_string(j)+"-8",get<1>(ret2));
            auto ret9 = taskunit2(get<0>(ret3),points,nodeSwap,filename,to_string(i)+"-"+to_string(j)+"-9",get<1>(ret3));
            
            auto ret10 = taskunit2(get<0>(ret4),points,nodeSwap,filename,to_string(i)+"-"+to_string(j)+"-10",get<1>(ret4));
            auto ret11 = taskunit2(get<0>(ret5),points,nodeSwap,filename,to_string(i)+"-"+to_string(j)+"-11",get<1>(ret5));
            auto ret12 = taskunit2(get<0>(ret6),points,nodeShift,filename,to_string(i)+"-"+to_string(j)+"-12",get<1>(ret6));
            auto ret13 = taskunit2(get<0>(ret7),points,nodeShift,filename,to_string(i)+"-"+to_string(j)+"-13",get<1>(ret7));
            auto ret14 = taskunit2(get<0>(ret8),points,twoOpt,filename,to_string(i)+"-"+to_string(j)+"-14",get<1>(ret8));
            auto ret15 = taskunit2(get<0>(ret9),points,twoOpt,filename,to_string(i)+"-"+to_string(j)+"-15",get<1>(ret9));

            // break;
        }
        // break;
    }

    delete g;
    return 0;
}
