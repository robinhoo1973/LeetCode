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

class IsChars
{
    public:
        IsChars(const char* charsToRemove) : chars(charsToRemove) {};

        bool operator()(char c)
        {
            const char* testChar = chars;
            for(; (*testChar != 0) && (*testChar != c); ++testChar);
            return (*testChar != 0);
        }

    private:
        const char* chars;
};

class Cell{
    public:
    unsigned x;
    unsigned y;
    unsigned available;
    char seed;  
    string iterated;

    Cell(unsigned ix, unsigned iy, vector<string> x_grp, vector<string> y_grp, vector<string> b_grp)
    {
        x = ix;
        y = iy;
        Seeding(x_grp, y_grp, b_grp);
    }

    void Seeding(vector<string> x_grp, vector<string> y_grp, vector<string> b_grp)
    {
        string seeds = "123456789";
        unsigned g = floor(x / 3) + floor(y / 3) * 3;
        string mask = x_grp[x] + y_grp[y] + b_grp[g] + iterated;
        std::sort(mask.begin(), mask.end());
        mask.erase(std::unique(mask.begin(), mask.end()), mask.end());
        if (mask.length() != 0)
        {
            seeds.erase(std::remove_if(seeds.begin(), seeds.end(), IsChars(mask.c_str())), seeds.end());
        }
        if (seeds.length() != 0)
        {
            sort(seeds.begin(), seeds.end());
        }
        available = seeds.length();
        seed = '0';
        if (available != 0)
        {
            seed = seeds[0];
        }
    }
};

bool comp(const Cell& x, const Cell& y)
{
    return x.available < y.available;
}

class Sudoku{
public:
    vector<vector<char>>                matrix;
    vector<Cell>                        queue;
    vector<string>                      preset;
    vector<string>                      x_grp;
    vector<string>                      y_grp;
    vector<string>                      b_grp;
    unsigned                            size=0;
    chrono::system_clock::time_point    starttime;
    void Solve(string sudoku)
    {
        regex reg("[ |\\.]");
        sudoku = regex_replace(sudoku, reg, "0");
        istringstream lines(sudoku);
        string line;
        unsigned x=0;
        unsigned y=0;
        while (getline(lines, line))
        {
            if (size == 0)
            {
                size    = line.length();
                matrix  = vector<vector<char>>(size,vector<char>(size));
                x_grp   = vector<string> (size);
                y_grp   = vector<string> (size);
                b_grp   = vector<string> (size);
            }
            for (unsigned x = 0; x < size; x++)
            {
                SetCellValue(x, y, line[x]);
            }
            y++;
        }

        Initialize();
        Seeking();
        FreeMemory();
    }

    void FreeMemory(){
        queue.clear();
        preset.clear();
        matrix.clear();
        x_grp.clear();
        y_grp.clear();
        b_grp.clear();
        size = 0;
    }

    void SetCellValue(unsigned ix, unsigned iy, char v)
    {
        unsigned ig = floor(ix / 3) + floor(iy / 3) * 3;
        DelCellValue(ix, iy);
        if (v != '0')
        {
            matrix[ix][iy] = v;
            x_grp[ix] += v;
            y_grp[iy] += v;
            b_grp[ig] += v;
        }
    }

    void DelCellValue(unsigned ix, unsigned iy)
    {
        char v = matrix[ix][iy];
        unsigned ig = floor(ix / 3) + floor(iy / 3) * 3;
        matrix[ix][iy] = '0';
        x_grp[ix].erase(remove(x_grp[ix].begin(), x_grp[ix].end(), v), x_grp[ix].end());
        y_grp[iy].erase(remove(y_grp[iy].begin(), y_grp[iy].end(), v), y_grp[iy].end());
        b_grp[ig].erase(remove(b_grp[ig].begin(), b_grp[ig].end(), v), b_grp[ig].end());
    }

    void Initialize()
    {
        for (unsigned x = 0; x < size; x++)
        {
            for (unsigned y = 0; y < size; y++)
            {
                if (matrix[x][y] == '0')
                {
                    queue.push_back(Cell(x, y, x_grp, y_grp, b_grp));
                }
                else
                {
                    preset.push_back(to_string(x) + "," + to_string(y));
                }
            }
        }

        sort(queue.begin(), queue.end(), comp);
        starttime = chrono::system_clock::now();
        cout << getTimestamp(starttime,"%Y-%m-%d %H:%M:%S") << endl;
        DisplaySudoku();
    }

    void DisplaySudoku()
    {
        cout << "+---+---+---+---+---+---+---+---+---+" << endl;
        for (unsigned y = 0; y < size; y++)
        {
            string line = "|";
            for (unsigned x = 0; x < size; x++)
            {
                if (matrix[x][y] != '0')
                {
                    if (std::find(preset.begin(), preset.end(), to_string(x) + "," + to_string(y)) != preset.end())
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

    void Seeking()
    {
        unsigned index = 0;
        while (index < queue.size() && index >= 0)
        {
            SetCellValue(queue[index].x, queue[index].y, queue[index].seed);
            queue[index].iterated += queue[index].seed;

            if (index == queue.size() - 1)
            {
                chrono::system_clock::time_point now=chrono::system_clock::now();
                cout << getTimestamp(now,"%Y-%m-%d %H:%M:%S") << endl;
                cout << getTimestamp(starttime,"%H:%M:%S") << " - " << getTimestamp(now,"%H:%M:%S") << endl;
                cout << "<C++>:[" << getTimeDiff(starttime,now) << "]" << endl;

                DisplaySudoku();
                index--;
            }
            else
            {
                index++;
            }
            unsigned i = index;
            while(index >= 0 && i < queue.size())
            {
                queue[i].Seeding(x_grp, y_grp, b_grp);
                if(queue[i].available == 0){
                    queue[i].iterated="";
                    index--;
                    i=index;
                    if(i < queue.size() && i >= 0){
                        DelCellValue(queue[i].x, queue[i].y);
                    }                    
                }
                else{
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

int main(){
    Sudoku sudoku;
    chrono::system_clock::time_point starttime=chrono::system_clock::now();
    cout << "\033[2J\033[1;1H" << endl;
    sudoku.Solve("1.4.67...\n5.983..4.\n.7..9.3..\n8..5.....\n..51.67..\n.....9..5\n..1.7..3.\n.9..541.2\n...91.6.4\n");
    sudoku.Solve("...6.....\n..5...3..\n.2.8.4.7.\n..4.2.5.3\n...9.6...\n2.6.7.8..\n.7.4.3.9.\n..3...7..\n.....1...\n");
    sudoku.Solve("8........\n..36.....\n.7..9.2..\n.5...7...\n....457..\n...1...3.\n..1....68\n..85...1.\n.9....4..\n");
    chrono::system_clock::time_point now=chrono::system_clock::now();
    cout << getTimestamp(starttime,"%Y-%m-%d %H:%M:%S") << " - " << getTimestamp(now,"%Y-%m-%d %H:%M:%S") << endl;
    cout << "<C++>:[" << getTimeDiff(starttime,now) << "]" << endl;
    return 0;
}
