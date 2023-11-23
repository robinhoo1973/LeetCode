#include <iostream>
using namespace std;
class Solution
{
public:
    bool isMerged(string mergedBound,int x,int y,int l){
        int i=0;
        int n=mergedBound.length();
        while(i<n){
            if(mergedBound[i]==x && mergedBound[i+1]==y && mergedBound[i+2]==l){
                return false;
            }
            i+=4;
        }
        return true;
    }

    bool isScramble(string src, string dst)
    {
        int    n = src.length();
        int    y_bit = 0;
        bool   l_bit = true;
        bool   r_bit = true;
        string mergedBound;
        string x_route[n], y_route[n];

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
                    if(i>0 && j>0 && src[i-1]==dst[j+1]){
                        int x=i-1;
                        int y=j-1;
                        while(x>0 && y>0 && src[x]==dst[y]){
                            mergedBound+=string(1,x)+string(1,y)+string(1,j-y+1)+":";
                            x_route[x]+=string(1,j-y+1);
                            y_route[x]+=string(1,y);
                            x--;y--;
                        }
                    }
                    if(i<n-1 && j<n-1 && src[i+1]==dst[j+1]){
                        int x=i+1;
                        int y=j+1;
                        while(x<n-1 && y<n-1 && src[x]==dst[y]){
                            mergedBound+=string(1,i)+string(1,j)+string(1,y-j+1)+":";
                            x_route[i]+=string(1,y-j+1);
                            y_route[i]+=string(1,j);                            
                            x++;y++;
                        }                        
                    }
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
                int lx1 = x_route[x1].length();
                int ix1 = 0;
                while(ix1 < lx1)
                {
                    int y1 = y_route[x1][ix1];
                    int l_x1 = x_route[x1][ix1];
                    int x2 = x1 + l_x1;
                    if(x2<n){
                        int lx2=x_route[x2].length();
                        for (int ix2 = 0; ix2 < lx2; ++ix2)
                        {
                            int l_x2 = x_route[x2][ix2];
                            int y2   = y_route[x2][ix2];
                            int y3   = min(y1, y2);
                            int y4   = max(y1 + l_x1 - 1, y2 + l_x2 - 1);
                            int l_x3 = l_x1+l_x2;
                            if (y4 - y3 == l_x3 - 1 && isMerged(mergedBound,x1,y3,l_x3))
                            {
                                if (x1 == 0 && l_x3 == n)
                                {
                                    return true;
                                }
                                
                                x_route[x1] += string(1,l_x3);
                                y_route[x1] += string(1,y3);
                                ++lx1;
                                mergedBound+=string(1,x1)+string(1,y3)+string(1,l_x3)+":";
                                // cout << "merge[(" << x1 << "," << y1 << ")-(" << x1 + l_x1 - 1 << "," << y1 + l_x1 - 1 << ")<" << l_x1 << ">]" << endl;
                                // cout << "<=>" << endl;
                                // cout << "[(" << x2 << "," << y2 << "),(" << x2 + l_x2 - 1 << "," << y2 + l_x2 - 1 << ")<" << l_x2 << ">]" << endl;
                                // cout << "[(" << x3 << "," << y3 << "),(" << x4 << "," << y4 << ")<" << l_x1 + l_x2 << ">]" << endl;
                                r_bit = true;
                                break;
                            }
                        }
                    }
                    ++ix1;
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