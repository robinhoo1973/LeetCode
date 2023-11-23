#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
class Solution
{
public:
    int largestRectangleArea(vector<unsigned> heights)
    {
        unsigned s_max = 0;
        unsigned n = heights.size();
        unsigned ptr = 0;
        vector<unsigned> index;

        for (unsigned i = 0; i < n; ++i)
        {
            unsigned val = heights[i];
            unsigned inc = i;
            if (ptr > val)
            {
                int idx = index.size()-1;
                while (idx >=0)
                {
                    if (heights[index[idx]] >=val)
                    {
                        unsigned s = heights[index[idx]] * (i-index[idx]);
                        if (s_max < s)
                        {
                            s_max = s;
                        }
                        if(index[idx]<inc){
                            inc=index[idx];
                            heights[inc]=val;
                        }
                        index.erase(index.begin() + idx);
                    }
                    --idx;
                }
            }

            if(val>0 && ptr!=val){
                index.push_back(inc);
            }
            ptr = val;

        }
        for (unsigned i = 0; i < index.size(); ++i)
        {
            unsigned s = heights[index[i]] * (n-index[i]);
            if (s_max < s)
            {
                s_max = s;
            }
        }
        return s_max;
    }

    int maximalRectangle(vector<vector<char>>& matrix)
    {
        int                      s_max = 0;
        int                      n=matrix.size();
        int                      m=matrix[0].size();
        int                      k=n>m?n:m;
        vector<vector<unsigned>> x_matrix=vector<vector<unsigned>>(n,vector<unsigned>(m,0));

        for(int i=0;i<n;++i){
            unsigned n_max;
            for(int j=0;j<m;++j){
                x_matrix[i][j]=matrix[i][j]&1;
                if(i>0 && x_matrix[i][j]){
                    x_matrix[i][j]+=x_matrix[i-1][j];
                }
            }
            n_max=largestRectangleArea(x_matrix[i]);
            if(s_max<n_max){
                s_max=n_max;
            }
        }
        return s_max;
    }
};

int main()
{
    Solution solution;
    vector<vector<char>> matrix;
    int s_max;

    matrix = {{'0','1','1','0','1'},{'1','1','0','1','0'},{'0','1','1','1','0'},{'1','1','1','1','0'},{'1','1','1','1','1'},{'0','0','0','0','0'}};
    s_max=solution.maximalRectangle(matrix);

    cout<<endl<<s_max<<endl;    
    
}