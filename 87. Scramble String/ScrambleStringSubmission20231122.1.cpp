#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
#define NMAX 30
class Solution
{
public:
    void displayMatrix(string matrix[])
    {
        for (int i = 0; i < matrix[0].length(); ++i)
        {
            for (int j = 0; j < matrix[0].length(); ++j)
            {
                cout << setw(3) << int(matrix[i][j]);
            }
            cout << endl;
        }
        cout << endl;
    }
    
    bool mergedBound(string x_route,string y_route,int y,int l){
        for(int i=0;i<x_route.length();++i){
            if(x_route[i]==l && y_route[i]==y){
                return false;
            }
        }
        return true;

    }

    bool isScramble(string src, string dst)
    {
        int n = src.length();
        bool l_bit = true;
        bool r_bit = true;
        int y_bit = 0;

        string x_route[n], y_route[n];

        cout << "src:" << src << endl;
        cout << "dst:" << dst << endl;
        if (src == dst)
        {
            return true;
        }
        if (src.length() != dst.length())
        {
            return false;
        }

        for (int i = 0; i < n; ++i)
        {
            x_route[i] = "";
            y_route[i] = "";
            for (int j = 0; j < n; ++j)
            {
                r_bit &= (i != n - j - 1) || (src[i] == dst[j]);
                l_bit &= (i != j) || (src[i] == dst[j]);
                if (src[i] == dst[j])
                {
                    x_route[i] += string(1, 1);
                    y_route[i] += string(1, j);
                    y_bit |= 1 << j;
                }
            }
            if (x_route[i].length() < 1)
            {
                return false;
            }
        }
        if (y_bit != (1 << n) - 1)
        {
            return false;
        }
        if (l_bit || r_bit)
        {
            return true;
        }
        r_bit = true;
        while (r_bit)
        {
            r_bit = false;
            for (int x1 = 0; x1 < n; ++x1)
            {
                for (int ix1 = 0; ix1 < x_route[x1].length(); ++ix1)
                {
                    int y1 = y_route[x1][ix1];
                    int l_x1 = x_route[x1][ix1];
                    bool breakout = false;
                    int x2 = x1 + l_x1;
                    if(x2<n){
                        for (int ix2 = 0; !breakout && ix2 < x_route[x2].length(); ++ix2)
                        {
                            int l_x2 = x_route[x2][ix2];
                            int y2 = y_route[x2][ix2];
                            int x3 = min(x1, x2);
                            int y3 = min(y1, y2);
                            int x4 = max(x1 + l_x1 - 1, x2 + l_x2 - 1);
                            int y4 = max(y1 + l_x1 - 1, y2 + l_x2 - 1);
                            if (mergedBound(x_route[x3],y_route[x3],y3,l_x1+l_x2) && y4 - y3 == x4 - x3 && x4 - x3 == l_x1 + l_x2 - 1)
                            {
                                if (x3 == 0 && l_x1 + l_x2 == n)
                                {
                                    return true;
                                }
                                
                                x_route[x3] += string(1,l_x1 + l_x2);
                                y_route[x3] += string(1,y3);
                                // cout << "merge[(" << x1 << "," << y1 << ")-(" << x1 + l_x1 - 1 << "," << y1 + l_x1 - 1 << ")<" << l_x1 << ">]" << endl;
                                // cout << "<=>" << endl;
                                // cout << "[(" << x2 << "," << y2 << "),(" << x2 + l_x2 - 1 << "," << y2 + l_x2 - 1 << ")<" << l_x2 << ">]" << endl;
                                // cout << "[(" << x3 << "," << y3 << "),(" << x4 << "," << y4 << ")<" << l_x1 + l_x2 << ">]" << endl;
                                breakout = true;
                                r_bit = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
};

int main()
{
    Solution solution;
    string s1, s2;

    s1 = "great";
    s2 = "rgeat";
    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "abab";
    s2 = "baba";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "ccabcbabcbabbbbcbb";
    s2 = "bbbbabccccbbbabcba";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "bccbccaaabab";
    s2 = "ccababcaabcb";

    cout << (solution.isScramble(s1, s2) ? "true" : "false") << endl;

    s1 = "vfldiodffghyq";
    s2 = "vdgyhfqfdliof";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "abcdbdacbdac";
    s2 = "bdacabcdbdac";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "hobobyrqd";
    s2 = "hbyorqdbo";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "xstjzkfpkggnhjzkpfjoguxvkbuopi";
    s2 = "xbouipkvxugojfpkzjhnggkpfkzjts";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "npfgmkuleygms";
    s2 = "ygksfmpngumle";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1="abcd";
    s2="bdca";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1="abcd";
    s2="badc";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1="abcde";
    s2="caebd";

    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    return 0;
}

// true
// true
// true
// true
// true
// false
// true
// true
// false