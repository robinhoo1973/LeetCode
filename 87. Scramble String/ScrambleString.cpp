#include <iostream>
#include <unordered_map>
using namespace std;
class Solution
{
public:
    bool isScramble(string src, string dst)
    {
        int    n = src.length();
        int    y_bit = 0;
        bool   l_bit = true;
        bool   r_bit = true;
        unordered_map<unsigned,char> merged;
        while(n>>y_bit++);
        char x_route[n][n*y_bit], y_route[n][n*y_bit];
        char index[n];

        if (src == dst)
        {
            return true;
        }
        if (src.length() != dst.length())
        {
            return false;
        }
        y_bit=0;
        for (int i = 0; i < n; ++i)
        {
            index[i]=0;
            for (int j = 0; j < n; ++j)
            {
                r_bit &= (i != n - j - 1) || (src[i] == dst[j]);
                l_bit &= (i != j) || (src[i] == dst[j]);
                if (src[i] == dst[j])
                {
                    x_route[i][index[i]]= 1;
                    y_route[i][index[i]]= j;
                    ++index[i];

                    y_bit |= 1 << j;
                    if(i<n-1 && j>0 && src[i+1]==dst[j-1]){
                        int x=i;
                        int y=j;
                        while(x++>0 && y-->0 && src[x]==dst[y]){
                            merged[(i<<16)|(y<<8)|(x-i+1)]='1';
                            x_route[i][index[i]]=x-i+1;
                            y_route[i][index[i]]=y;
                            ++index[i];
                        }
                    }
                    if(i<n-1 && j<n-1 && src[i+1]==dst[j+1]){
                        int x=i;
                        int y=j;
                        while(x++<n-1 && y++<n-1 && src[x]==dst[y]){
                            merged[(i<<16)|(j<<8)|(x-i+1)]='1';
                            x_route[i][index[i]]=x-i+1;
                            y_route[i][index[i]]=j;                           
                            ++index[i];
                        }                        
                    }
                }
            }
            if (index[i] < 1)
            {
                return false;
            }
        }
        if (y_bit != (1 << n) - 1)
        {
            return false;
        }
        if (n<4 || l_bit || r_bit)
        {
            return true;
        }
        for (int x1 = n-3; x1>=0; --x1)
        {
            int lx1 = index[x1];
            int ix1 = 0;
            while(ix1 < lx1)
            {
                int y1   = y_route[x1][ix1];
                int l_x1 = x_route[x1][ix1];
                int x2   = x1 + l_x1;
                if(x2<n){
                    int lx2=index[x2];
                    for (int ix2 = 0; ix2 < lx2; ++ix2)
                    {
                        int l_x2 = x_route[x2][ix2];
                        int l_x3 = l_x1+l_x2;
                        int y2   = y_route[x2][ix2];
                        int y3   = min(y1, y2);
                        int y4   = max(y1 + l_x1 , y2 + l_x2);
                        if (y4 - y3 == l_x3 && merged.count((x1<<16)|(y3<<8)|l_x3)<1)
                        {
                            if (x1 == 0 && l_x3 == n)
                            {
                                return true;
                            }                            
                            x_route[x1][index[x1]] = l_x3;
                            y_route[x1][index[x1]] = y3;
                            ++index[x1];
                            ++lx1;
                            merged[(x1<<16)|(y3<<8)|l_x3]='1';
                            // cout << "merge[(" << x1 << "," << y1 << ")-(" << x1 + l_x1 - 1 << "," << y1 + l_x1 - 1 << ")<" << l_x1 << ">]" << endl;
                            // cout << "<=>" << endl;
                            // cout << "[(" << x2 << "," << y2 << "),(" << x2 + l_x2 - 1 << "," << y2 + l_x2 - 1 << ")<" << l_x2 << ">]" << endl;
                            // cout << "[(" << x1 << "," << y3 << "),(" << x1+l_x3-1 << "," << y4-1 << ")<" << l_x1 + l_x2 << ">]" << endl;
                        }
                    }
                }
                ++ix1;
            }
        }
        return false;
    }
};
int main()
{
    Solution solution;
    string s1, s2;

    s1 = "abbbcbaaccacaacc";
    s2 = "acaaaccabcabcbcb";
    cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl;

    s1 = "abcd";
    s2 = "abdc";

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