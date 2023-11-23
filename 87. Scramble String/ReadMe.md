#Different Way to Solve the Scramble String Problem

Before we go further into the problem, let's put the strings in to 2 dimension matrix like below fig. 1 with example string "great" and "rgeat". And cross out the same characters in the according positions of the matrix.
```
  g    r   e   a   t
 +---+---+---+---+---+
r|   | x |   |   |   |
 +---+---+---+---+---+
g| x |   |   |   |   |
 +---+---+---+---+---+
e|   |   | x |   |   |
 +---+---+---+---+---+
a|   |   |   | x |   |
 +---+---+---+---+---+
t|   |   |   |   | x |
 +---+---+---+---+---+
         fig. 1
```

We can easily figure out the same sequence substring or reverse sequence substring all line up in diagonal lines of their small squares, and it also indicates both substrings are scrambled. Let's do the scramble, and get the below matrix. Meanwhile the both strings are exactly same, and what happens to the diagonal line, it comes from top left to down right as a complete diagonal line of the whole square that represents the target string. Let's call diagonal line square like now positive diagonal square, and the diagonal line from top right to down left as negative diagonal square.
```
  r    g   e   a   t
 +---+---+---+---+---+
r| x |   |   |   |   |
 +---+---+---+---+---+
g|   | x |   |   |   |
 +---+---+---+---+---+
e|   |   | x |   |   |
 +---+---+---+---+---+
a|   |   |   | x |   |
 +---+---+---+---+---+
t|   |   |   |   | x |
 +---+---+---+---+---+
         fig. 2
```
So the solution is to find 2 of the squares with corner by corner next to each other, and reverse the string representing the negative diagonal square into positive, which will leads to the squares represent the substrings can join into a longer scrambled string. And repeat to find such squares and merge them until we connect all squares into a full length of positive diagonal square that represent our target string. And that's the algorithm to our final solution. BTW, if we get a negative diagonal square is already a solution for us, you may look it as from another way to turn s2 into s1.

In fact, we don't have to to scramble the strings to convert the negative/positive diagonal into positive/negative, to save time, we just need to find them and mark the merged square as the positive/negative diagonal square. Now, let's check out our steps.

>[!NOTE]Steps:
>[^1]Find out all crosses.
>[^2]Find out the all the diagonal lines with the representing squares.
>[^3]Find the next to each square pairs and merge them into one positive/negative diagonal square.
>[^4]Check after merge to determine wether we have our goal, if yes, return true.
>[^5]Record the new diagonal square into our database and repeat step 3-5.
>[^6]If no luck to find any suitable squares, we return sorry.

To improve the performance, we have some tips for you.
>[!TIP]
>[^1]If in the matrix with a row or a column without any cross, it means we have a character not shown in the other string, should not be scrambled string pair.
>[^2]If the strings are composed by same set of characters with same quantities of each character, which means passed the tip 1 exam, and the length of the string less than 4, they should be scrambled.
>[^3]If the matrix of strings comes with positive/negative diagonal line or both, they are same string or reverse string pair and scrambled.(the positive diagonal line can easily determined by comparing the strings if equal)
>[^4]To find all diagonal lines by its length, we can scan the cross with two direction down-left or down-right to get all the diagonal lines according to the cross with its lengths.

It looks we can start for 1 and 2 steps.
In general, we will create a nxn matrix to store the state of the crosses, and find another data structure to archive the cross related diagonal lines information like possible lengths and starting point position. Moreover, we also notice that the matrix would be sparse matrix in most case. And we needs to figure out a more efficient data structure to store both information.
Let's mark the examining cross as "o" in fig. 3, and it should have the information of coordinations data (0,0) and lengths of 1,2,3,4,5, and it's all associated with x equals 0.
```
               Y
  +-------------------------->
  |    r    g   e   a   t
  |   +---+---+---+---+---+
  |  r| o |   |   |   |   |
  |   +---+---+---+---+---+
  |  g|   | x |   |   |   |
  |   +---+---+---+---+---+
X |  e|   |   | x |   |   |
  |   +---+---+---+---+---+
  |  a|   |   |   | x |   |
  |   +---+---+---+---+---+
  |  t|   |   |   |   | x |
  |   +---+---+---+---+---+
  V
             fig. 3
```

So we create an array to store all the information related to certain x row in our diagram is column. In this way, we have x[0] with a dynamic length array to representing coordination y and lengths.It should look like this:
```c_cpp
x[0]={(0,1),(0,1),(0,2),(0,3),(0,4),(0,5)}
```

Somehow, it's easy to understand and hard to manipulate by compute in quick way. So we redesigned the data structure like below. We can quickly access the x,y coordination and the length of the cross associated diagonal.
```c_cpp
x[0]={1,2,3,4,5}
y[0]={0,0,0,0,0}
```

The last piece of the puzzle is what would be the dynamic array with less space consumed data structure as well as the quick accessing speed for x[i] and y[i]. That's string! Let's create a n dimension string array for x[i] and y[i]. Now, let's code on the step 1 & 2 as below.

```c_cpp
int    n = src.length();
int    y_bit = 0;
bool   l_bit = true;
bool   r_bit = true;
string x_route[n], y_route[n];
string mergedBound;
for (int i = 0; i < n; ++i)
{
    x_route[i] = "";
    y_route[i] = "";
    for (int j = 0; j < n; ++j)
    {
        //l_bit & r_bit to check whether the strings have positive/negative diagonal line or both of the largest square
        r_bit &= (i != n - j - 1) || (src[i] == dst[j]);
        l_bit &= (i != j) || (src[i] == dst[j]);
        if (src[i] == dst[j])
        {
            x_route[i] += string(1, 1);
            y_route[i] += string(1, j);
            //y_bit check the strings have the characters not appearing in other string
            y_bit |= 1 << j;

            //Finding all existing negative diagonal lines
            if(i>0 && j>0 && src[i-1]==dst[j+1]){
                int x=i-1;
                int y=j-1;
                while(x>0 && y>0 && src[x]==dst[y]){
                    //record the merged squares and DO NOT manipulate the merging later
                    mergedBound+=string(1,x)+string(1,y)+string(1,j-y+1)+":";
                    x_route[x]+=string(1,j-y+1);
                    y_route[x]+=string(1,y);
                    x--;y--;
                }
            }

            //Finding all existing positive diagonal lines
            if(i<n-1 && j<n-1 && src[i+1]==dst[j+1]){
                int x=i+1;
                int y=j+1;
                while(x<n-1 && y<n-1 && src[x]==dst[y]){
                    //record the merged squares and DO NOT manipulate the merging later
                    mergedBound+=string(1,i)+string(1,j)+string(1,y-j+1)+":";
                    x_route[i]+=string(1,y-j+1);
                    y_route[i]+=string(1,j);                            
                    x++;y++;
                }                        
            }
        }
    }

    //y_bit check the strings have the characters not appearing in other string
    if (x_route[i].length() < 1)
    {
        return false;
    }
}
//some characters missed in target string or source string
if (y_bit != (1 << n) - 1)
{
    return false;
}
//Eureka! we found some diagonal lines in the largest square.
if (l_bit || r_bit)
{
    return true;
}
```

Further more, let's optimize the code for space and performance by replacing the string for dynamic array to fix length char array and merged squares string for with hash table/unordered_map. Why is unordered_map? Answer is performance is far better than set/unordered_set.

```c_cpp
int                          n = src.length();
int                          y_bit = 0;
bool                         l_bit = true;
bool                         r_bit = true;
unordered_map<unsigned,char> merged;
while(n>>y_bit++);                                  //calculate the max length of the dynamic array
char x_route[n][n*y_bit], y_route[n][n*y_bit];      //set the max. length of dynamic array with n*log2(n)
char index[n];                                      //store the length of the dynamic array length of each X row
//simple check on the strings to ensure not to waste time on simple case computing
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
 
```

Now, we come to step 3-6, finding the adjacent squares that can be merged. Assume we have a square at (x1,y1) with side length of l1, if there is square at(x2,y2) with length of l2 fits the merging requirements. Simple calculation tells us:x2=x1+l1, and max(y1+l1,y2+l2)-min(y1,y2)=l1+l2. And when we standing on x row x1, then we needs only check all the squares of x1+l1 data array have any feasible squares. If found, creating a new record for x row x[x1], and put it into merged set to avoid duplicating efforts later. The code should looks like below. And we repeat doing this for all x rows/columns, if we found any l1+l2==n and return true. Otherwise, tell caller sorry.
```c_cpp
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
                }
            }
        }
        ++ix1;
    }
}

return false;
```

Some explanations on why we start backwards and why start from n-3.
* Why we start backwards
  When we start backwards, all merged squares are in the right side and even they could be merged with any squares before current x row/column, we can handle it later while we backward to that points. It reduces the efforts to rewind the calculation in another round. Making us to merging all feasible squares in one loop.
* Why start from n-3
  As the previous scanning while the NxN loop, we get all obvious positive/negative diagonal lines with different lengths. So in the column n-2 & n-1, only have length 1 or 2 squares, which is very obvious ones that can be found by the NxN loop. We could save the time for checking. And from n-3 column the square side length comes to 3, we would face the fig. 4 situation, which cannot be found by the NxN loop.

```
           Y
  +------------------->
  |  +---+---+---+
  |  | x |   |   |
  |  +---+---+---+
X |  |   |   | x |
  |  +---+---+---+
  |  |   | x |   |
  |  +---+---+---+
  V
         fig. 4
```

Ok, we have reviewed all major parts of the solution, and let's put everything together, and that's the complete solution of mine.
```c_cpp
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
```
