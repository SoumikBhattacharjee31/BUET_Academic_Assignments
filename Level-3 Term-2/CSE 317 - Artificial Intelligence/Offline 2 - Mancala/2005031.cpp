#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <tuple>
#include <utility>
#include <algorithm>
#include <random>

#define W1 5
#define W2 5
#define W3 5
#define W4 5

using namespace std;

mt19937 *g;

enum GameState
{
    GAME_UNFINISHED,
    PLAYER1_WINS,
    PLAYER2_WINS,
    DRAW
};

struct Mancala
{
    int player1_storage;
    int player2_storage;
    vector<int> player1_bins;
    vector<int> player2_bins;
    bool player1_current;

    Mancala()
    {
        this->player1_storage = 0;
        this->player2_storage = 0;
        this->player1_bins = vector<int>(6, 4);
        this->player2_bins = vector<int>(6, 4);
        this->player1_current = true;
    }

    Mancala(const Mancala &mancala)
    {
        this->player1_storage = mancala.player1_storage;
        this->player2_storage = mancala.player2_storage;
        this->player1_bins = mancala.player1_bins;
        this->player2_bins = mancala.player2_bins;
        this->player1_current = mancala.player1_current;
    }

    void print()
    {
        cout << "Player1 storage: " << this->player1_storage << endl;
        cout << "Player2 storage: " << this->player2_storage << endl;
        cout << "Player1 bins: ";
        for (int i = 0; i < 6; i++)
            cout << this->player1_bins[i] << " ";
        cout << endl;
        cout << "Player2 bins: ";
        for (int i = 5; i >= 0; i--)
            cout << this->player2_bins[i] << " ";
        cout << endl;
    }

    tuple<Mancala, bool, int> getMancalaAfterMove(int pos)
    {
        // create return object
        Mancala mancala(*this);
        int extra_move = 0;
        int captured = 0;

        // invalid pos value
        bool zeroRect = this->player1_current ? (this->player1_bins[pos] == 0) : (this->player2_bins[pos] == 0);
        if (pos < 0 || pos > 5 || zeroRect)
        {
            cout << "Illegal move" << endl;
            exit(0);
            return make_tuple(mancala, extra_move, captured);
        }

        // take all stones from given position
        int initial_stones;
        if (mancala.player1_current)
        {
            initial_stones = player1_bins[pos];
            mancala.player1_bins[pos] = 0;
        }
        else
        {
            initial_stones = player2_bins[pos];
            mancala.player2_bins[pos] = 0;
        }

        // place stones
        bool cur_player1_side = mancala.player1_current;
        for (int i = 0; i < initial_stones; i++)
        {
            pos++;
            if (cur_player1_side)
            {
                if (pos == 6)
                {
                    cur_player1_side = false;
                    pos = -1;
                    if (mancala.player1_current)
                    {
                        mancala.player1_storage++;
                        captured++;
                        if (i == initial_stones - 1)
                            extra_move = 1;
                    }
                    else
                        i--;
                }
                else
                {
                    if (i == initial_stones - 1 && mancala.player1_current && mancala.player1_bins[pos] == 0 && mancala.player2_bins[5 - pos] != 0)
                    {
                        mancala.player1_storage += mancala.player2_bins[5 - pos] + 1;
                        captured += mancala.player2_bins[5 - pos] + 1;
                        mancala.player2_bins[5 - pos] = 0;
                    }
                    else
                        mancala.player1_bins[pos]++;
                }
            }
            else
            {
                if (pos == 6)
                {
                    cur_player1_side = true;
                    pos = -1;
                    if (!(mancala.player1_current))
                    {
                        mancala.player2_storage++;
                        captured--;
                        if (i == initial_stones - 1)
                            extra_move = -1;
                    }
                    else
                        i--;
                }
                else
                {
                    if (i == initial_stones - 1 && !mancala.player1_current && mancala.player2_bins[pos] == 0 && mancala.player1_bins[5 - pos] != 0)
                    {
                        mancala.player2_storage += mancala.player1_bins[5 - pos] + 1;
                        captured -= mancala.player1_bins[5 - pos] + 1;
                        mancala.player1_bins[5 - pos] = 0;
                    }
                    else
                        mancala.player2_bins[pos]++;
                }
            }
        }
        if (!extra_move)
            mancala.player1_current = !mancala.player1_current;
        return make_tuple(mancala, extra_move, captured);
    }
    void move(int pos)
    {
        Mancala mancala = get<0>(this->getMancalaAfterMove(pos));
        this->player1_bins = mancala.player1_bins;
        this->player2_bins = mancala.player2_bins;
        this->player1_current = mancala.player1_current;
        this->player1_storage = mancala.player1_storage;
        this->player2_storage = mancala.player2_storage;
    }

    GameState check_game_state()
    {
        int player1_bin_sum = 0;
        for (int i = 0; i < this->player1_bins.size(); i++)
            player1_bin_sum += this->player1_bins[i];

        int player2_bin_sum = 0;
        for (int i = 0; i < this->player2_bins.size(); i++)
            player2_bin_sum += this->player2_bins[i];

        if (player1_bin_sum == 0 || player2_bin_sum == 0)
        {
            if (player1_bin_sum == 0)
                player2_storage += player2_bin_sum;
            else
                player1_storage += player1_bin_sum;
            if (this->player1_storage > this->player2_storage)
                return PLAYER1_WINS;
            else if (this->player1_storage < this->player2_storage)
                return PLAYER2_WINS;
            else
                return DRAW;
        }

        return GAME_UNFINISHED;
    }
};

pair<int,int> adversarial_search(Mancala mancala, int depth, int (*heuristic)(int, int, int, int, int, int, int, int), bool player1_win, int alpha, int beta, int extramove, int captured, int w1, int w2, int w3, int w4)
{
    if (depth == 0 || mancala.check_game_state() != GAME_UNFINISHED)
    {
        int storagesub = mancala.player1_storage - mancala.player2_storage;
        int binsumsub = 0;
        for (int i = 0; i < 6; i++)
            binsumsub += mancala.player1_bins[i] - mancala.player2_bins[i];
        return make_pair(heuristic(storagesub, binsumsub, extramove, captured, w1, w2, w3, w4) * (player1_win ? 1 : -1),-1);
    }

    int besti = -1;
    int bestv = INT_MIN;
    vector<int> arr = {0,1,2,3,4,5};
    shuffle(arr.begin(),arr.end(),*g);

    if (player1_win == mancala.player1_current)
    {
        int maxh = INT_MIN;
        for (int i : arr)
        {
            if ((mancala.player1_bins[i] == 0 && mancala.player1_current) || (mancala.player2_bins[i] == 0 && !mancala.player1_current))
                continue;
            auto newmancala = mancala.getMancalaAfterMove(i);
            auto retpair = adversarial_search(get<0>(newmancala), depth - 1, heuristic, player1_win, alpha, beta, extramove + get<1>(newmancala), captured + get<2>(newmancala), w1, w2, w3, w4);

            int curh = retpair.first;

            if (curh > bestv)
            {
                bestv = curh;
                besti = i;
            }

            maxh = max(maxh, curh);
            alpha = max(alpha, curh);
            if (beta <= alpha)
                break;
        }
        return make_pair(maxh, besti);
    }
    else
    {
        int minh = INT_MAX;
        for (int i : arr)
        {
            if (mancala.player1_bins[i] == 0 && mancala.player1_current)
                continue;
            if (mancala.player2_bins[i] == 0 && !mancala.player1_current)
                continue;
            auto newmancala = mancala.getMancalaAfterMove(i);
            auto retpair = adversarial_search(get<0>(newmancala), depth - 1, heuristic, player1_win, alpha, beta, extramove + get<1>(newmancala), captured + get<2>(newmancala), w1, w2, w3, w4);

            int curh = retpair.first;

            minh = min(minh, curh);
            beta = min(beta, curh);
            if (beta <= alpha)
                break;
        }
        return make_pair(minh, besti);
    }
}

int heuristic1(int storagesub, int binsumsub, int extramovesub, int capturedsub, int w1, int w2, int w3, int w4)
{
    return storagesub;
}
int heuristic2(int storagesub, int binsumsub, int extramovesub, int capturedsub, int w1, int w2, int w3, int w4)
{
    return storagesub * w1 + binsumsub * w2;
}
int heuristic3(int storagesub, int binsumsub, int extramovesub, int capturedsub, int w1, int w2, int w3, int w4)
{
    return storagesub * w1 + binsumsub * w2 + extramovesub * w3;
}
int heuristic4(int storagesub, int binsumsub, int extramovesub, int capturedsub, int w1, int w2, int w3, int w4)
{
    return storagesub * w1 + binsumsub * w2 + extramovesub * w3 + capturedsub * w4;
}

pair<int, int> compareMancala(int (*h1)(int, int, int, int, int, int, int, int), int (*h2)(int, int, int, int, int, int, int, int), int total_games)
{
    int h1win = 0;
    int h2win = 0;
    uniform_int_distribution<> dist(0,1);
    uniform_int_distribution<> dist_weight(1,10);
    uniform_int_distribution<> dist_depth(3,5);
    int w1=dist_weight(*g),w2=dist_weight(*g),w3=dist_weight(*g),w4=dist_weight(*g),w5=dist_weight(*g),w6=dist_weight(*g),w7=dist_weight(*g),w8=dist_weight(*g);
    int d1=dist_depth(*g), d2=dist_depth(*g);
    // int w1=W1,w2=W2,w3=W3,w4=W4,w5=W1,w6=W2,w7=W3,w8=W4;
    while (total_games--)
    {
        Mancala m;
        dist(*g);
        int turn = dist(*g);
        if(turn)
            m.player1_current=false;
        GameState gamestate = GAME_UNFINISHED;
        while (gamestate == GAME_UNFINISHED)
        {
            if (m.player1_current)
                m.move(adversarial_search(m, d1, h1, m.player1_current, INT_MIN, INT_MAX, 0, 0, w1, w2, w3, w4).second);
            else
                m.move(adversarial_search(m, d2, h2, m.player1_current, INT_MIN, INT_MAX, 0, 0, w5, w6, w7, w8).second);
            gamestate = m.check_game_state();
        }
        if (gamestate == PLAYER1_WINS)
            h1win++;
        else
            h2win++;
    }
    return make_pair(h1win, h2win);
}


int main()
{
    random_device rd;
    g = new mt19937(rd());
    int wins[] = {0,0,0,0};
    int total_games = 100;
    auto rat = compareMancala(heuristic1, heuristic2, total_games);
    cout << "H1:H2 -> " << rat.first << ":" << rat.second << endl;
    wins[0]+=rat.first;
    wins[1]+=rat.second;
    rat = compareMancala(heuristic1, heuristic3, total_games);
    cout << "H1:H3 -> " << rat.first << ":" << rat.second << endl;
    wins[0]+=rat.first;
    wins[2]+=rat.second;
    rat = compareMancala(heuristic1, heuristic4, total_games);
    cout << "H1:H4 -> " << rat.first << ":" << rat.second << endl;
    wins[0]+=rat.first;
    wins[3]+=rat.second;
    rat = compareMancala(heuristic2, heuristic3, total_games);
    cout << "H2:H3 -> " << rat.first << ":" << rat.second << endl;
    wins[1]+=rat.first;
    wins[2]+=rat.second;
    rat = compareMancala(heuristic2, heuristic4, total_games);
    cout << "H2:H4 -> " << rat.first << ":" << rat.second << endl;
    wins[1]+=rat.first;
    wins[3]+=rat.second;
    rat = compareMancala(heuristic3, heuristic4, total_games);
    cout << "H3:H4 -> " << rat.first << ":" << rat.second << endl;
    wins[2]+=rat.first;
    wins[3]+=rat.second;
    int *m1 = max_element(wins, wins+4);
    *m1=INT_MIN;
    int *m2 = max_element(wins, wins+4);
    *m2=INT_MIN;
    int *m3 = max_element(wins, wins+4);
    *m3=INT_MIN;
    int *m4 = max_element(wins, wins+4);
    *m4=INT_MIN;
    cout<<"Heuristics: H"<<(distance(begin(wins),m1)+1)<<" > H"<<(distance(begin(wins),m2)+1)<<" > H"<<(distance(begin(wins),m3)+1)<<" > H"<<(distance(begin(wins),m4)+1)<<endl;
    delete(g);

    // Mancala m;
    // while(true){
    //     m.print();
    //     if(m.player1_current){
    //         int bestmove = adversarial_search(m,12,heuristic4,m.player1_current,INT_MIN,INT_MAX, 0, 0, w1, w2, w3, w4).second;
    //         cout<<"Move "<<bestmove<<endl;
    //         m.move(bestmove);
    //     }
    //     else{
    //         cout<<"Opponent Move: ";
    //         int opmove;
    //         cin>>opmove;
    //         m.move(opmove);
    //     }
    // }

    return 0;
}