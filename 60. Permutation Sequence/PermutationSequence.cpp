#include <iostream>
#include <vector>
using namespace std;
class Solution{
public:
    string getPermutation(int n, int k) {
       vector<char> candidate;
        unsigned counter=1;
        string ret;
        unsigned order;
        for(unsigned i=0;i<n;++i){
            candidate.push_back(i+'1');
            if(i>1){
                counter*=i;
            }
        }
        for(unsigned i=0;i<n;++i){
            unsigned idx = n-i;
            order = (k-1)/counter;
            ret += candidate[order];
            k-=order*counter;
            candidate.erase(candidate.begin()+order);
            if(idx>2){
                counter /=(idx-1);
            }
        }
        candidate.clear();
        return ret;
    }
};

int main()
{
    Solution solution;

    cout <<solution.getPermutation(2,1) << endl;
    cout <<solution.getPermutation(3,2) << endl;
    cout <<solution.getPermutation(3,3) << endl;
    cout <<solution.getPermutation(4,9) << endl;
    cout <<solution.getPermutation(5,9) << endl;


    return 0;
}