#include <cmath>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

struct Cell {
    unsigned x;
    unsigned y;
    unsigned available  = 0;
    char     seed       = '.';
    unsigned iterated   = 0;
};

bool comp(const Cell &x, const Cell &y)
{
    return x.available < y.available;
}

string getTimestamp(chrono::system_clock::time_point currentTime,string format)
{
    using std::chrono::system_clock;
    int millis = (currentTime.time_since_epoch().count() / 1000000) % 1000;
    time_t time = system_clock::to_time_t (currentTime);
    struct tm *time_info = localtime(&time);
    ostringstream oss;
    oss << put_time(time_info, format.c_str());
    return oss.str() + "."+to_string(millis+1000).substr(1,3);
}

string getTimeDiff(chrono::system_clock::time_point starttime,chrono::system_clock::time_point endtime){
    chrono::duration<float> diff = endtime - starttime;
    string timestamp  = to_string(((chrono::duration_cast<chrono::hours>(diff)).count())%24+100).substr(1,2);
    timestamp        += ":" + to_string(((chrono::duration_cast<chrono::minutes>(diff)).count())%60+100).substr(1,2);
    timestamp        += ":" + to_string(((chrono::duration_cast<chrono::seconds>(diff)).count())%60+100).substr(1,2);
    timestamp        += "." + to_string(((chrono::duration_cast<chrono::milliseconds>(diff)).count())%1000+1000).substr(1,3); 
    return timestamp;
}

const unsigned MASK = 0b111111111;

class Solution
{
public:
    vector<Cell>    queue;
    string          preset = "|";
    unsigned        x_grp[9] = {0,0,0,0,0,0,0,0,0};
    unsigned        y_grp[9] = {0,0,0,0,0,0,0,0,0};
    unsigned        b_grp[9] = {0,0,0,0,0,0,0,0,0};
    chrono::system_clock::time_point    starttime;

    void solveSudoku(vector<vector<char>> &board)
    {
        if (board.size() != 9 || board[0].size() != 9 || !Initialize(board))
        {
            cout << "Invalid Board Input!" << endl;
            return;
        }
        sort(queue.begin(), queue.end(), comp);
        starttime = chrono::system_clock::now();
        cout << getTimestamp(starttime,"%Y-%m-%d %H:%M:%S") << endl;
        DisplaySudoku(board);
        Seeking(board);
    }

    void SetCellValue(vector<vector<char>> &matrix, unsigned ix, unsigned iy, char v)
    {
        unsigned ig = floor(ix / 3) + floor(iy / 3) * 3;
        DelCellValue(matrix,ix, iy);
        if (v != '.')
        {
            matrix[ix][iy] = v;
            unsigned bit = 1 << (v - '1');
            x_grp[ix] |= bit;
            y_grp[iy] |= bit;
            b_grp[ig] |= bit;
        }
    }

    void Seeding(Cell &c)
    {
        unsigned g = floor(c.x / 3) + floor(c.y / 3) * 3;
        unsigned seeds =MASK & (MASK ^ (x_grp[c.x] | y_grp[c.y] | b_grp[g] | c.iterated));
        c.available = 0;
        c.seed = '.';
        for(unsigned i=0;i < 9;i++){
            if(seeds & (1<<i)){
                c.available++;
                if(c.seed == '.'){
                    c.seed = '1'+ i;
                }
            }
        }
    }

    void DelCellValue(vector<vector<char>> &matrix,unsigned ix, unsigned iy)
    {
        char v = matrix[ix][iy];
        if(v == '.'){
            return;
        }
        unsigned ig = floor(ix / 3) + floor(iy / 3) * 3;
        matrix[ix][iy] = '.';
        unsigned bit = MASK ^ (1 << v - '1');
        x_grp[ix] &= bit;
        y_grp[iy] &= bit;
        b_grp[ig] &= bit;
    }

    bool Initialize(vector<vector<char>> &matrix)
    {
        unsigned bit;
        unsigned g;
        for (unsigned x = 0; x < 9; x++)
        {
            for (unsigned y = 0; y < 9; y++)
            {
                
                if (matrix[x][y] == '.')
                {
                    Cell c = Cell();
                    c.x = x;
                    c.y = y;
                    Seeding(c);
                    queue.push_back(c);
                }
                else
                {
                    bit = 1 << (matrix[x][y] - '1');
                    g = floor(x / 3) + floor(y / 3) * 3;
                    preset += to_string(x)+ to_string(y)+"|";
                    if((x_grp[x] & bit) | (y_grp[y] & bit) | (b_grp[g] & bit)){
                        cout << x << "," << y <<endl;
                        return false;
                    }
                    SetCellValue(matrix, x, y, matrix[x][y]);
                }
            }
        }
        return true;
    }

    void DisplaySudoku(vector<vector<char>> &matrix)
    {
        cout << "+---+---+---+---+---+---+---+---+---+" << endl;
        for (unsigned y = 0; y < 9; y++)
        {
            string line = "|";
            for (unsigned x = 0; x < 9; x++)
            {
                if (matrix[x][y] != '.')
                {
                    if (preset.find("|" + to_string(x) + to_string(y )+ "|") != string::npos)
                    {
                        line += "[" + string(1, matrix[x][y]) + "]|";
                    }
                    else
                    {
                        line += " " + string(1, matrix[x][y]) + " |";
                    }
                }
                else
                {
                    line += "   |";
                }
            }
            cout << line << endl;
            cout << "+---+---+---+---+---+---+---+---+---+" << endl;
        }
        cout << endl;
    }

    void Seeking(vector<vector<char>> &matrix)
    {
        unsigned index = 0;
        while (index < queue.size() && index >= 0)
        {
            SetCellValue(matrix,queue[index].x, queue[index].y, queue[index].seed);
            if(queue[index].seed != '.'){
                queue[index].iterated |= 1 << (queue[index].seed - '1');
            }

            if (index == queue.size() - 1)
            {
                chrono::system_clock::time_point now=chrono::system_clock::now();
                cout << getTimestamp(now,"%Y-%m-%d %H:%M:%S") << endl;
                cout << getTimestamp(starttime,"%H:%M:%S") << " - " << getTimestamp(now,"%H:%M:%S") << endl;
                cout << "<C++>:[" << getTimeDiff(starttime,now) << "]" << endl;

                DisplaySudoku(matrix);
                return;
                index--; // if you want to get all solutions, remove the last line's return
            }
            else
            {
                index++;
            }
            unsigned i = index;
            while (index >= 0 && i < queue.size())
            {
                Seeding(queue[i]);
                if (queue[i].available == 0)
                {
                    queue[i].iterated = 0;
                    index--;
                    i = index;
                    if (i < queue.size() && i >= 0)
                    {
                        DelCellValue(matrix, queue[i].x, queue[i].y);
                    }
                }
                else
                {
                    i++;
                }
            }
            if (index < queue.size() && index >= 0)
            {
                // sort(queue.begin() + index, queue.end(), comp);
                partial_sort(queue.begin() + index, queue.end(), queue.end(), comp);
            }
        }
    }
};

int main()
{
    Solution sudoku;
    vector<vector<char>> board = {{ '5', '3', '.', '.', '7', '.', '.', '.', '.' }, 
                                  { '6', '.', '.', '1', '9', '5', '.', '.', '.' }, 
                                  { '.', '9', '8', '.', '.', '.', '.', '6', '.' }, 
                                  { '8', '.', '.', '.', '6', '.', '.', '.', '3' }, 
                                  { '4', '.', '.', '8', '.', '3', '.', '.', '1' }, 
                                  { '7', '.', '.', '.', '2', '.', '.', '.', '6' }, 
                                  { '.', '6', '.', '.', '.', '.', '2', '8', '.' }, 
                                  { '.', '.', '.', '4', '1', '9', '.', '.', '5' }, 
                                  { '.', '.', '.', '.', '8', '.', '.', '7', '9' }};
    sudoku.solveSudoku(board);
    return 0;
}
