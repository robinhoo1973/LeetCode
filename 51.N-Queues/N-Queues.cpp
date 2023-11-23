#include <vector>
#include <iostream>
using namespace std;
class Solution
{
public:
    vector<vector<string>>  sln;
    vector<string>          board;
    string                  ptx;
    bool ValidMove(int x,int y){
        unsigned ix,iy;
        for(unsigned i=0;i<y;++i){
            ix=x>ptx[i]?x-ptx[i]:ptx[i]-x;
            iy=y-i;
            if(ix==iy || ix==0){
                return false;
            }
        }
        return true;
    }
 
    vector<vector<string>> solveNQueens(int n)
    {
        unsigned char ptr = 0;
        unsigned char x   = 0;
        board = vector<string>(n, string(n, '.'));
        ptx   = string(n,0);
        while (ptr < n && ptr >= 0)
        {
            x=ptx[ptr];
            if( board[ptr][x]=='.' && ValidMove(x,ptr)){
                board[ptr][x]='Q';
                ptr++;
                if(ptr==n){
                    sln.push_back(board);
                    ptr--;
                }
                continue;
            }
            if( board[ptr][x]=='Q'){
                board[ptr][x]='.';
            }
            if(++ptx[ptr]==n){
                ptx[ptr]=0;
                ptr--;
            }
        }
        return sln;
    }
};

int main()
{
    Solution solution;
    vector<vector<string>> sol = solution.solveNQueens(4);
    for (unsigned i = 0; i < sol.size(); i++)
    {
        for (unsigned j = 0; j < sol[i].size(); j++)
        {
            cout << sol[i][j] << endl;
        }
        cout << endl;
    }

    return 0;
}