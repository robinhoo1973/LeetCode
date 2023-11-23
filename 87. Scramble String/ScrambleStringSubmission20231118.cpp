#include <iostream>
#include <iomanip>

#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
class Solution
{
public:
    unordered_map <string,bool> dict;
    void displayMatrix(unsigned matrix[], int n)
    {
        for (int i = 0; i < n; ++i)
        {
            string bits;
            for (int j = 0; j < n; ++j)
            {
                unsigned bit = matrix[i] & (1 << (n - j - 1));
                bits += bit > 0 ? " 1" : " 0";
            }
            cout << bits << endl;
        }
        cout << endl;
    }

    bool splitMatrix(unsigned matrix[], int l_size, int r_size, unsigned l_mat[], unsigned r_mat[], bool rev)
    {
        unsigned r_mask = (1<<r_size) - 1;
        unsigned l_mask = (1<<l_size) - 1;
        unsigned l_bit = 0;
        unsigned r_bit = 0;

        for (int row = 0; row < max(l_size, r_size); ++row)
        {
            if (row < l_size)
            {
                unsigned x = rev ? r_size + row:row;
                l_mat[row] = matrix[x] >> r_size;
                l_bit |= l_mat[row];
                if(l_mat[row]<1){
                    return false;
                }
            }
            if(row < r_size){
                unsigned x = rev ? row:l_size + row;
                r_mat[row] = matrix[x] & r_mask;
                r_bit |= r_mat[row];
                if(r_mat[row]<1){
                    return false;
                }
            }
        }
        if(r_bit==r_mask && l_bit==l_mask){
            // displayMatrix(matrix,l_size+r_size);
            // displayMatrix(l_mat,l_size);
            // displayMatrix(r_mat,r_size);
            // cout<<(r_bit==r_mask && l_bit==l_mask?"valid:true":"valid:false")<<endl;
        }

        return r_bit==r_mask && l_bit==l_mask;
    }
    string getMatrixKey(unsigned matrix[],int n){
        string m_key;
        for(int i=0;i<n;++i){
            unsigned key_char=matrix[i];
            for(int j=0;j<(n+7)/8;++j){
                m_key+=key_char&255;
                key_char>>=8;
            }
        }
        return m_key;
    }
    bool validateMatrix(unsigned matrix[],int n){
        bool l_magicX=true;
        bool r_magicX=true;
        string m_key=getMatrixKey(matrix,n);
        if(n<3){
            return true;
        }
        if(dict.find(m_key) != dict.end()){
            return dict[m_key];
        }
        for(int i=0;i<n;++i){
            unsigned l_mask=(1<<(n-i-1));
            unsigned r_mask=(1<<i);
            if((l_magicX && (matrix[i] & l_mask )<1) && 
               (i>n-2 || (matrix[i+1]&(l_mask>>1)>0) || (matrix[i]&(l_mask>>1)<1) || (matrix[i+1]&l_mask)<1)){
                l_magicX=false;
            };
            if((r_magicX && (matrix[i] & r_mask )<1) && 
               (i>n-2 || (matrix[i+1]&(r_mask<<1)>0) || (matrix[i]&(r_mask<<1))<1 || (matrix[i+1]&r_mask)<1)){
                r_magicX=false;
            };
            if(!l_magicX && !r_magicX){
                break;
            }
        }
        if(l_magicX || r_magicX){
            // if(r_magicX){
            // // cout<<"Magic X!!"<<endl;
            // // cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            // // displayMatrix(matrix,n);
            // // cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            // }
            dict[m_key]=true;
            return true;
        }

        for(int i=1;i<n;++i){
            unsigned l_mat[i];
            unsigned r_mat[n-i];
            // cout<<"matrix["<<setw(2)<<n<<"]:N("<<setw(2)<<i<<","<<setw(2)<<n-i<<")"<<endl;
            if(splitMatrix(matrix,i,n-i,l_mat,r_mat,false) && validateMatrix(l_mat,i) && validateMatrix(r_mat,n-i)){
                // cout<<"matrix["<<setw(2)<<n<<"]:N("<<setw(2)<<i<<","<<setw(2)<<n-i<<")*"<<endl;
                // displayMatrix(l_mat,i);
                dict[m_key]=true;
                return true;
            }
            // cout<<"matrix["<<setw(2)<<n<<"]:R("<<setw(2)<<i<<","<<setw(2)<<n-i<<")"<<endl;
            if(splitMatrix(matrix,i,n-i,l_mat,r_mat,true) && validateMatrix(l_mat,i) && validateMatrix(r_mat,n-i)){
                // cout<<"matrix["<<setw(2)<<n<<"]:N("<<setw(2)<<i<<","<<setw(2)<<n-i<<")*"<<endl;
                // displayMatrix(r_mat,n-i);
                dict[m_key]=true;
                return true;
            }
        }
        dict[m_key]=false;
        return false;
    }

    bool isScramble(string s1, string s2)
    {
        string src = s1;
        string dst = s2;
        int n = src.length();
        unsigned matrix[n];

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
        for (int i = 0; i < n; ++i)
        {
            matrix[i] = 0;
            for (int j = 0; j < n; ++j)
            {
                if (src[i] == dst[j])
                {
                    matrix[i] |= 1 << (n - j - 1);
                }
            }
        }
        cout << "src:" << src << endl
             << "dst:" << dst << endl;
        // displayMatrix(matrix, n);
        // cout << "---------------------------------" << endl;

        return validateMatrix(matrix,n);
    }
};

int main()
{
    Solution solution;
    string s1, s2;

    s1 = "eebaacbcbcadaaedceaaacadccd";
    s2 = "eadcaacabaddaceacbceaabeccd";
    cout<<(solution.isScramble(s1,s2)?"true":"false")<< endl<<"---------------------------------"<<endl;

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