#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <set>
#include <cmath>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

random_device rd;
mt19937 *gen;

struct CarData {
    string buying_price;
    string maint_price;
    string doors;
    string persons;
    string lug_boot;
    string safety;
    string class_label;
};

// Function to read the dataset from a CSV file
vector<CarData> read_csv(const string& filename) {
    vector<CarData> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        CarData car_data;

        getline(ss, car_data.buying_price, ',');
        getline(ss, car_data.maint_price, ',');
        getline(ss, car_data.doors, ',');
        getline(ss, car_data.persons, ',');
        getline(ss, car_data.lug_boot, ',');
        getline(ss, car_data.safety, ',');
        getline(ss, car_data.class_label);

        data.push_back(car_data);
    }

    return data;
}

// Decision Tree Helper Functions
string pluralityValue(const vector<CarData>& examples) {
    map<string, int> labelCount;
    for (const auto& e : examples) {
        labelCount[e.class_label]++;
    }
    return max_element(labelCount.begin(), labelCount.end(),
                       [](const pair<string, int>& a, const pair<string, int>& b) {
                           return a.second < b.second;
                       })->first;
}

bool allExamplesSameClass(const vector<CarData>& examples) {
    string firstLabel = examples[0].class_label;
    for (const auto& e : examples) {
        if (e.class_label != firstLabel) return false;
    }
    return true;
}

double calculateEntropy(const vector<CarData>& examples) {
    map<string, int> labelCount;
    for (const auto& e : examples) {
        labelCount[e.class_label]++;
    }

    double entropy = 0.0;
    int total = examples.size();
    for (const auto& pair : labelCount) {
        double p = (double)pair.second / total;
        entropy -= p * log2(p);
    }
    return entropy;
}

double calculateGini(const vector<CarData>& examples) {
    map<string, int> labelCount;
    for (const auto& e : examples) {
        labelCount[e.class_label]++;
    }

    double gini = 1.0;
    int total = examples.size();
    for (const auto& pair : labelCount) {
        double p = (double)pair.second / total;
        gini -= p * p;
    }
    return gini;
}

vector<CarData> filterExamples(const vector<CarData>& examples, const string& attribute, const string& value) {
    vector<CarData> filtered;
    for (const auto& e : examples) {
        if ((attribute == "buying_price" && e.buying_price == value) ||
            (attribute == "maint_price" && e.maint_price == value) ||
            (attribute == "doors" && e.doors == value) ||
            (attribute == "persons" && e.persons == value) ||
            (attribute == "lug_boot" && e.lug_boot == value) ||
            (attribute == "safety" && e.safety == value)) {
            filtered.push_back(e);
        }
    }
    return filtered;
}


double importance(const string& attribute, const vector<CarData>& examples, const string& method) {
    set<string> uniqueValues;
    for (const auto& e : examples) {
        if (attribute == "buying_price") uniqueValues.insert(e.buying_price);
        else if (attribute == "maint_price") uniqueValues.insert(e.maint_price);
        else if (attribute == "lug_boot") uniqueValues.insert(e.lug_boot);
        else if (attribute == "safety") uniqueValues.insert(e.safety);
        else if (attribute == "doors") uniqueValues.insert(e.doors);
        else if (attribute == "persons") uniqueValues.insert(e.persons);
    }

    double total = examples.size();
    double score = (method == "information_gain") ? calculateEntropy(examples) : 0.0;

    for (const auto& value : uniqueValues) {
        vector<CarData> subset = filterExamples(examples, attribute, value);
        double weight = (double)subset.size() / total;

        if (method == "information_gain") {
            score -= weight * calculateEntropy(subset);
        } else if (method == "gini") {
            score += weight * calculateGini(subset);
        }
    }

    return (method == "information_gain") ? score : -score;
}

string randomBestAttribute(const vector<string>& attributes, const vector<CarData>& examples, const string& method, bool best) {
    vector<pair<string, double>> scores;

    for (const string& attr : attributes) {
        scores.push_back({attr, importance(attr, examples, method)});
    }

    sort(scores.begin(), scores.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });

    vector<string> top3;
    for (int i = 0; i < min(3, (int)scores.size()); i++) {
        top3.push_back(scores[i].first);
    }

    if(best)
        return top3[0];

    // cout<<top3.size()<<endl;
    uniform_int_distribution<> dist(0, top3.size() - 1);
    int x = dist(*gen);
    return top3[x];
}

struct TreeNode {
    string attribute;
    string label;
    map<string, TreeNode*> children;

    TreeNode(string attr = "", string lbl = "") : attribute(attr), label(lbl) {}
};

TreeNode* decisionTreeLearning(
    vector<CarData> examples, 
    vector<string> attributes, 
    vector<CarData> parentExamples, 
    const string& method, 
    const map<string, vector<string>>& attribute_val, 
    bool best = true
) {
    if (examples.empty()) {
        return new TreeNode("", pluralityValue(parentExamples));
    }
    if (allExamplesSameClass(examples)) {
        return new TreeNode("", examples[0].class_label);
    }
    if (attributes.empty()) {
        return new TreeNode("", pluralityValue(examples));
    }

    string A = randomBestAttribute(attributes, examples, method, best);
    TreeNode* tree = new TreeNode(A, "");

    vector<string> remainingAttributes = attributes;
    remainingAttributes.erase(remove(remainingAttributes.begin(), remainingAttributes.end(), A), remainingAttributes.end());

    const vector<string>& possibleValues = attribute_val.at(A); // Get all possible values for the attribute

    for (const string& v : possibleValues) {
        vector<CarData> exs = filterExamples(examples, A, v);
        TreeNode* subtree;

        if (exs.empty()) {
            // No examples for this value; use plurality value as default
            subtree = new TreeNode("", pluralityValue(examples));
        } else {
            subtree = decisionTreeLearning(exs, remainingAttributes, examples, method, attribute_val, best);
        }

        tree->children[v] = subtree;
    }

    return tree;
}


double testTree(TreeNode* tree, const vector<CarData>& test_data) {
    int correct = 0;

    for (const auto& data : test_data) {
        TreeNode* current = tree;
        string predicted_label = "Unknown";

        // Traverse the tree to predict the label
        while (current && current->label.empty()) {
            string value;
            if (current->attribute == "buying_price") value = data.buying_price;
            else if (current->attribute == "maint_price") value = data.maint_price;
            else if (current->attribute == "lug_boot") value = data.lug_boot;
            else if (current->attribute == "safety") value = data.safety;
            else if (current->attribute == "doors") value = data.doors;
            else if (current->attribute == "persons") value = data.persons;

            if (current->children.find(value) != current->children.end()) {
                current = current->children[value];
            } else {
                // Handle unseen values or no matching child nodes
                current = nullptr;
                break;
            }
        }

        if (current) {
            predicted_label = current->label; // Assign the predicted label
        }
        else {
            predicted_label = pluralityValue(test_data); // Use plurality class as default
        }


        // Print actual and predicted labels
        // if(data.class_label!=predicted_label)
        // cout << data.class_label << ", " << predicted_label << "\n";

        // Check if the prediction is correct
        if (predicted_label == data.class_label) {
            correct++;
        }
    }

    // Return accuracy as a percentage
    return (double)correct / test_data.size();
}



void deleteTree(TreeNode* root) {
    if (!root) return;
    for (auto& child : root->children) {
        deleteTree(child.second);
    }
    delete root;
}


double taskUnit(
    vector<CarData> train_data, 
    vector<CarData> test_data, 
    vector<string> attributes, 
    string method, 
    const map<string, vector<string>>& attribute_val, 
    bool best = true
) {
    TreeNode* tree = decisionTreeLearning(train_data, attributes, {}, method, attribute_val, best);
    double accuracy = testTree(tree, test_data);

    deleteTree(tree); // Clean up memory
    return accuracy;
}


double testTreeWithLog(TreeNode* tree, const vector<CarData>& test_data, const string& heuristic, bool best, int index, ofstream& logFile, double trainTime) {
    int correct = 0;
    auto testStart = high_resolution_clock::now();

    for (const auto& data : test_data) {
        TreeNode* current = tree;
        string predicted_label = "Unknown";

        while (current && current->label.empty()) {
            string value;
            if (current->attribute == "buying_price") value = data.buying_price;
            else if (current->attribute == "maint_price") value = data.maint_price;
            else if (current->attribute == "lug_boot") value = data.lug_boot;
            else if (current->attribute == "safety") value = data.safety;
            else if (current->attribute == "doors") value = data.doors;
            else if (current->attribute == "persons") value = data.persons;

            if (current->children.find(value) != current->children.end()) {
                current = current->children[value];
            } else {
                current = nullptr;
                break;
            }
        }

        if (current) {
            predicted_label = current->label;
        } else {
            predicted_label = pluralityValue(test_data);
        }

        if (predicted_label == data.class_label) {
            correct++;
        }

        logFile << heuristic << "," << (best ? "best" : "top3") << "," << index << "," 
                << trainTime << "," << duration<double>(high_resolution_clock::now() - testStart).count() << ","
                << data.class_label << "," << predicted_label << endl;
    }

    return (double)correct / test_data.size();
}

// Function to recursively print the tree to a file in a readable format
void printTreeToFile(TreeNode* node, ofstream& file, int depth = 0) {
    if (!node) return;

    // Print indentation for the current depth
    file << string(depth * 4, ' ');

    if (!node->label.empty()) {
        // If the node is a leaf, print the label
        file << "Leaf: " << node->label << "\n";
    } else {
        // If the node is not a leaf, print the attribute and recurse for children
        file << "Attribute: " << node->attribute << "\n";
        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            file << string(depth * 4, ' ') << "  Value: " << it->first << "\n";
            printTreeToFile(it->second, file, depth + 1);
        }
    }
}

double taskUnitWithLog(
    vector<CarData> train_data,
    vector<CarData> test_data,
    vector<string> attributes,
    string method,
    const map<string, vector<string>>& attribute_val,
    bool best,
    int index,
    ofstream& logFile
) {
    auto trainStart = high_resolution_clock::now();
    TreeNode* tree = decisionTreeLearning(train_data, attributes, {}, method, attribute_val, best);
    auto trainEnd = high_resolution_clock::now();

    double trainTime = duration<double>(trainEnd - trainStart).count();
    double accuracy = testTreeWithLog(tree, test_data, method, best, index, logFile, trainTime);

    ofstream treeFile("tree/tree_" + method + "_run" + to_string(index) + ".txt");
    printTreeToFile(tree, treeFile);
    treeFile.close();

    deleteTree(tree);
    return accuracy;
}


int main(int argc, char* argv[]) {
    gen = new mt19937(rd());
    ofstream logFile("log.csv");
    logFile << "heuristic,bestortop3,index,traintime,testtime,truevalue,predictedvalue" << endl;

    vector<CarData> carData = read_csv("car evaluation dataset/car.data");
    vector<string> methods = {"information_gain", "gini"};
    vector<string> attributes = {"buying_price", "maint_price", "doors", "persons", "lug_boot", "safety"};
    map<string, vector<string>> attribute_val = {
        {"buying_price", {"vhigh", "high", "med", "low"}},
        {"maint_price", {"vhigh", "high", "med", "low"}},
        {"doors", {"2", "3", "4", "5more"}},
        {"persons", {"2", "4", "more"}},
        {"lug_boot", {"small", "med", "big"}},
        {"safety", {"low", "med", "high"}}
    };
    double a=0,b=0,c=0,d=0,e,f;

    for (int i = 0; i < 20; i++) {
        shuffle(carData.begin(), carData.end(), *gen);
        size_t train_size = (carData.size() * 8) / 10;
        vector<CarData> train_data(carData.begin(), carData.begin() + train_size);
        vector<CarData> test_data(carData.begin() + train_size, carData.end());

        

        for (const string& method : methods) {
            e=taskUnitWithLog(train_data, test_data, attributes, method, attribute_val, true, i, logFile);
            f=taskUnitWithLog(train_data, test_data, attributes, method, attribute_val, false, i, logFile);
            if(method=="gini"){
                c+=e;d+=f;
            }
            else{
                a+=e;b+=f;
            }
        }
    }
    cout<<a/20<<" "<<b/20<<" "<<c/20<<" "<<d/20<<endl;

    logFile.close();
    cout << "Log file generated: log.csv" << endl;
    return 0;
}
