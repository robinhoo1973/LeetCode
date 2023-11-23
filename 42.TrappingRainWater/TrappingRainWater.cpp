#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
    int trap(vector<int>& height) {
        unsigned size=height.size();
        if(size < 3){
            return 0;
        }
        int l_max[size];
        int r_max[size];
        int l_val[size];
        int r_val[size];
        l_max[0]=l_val[0]=r_max[0]=r_val[0]=l_val[size-1]=l_max[size-1]=r_max[size-1]=r_val[size-1]=0;
        for(unsigned i=1; i<size-1;i++){
            l_max[i]=l_max[i-1]>height[i-1]?l_max[i-1]:height[i-1];
            l_val[i]=l_max[i]-height[i]>0?l_max[i]-height[i]:0;
            r_max[size-i-1]=r_max[size-i]>height[size-i]?r_max[size-i]:height[size-i];
            r_val[size-i-1]=r_max[size-i-1]-height[size-i-1]>0?r_max[size-i-1]-height[size-i-1]:0;
        }
        int w = 0;
        for(unsigned i=1; i < size-1;i++){
            w+=l_val[i]<r_val[i]?l_val[i]:r_val[i];
        }
        return w;
    }
};

int main(){
    vector<int> height = {0,1,0,2,1,0,1,3,2,1,2,1};
    Solution solution;
    cout << solution.trap(height) << endl;
    return 0;
}