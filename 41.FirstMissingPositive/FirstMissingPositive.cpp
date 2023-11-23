#include <vector>
#include <iostream>

using namespace std;
class Solution
{
public:
    int firstMissingPositive(vector<int> &nums)
    {
        unsigned size = nums.size();
        string space=string(size+2,'0');
        unsigned i=0;
        for (i = 0; i < size; ++i)
        {
            if (nums[i] >0 && nums[i] <= size)
            {
                space[nums[i]]='1';
            }
        }

        size=space.length();
        for(i=1;i<size;i++){
            if(space[i]=='0'){
                return i;
            }
        }
        return size;
    }
};

int main()
{
    vector<int> nums;
    Solution solution;
    nums = {2147483647,2147483646,2147483645,3,2,1,-1,0,-2147483648};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {1,2,0};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {1,2,2,1,3,1,0,4,0};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {3, 4, -1, 1};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {7, 8, 9, 11, 12};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {1, 2, 6, 3, 5, 4};
    cout << solution.firstMissingPositive(nums) << endl;
    nums = {1,2,3,10,2147483647,9};
    cout << solution.firstMissingPositive(nums) << endl;
    return 0;
}