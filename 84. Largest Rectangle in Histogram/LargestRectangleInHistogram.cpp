#include <iostream>
#include <vector>
using namespace std;
class Solution
{
public:
    int largestRectangleArea(vector<int> &heights)
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
        // index.clear();
        return s_max;
    }
};

int main()
{
    Solution solution;
    vector<int> heights;

    heights = {3,6,5,7,4,8,1,0};
    cout <<"(";
    for(unsigned i=0;i<heights.size();++i){
        cout << heights[i] ;
        if(i<heights.size()-1){
            cout <<" , ";
        }
    }
    cout << ")[" << heights.size()<<"]:"<< solution.largestRectangleArea(heights) << endl;

    heights = {2,1,2};
    cout <<"(";
    for(unsigned i=0;i<heights.size();++i){
        cout << heights[i] ;
        if(i<heights.size()-1){
            cout <<" , ";
        }
    }
    cout << ")[" << heights.size()<<"]:"<< solution.largestRectangleArea(heights) << endl;

    heights = {4,7,5,1,8,9,4,4,1,3,5,3,7,9,9,1,4};
    cout <<"(";
    for(unsigned i=0;i<heights.size();++i){
        cout << heights[i] ;
        if(i<heights.size()-1){
            cout <<" , ";
        }
    }
    cout << ")[" << heights.size()<<"]:"<< solution.largestRectangleArea(heights) << endl;

    heights = {4,2,0,3,2,4,3,4};
    cout <<"(";
    for(unsigned i=0;i<heights.size();++i){
        cout << heights[i] ;
        if(i<heights.size()-1){
            cout <<" , ";
        }
    }
    cout << ")[" << heights.size()<<"]:"<< solution.largestRectangleArea(heights) << endl;


    return 0;
}