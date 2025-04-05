#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <string>
using namespace std;
typedef vector<vector<int>> v2d;
#define ASTERISK '*'

int hamming_distance(v2d board)
{
    int board_size = board.size();
    int ans = 0;
    for (int i = 0; i < board_size; i++)
        for (int j = 0; j < board_size; j++)
            if ((i != board_size - 1 || j != board_size - 1) && board[i][j] != i * board_size + j + 1)
                ans++;
    return ans;
}

int manhattan_distance(v2d board)
{
    int board_size = board.size();
    int ans = 0;
    for (int i = 0; i < board_size; i++)
        for (int j = 0; j < board_size; j++)
            if (board[i][j] != ASTERISK && board[i][j] != i * board_size + j + 1)
                ans += abs((board[i][j] - 1) / board_size - i) + abs((board[i][j] - 1) % board_size - j);
    return ans;
}

int print_v2d(v2d board)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board.size(); j++)
        {
            if (board[i][j]==ASTERISK)
                cout << '*' << " ";
            else
                cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

struct Node
{
    v2d board;
    int weight;
    int h_weight;
    Node *prev;

    Node(v2d board, int weight, int h_weight, Node *prev = nullptr)
    {
        this->board = board;
        this->weight = weight;
        this->h_weight = h_weight;
        this->prev = prev;
    }
};

struct Results
{
    int explored_count;
    int expanded_count;
    vector<v2d> steps;
    Results(int explored_count, int expanded_count, vector<v2d> steps)
    {
        this->explored_count = explored_count;
        this->expanded_count = expanded_count;
        this->steps = steps;
    }
};

struct node_comparator
{
    bool operator()(Node *&a, Node *&b) const
    {
        return a->h_weight > b->h_weight;
    }
};

struct v2d_hash {
    unsigned long long operator() (const v2d& board) const {
        unsigned long long hash = 0;
        for(int i=0;i<board.size();i++)
            for(int j=0;j<board.size();j++)
                hash = hash * 31 + board[i][j];
        return hash;
    }
};

void explore_node(Node *cur, int i_star, int j_star, int i_star_, int j_star_, int (*heuristic_func)(v2d), priority_queue<Node *, vector<Node *>, node_comparator> &pq, unordered_set<v2d, v2d_hash> &expanded_list)
{
    v2d newboard = cur->board;
    swap(newboard[i_star][j_star], newboard[i_star_][j_star_]);
    bool found = expanded_list.find(newboard)!=expanded_list.end();
    if (!found)
    {
        Node *newnode = new Node(newboard, cur->weight + 1, cur->weight + 1 + heuristic_func(newboard), cur);
        pq.push(newnode);
    }
}

Results n_puzzle_solver(int board_size, v2d board, int (*heuristic_func)(v2d))
{
    // create goal board
    v2d goal(board_size, vector<int>(board_size));
    for (int i = 0; i < board_size * board_size - 1; i++)
        goal[i / board_size][i % board_size] = i + 1;
    goal[board_size - 1][board_size - 1] = ASTERISK;

    // init
    priority_queue<Node *, vector<Node *>, node_comparator> pq;
    unordered_set<v2d, v2d_hash> expanded_list;
    Node *init_node = new Node(board, 0, 0);
    vector<Node*> expanded_nodes;
    pq.push(init_node);
    while (!pq.empty())
    {
        Node *cur = pq.top();
        pq.pop();

        bool found = expanded_list.find(cur->board)!=expanded_list.end();
        if (found)
            continue;

        expanded_list.insert(cur->board);

        if (cur->board == goal)
        {
            vector<v2d> result;
            Node *temp = cur;
            while (temp)
            {
                result.push_back(temp->board);
                temp = temp->prev;
            }
            int expanded_count = expanded_list.size();
            while (pq.size())
            {
                Node *temp = pq.top();
                pq.pop();
                expanded_list.insert(temp->board);
                delete temp;
            }
            for(int i=0;i<expanded_nodes.size();i++)
                delete expanded_nodes[i];
            int explored_count = expanded_list.size();

            Results results(explored_count, expanded_count, result);
            return results;
        }

        // find star position
        int i_star, j_star;
        bool loop = true;
        for (i_star = 0; loop && i_star < board_size; i_star++)
        {
            for (j_star = 0; loop && j_star < board_size; j_star++)
                if (cur->board[i_star][j_star] == ASTERISK)
                {
                    loop = false;
                    break;
                }
            if (!loop)
                break;
        }

        // explore
        if (i_star != 0)
            explore_node(cur, i_star, j_star, i_star - 1, j_star, heuristic_func, pq, expanded_list);
        if (j_star != 0)
            explore_node(cur, i_star, j_star, i_star, j_star - 1, heuristic_func, pq, expanded_list);
        if (i_star != board_size - 1)
            explore_node(cur, i_star, j_star, i_star + 1, j_star, heuristic_func, pq, expanded_list);
        if (j_star != board_size - 1)
            explore_node(cur, i_star, j_star, i_star, j_star + 1, heuristic_func, pq, expanded_list);
        expanded_nodes.push_back(cur);
    }
}

bool n_puzzle_solvable_finder(v2d board)
{
    vector<int> flattened;
    int i_star;
    int inv = 0;
    bool solvable;
    for (int i = 0; i < board.size(); i++)
        for (int j = 0; j < board.size(); j++)
            if (board[i][j] == ASTERISK)
                i_star = i;
            else
                flattened.push_back(board[i][j]);
    for (int i = 0; i < flattened.size(); i++)
        for (int j = i + 1; j < flattened.size(); j++)
            if (flattened[i] > flattened[j])
                inv++;

    if (board.size() % 2 == 1)
        solvable = inv % 2 == 0;
    else
    {
        int row_from_bottom = board.size() - i_star;
        solvable = (row_from_bottom % 2 == 0 && inv % 2 == 1) || (row_from_bottom % 2 == 1 && inv % 2 == 0);
    }
    return solvable;
}

int main(void)
{
    int board_size;
    cin >> board_size;
    v2d board(board_size, vector<int>(board_size, 0));
    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            string k;
            cin >> k;
            if (k == "*")
                board[i][j] = ASTERISK;
            else
                board[i][j] = stoi(k);
        }
    }

    bool solvable = n_puzzle_solvable_finder(board);
    cout << (solvable ? "solvable" : "not solvable") << endl;

    if (solvable)
    {
        Results r1 = n_puzzle_solver(board.size(), board, hamming_distance);
        Results r2 = n_puzzle_solver(board.size(), board, manhattan_distance);

        cout << "Solution" << endl;
        for (int i = r1.steps.size() - 1; i >= 0; i--)
            print_v2d(r1.steps[i]);
        
        cout << "Using Hamming Distance:" << endl;
        cout << "Explored Nodes: " << r1.explored_count << endl;
        cout << "Expanded Nodes: " << r1.expanded_count << endl;
        cout << "Number of steps: " << r1.steps.size()-1 << endl;

        cout << "Using Manhattan Distance:" << endl;
        cout << "Explored Nodes: " << r2.explored_count << endl;
        cout << "Expanded Nodes: " << r2.expanded_count << endl;
        cout << "Number of steps: " << r2.steps.size()-1 << endl;
        // for (int i = r2.steps.size() - 1; i >= 0; i--)
        //     print_v2d(r2.steps[i]);
    }
    return 0;
}