#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
#define NMAX 30
class Solution
{
public:
    bool debug=true;
    bool trace=false;
    unordered_map <string,bool> cache;
    unsigned matrix[NMAX];
    unsigned n_max;
    void displayMatrix(unsigned matrix[], int n)
    {
        if(!debug){
            return;
        }
        for (int i = 0; i < n; ++i)
        {
            string bits;
            for (int j = 0; j < n; ++j)
            {
                unsigned bit = matrix[i] & (1 << (n - j - 1));
                bits += bit > 0 ? "1" : "0";
            }
            cout << bits << endl;
        }
        cout << endl;
    }

    void traceMatrix(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,bool status){
        if(!debug && !trace){
            return;
        }
        cout<<"matrix["<<setw(2)<<x1<<","<<setw(2)<<y1<<" - "<<setw(2)<<x2<<","<<setw(2)<<y2<<"]";
        cout<<"-";
        cout<<"matrix["<<setw(2)<<x3<<","<<setw(2)<<y3<<" - "<<setw(2)<<x4<<","<<setw(2)<<y4<<"]"<<(status?"T":"F")<<endl;

    }
    void dumpCache(){
        if(debug){
            for(std::unordered_map<string,bool>::iterator it = cache.begin(); it != cache.end(); ++it) {
                string key=it->first;
                bool   val=it->second;
                cout<<"ptr:("<<setw(2)<<int(key[0])<<","<<setw(2)<<int(key[1])<<")["<<setw(2)<<int(key[2])<<"]="<<(val?"T":"F")<<endl;
            }
        }
        cache.clear();
    }
    bool validate(unsigned lx_ptr,unsigned ly_ptr,int n){
        bool l_magicX=true;
        bool r_magicX=true;
        unsigned row_bits=0;
        string m_key=string(1,lx_ptr)+string(1,ly_ptr)+string(1,n);
        if(cache.find(m_key) != cache.end()){
            return cache[m_key];
        }
        for(int i=0;i<n;++i){
            unsigned x=lx_ptr+i;
            unsigned y=ly_ptr+i;
            unsigned l_mask=(1<<(n-i-1));
            unsigned r_mask=(1<<i);
            unsigned c_row=(matrix[x]>>(n_max-ly_ptr-n))&((1<<n)-1);
            unsigned n_row=i<n-1?(matrix[x+1]>>(n_max-ly_ptr-n))&((1<<n)-1):0;
            if(c_row<1){
                cache[m_key]=false;
                return false;               
            }
            row_bits|=c_row;
            if((l_magicX && (c_row & l_mask )<1)){
                l_magicX=false;
            };
            if((r_magicX && (c_row & r_mask )<1)){
                r_magicX=false;
            };
        }

        if(row_bits!=(1<<n)-1){
            cache[m_key]=false;
            return false;             
        }
        if(n<3 || l_magicX || r_magicX){
            cache[m_key]=true;
            return true;
        }

        for(int i=1;i<n;++i){
            unsigned x1[2]={lx_ptr,lx_ptr+n-i};
            unsigned y1[2]={ly_ptr,ly_ptr};
            unsigned x2[2]={lx_ptr+i,lx_ptr};
            unsigned y2[2]={ly_ptr+i,ly_ptr+i};
            for(int j=0;j<2;++j){
                if(validate(x1[j],y1[j],i) && validate(x2[j],y2[j],n-i)){
                    traceMatrix(x1[j],y1[j],x1[j]+i-1,y1[j]+i-1,x2[j],y2[j],x2[j]+n-i-1,y2[j]+n-i-1,true);
                    cache[m_key]=true;
                    return true;
                }
                traceMatrix(x1[j],y1[j],x1[j]+i-1,y1[j]+i-1,x2[j],y2[j],x2[j]+n-i-1,y2[j]+n-i-1,false);
            }
        }
        cache[m_key]=false;
        return false;
    }

    bool isScramble(string s1, string s2)
    {
        string src = s1;
        string dst = s2;
        n_max = src.length();

        if (s1 == s2)
        {
            return true;
        }
        if (s1.length() != s2.length())
        {
            return false;
        }
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        if (s1 != s2)
        {
            return false;
        }
        if (s1.length() < 4)
        {
            return true;
        }
        for (int i = 0; i < n_max; ++i)
        {
            matrix[i] = 0;
            for (int j = 0; j < n_max; ++j)
            {
                if (src[i] == dst[j])
                {
                    matrix[i] |= 1 << (n_max - j - 1);
                }
            }
        }
        cache.clear();
        cout << "src:" << src << endl
             << "dst:" << dst << endl;
        // debug=false;
        displayMatrix(matrix, n_max);
        cout << "---------------------------------" << endl;
        bool ret= validate(0,0,n_max);
        dumpCache();
        return ret;
    }
};

int main()
{
    Solution solution;
    string s1, s2;

    // s1 = "eebaacbcbcadaaedceaaacadccd";
    // s2 = "eadcaacabaddaceacbceaabeccd";
    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "cbcbabc";
    // s2 = "bbbccca";
    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "abcd";
    // s2 = "bdac";
    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "great";
    // s2 = "rgeat";
    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "abab";
    // s2 = "baba";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    s1 = "ccabcbabcbabbbbcbb";
    s2 = "bbbbabccccbbbabcba";
    cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "bccbccaaabab";
    // s2 = "ccababcaabcb";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "vfldiodffghyq";
    // s2 = "vdgyhfqfdliof";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "abcdbdacbdac";
    // s2 = "bdacabcdbdac";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1 = "hobobyrqd";
    // s2 = "hbyorqdbo";

    // cout << (solution.isScramble(s1, s2) ? "true" : "false") << endl<<"---------------------------------"<<endl;

    // s1 = "xstjzkfpkggnhjzkpfjoguxvkbuopi";
    // s2 = "xbouipkvxugojfpkzjhnggkpfkzjts";

    // cout << (solution.isScramble(s1, s2) ? "true" : "false") << endl<<"---------------------------------"<<endl;

    // s1 = "npfgmkuleygms";
    // s2 = "ygksfmpngumle";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

    // s1="abcd";
    // s2="bdca";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl<<"---------------------------------"<<endl;

    // s1="abcd";
    // s2="badc";

    // cout<<(solution.isScramble(s1,s2)?"true":"false")<<endl<<"---------------------------------"<<endl;

    // s1 = "abcde";
    // s2 = "caebd";

    // cout << (solution.isScramble(s1, s2) ? "true" : "false") << endl<<"---------------------------------"<<endl;

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