# Different Way to Solve the Scramble String Problem

Before we go further into the problem, let's put the strings into 2 dimension matrix like below in Fig. 1 with example strings "great" and "rgeat". And cross out the same characters in the appropriate positions of the matrix.
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
         Fig. 1
```

We can quickly figure out the exact sequence substring or reverse sequence substring all line up in diagonal lines of their small squares, and it also indicates both substrings are scrambled. Let's do the scramble and get the matrix below. Meanwhile, both strings are precisely the same, and what happens to the diagonal line is that it comes from top left to down right as a complete diagonal line of the whole square representing the target string. Let's call the diagonal line square the positive diagonal square, and the diagonal line from top right to down left the negative diagonal square.
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
         Fig. 2
```
So the solution is to find 2 of the squares corner by corner next to each other and reverse the string representing the negative diagonal square into positive, leading to the squares representing the substrings joining into a longer scrambled string. Repeat to find and merge such squares until we connect them into a total length of the positive diagonal square representing our target string. And that's the algorithm for our final solution. BTW, if we get a negative diagonal square, it is already a solution for us. You may look at it as another way to turn s2 into s1.

In fact, we don't have to scramble the strings to convert the negative/positive diagonal into positive/negative. To save time, we need to find them and mark the merged square as the positive/negative diagonal square. Now, let's check out our steps.

>[!NOTE]
>Steps:
>1. Find out all the crosses.
>2. Find out all the diagonal lines with the representing squares.
>3. Find the next to each square pair and merge them into one positive/negative diagonal square.
>4. Check after merge to determine whether we have our goal. If yes, return true.
>5. Record the new diagonal square into our database and repeat steps 3-5.
>6. If we have no luck finding suitable squares, we return sorry.

To improve our performance, we have some tips for you.
>[!TIP]
>1. If in the matrix with a row or a column without any cross, it means we have a character not shown in the other string, which should not be a scrambled string pair.
>2. If the strings are composed of the same set of characters with the same quantities of each character, which means they passed the tip 1 exam, and the string length is less than 4, they should be scrambled.
>3. If the matrix of strings comes with a positive/negative diagonal line or both, they are the same string or reverse string pair and scrambled. (the positive diagonal line can easily determined by comparing the strings if equal)
>4. To find all diagonal lines by their length, we can scan the cross in two directions, down-left or down-right, to get all the diagonal lines according to the cross with its lengths.

It looks like we can start with 1 and 2 steps.
In general, we will create an NxN matrix to store the state of the crosses and find another data structure to archive the cross-related diagonal lines' information, such as possible lengths and starting point position. Moreover, we also notice that the matrix would be sparse matrix in most cases. We need to figure out a more efficient data structure to store both types of information.
Let's mark the examining cross as "o" in Fig. 3, and it should have the information of coordinations data (0,0) and lengths of 1,2,3,4,5, and it's all associated with x equals 0.
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
             Fig. 3
```

So, we create an array to store all the information related to a certain x row in our diagram, which is a column. In this way, we have x[0] with a dynamic length array to represent coordination y and lengths. It should look like this:
```c_cpp
x[0]={(0,1),(0,1),(0,2),(0,3),(0,4),(0,5)}
```

Somehow, it's easy to understand and hard to manipulate by computing in a quick way. So, we redesigned the data structure as shown below. We can quickly access the x and y coordination and the length of the cross-associated diagonal.
```c_cpp
x[0]={1,2,3,4,5}
y[0]={0,0,0,0,0}
```

The last piece of the puzzle is what would be the dynamic array with less space consumed data structure as well as the quick accessing speed for x[i] and y[i]. That's a string! Let's create a n-dimension string array for x[i] and y[i]. Now, let's code on the steps 1 & 2 as below.

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
        //l_bit & r_bit to check whether the strings have positive/negative diagonal lines or both of the largest square
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

    //y_bit check the strings have the characters not appearing in other strings
    if (x_route[i].length() < 1)
    {
        return false;
    }
}
//some characters are missed in the target string or source string
if (y_bit != (1 << n) - 1)
{
    return false;
}
//Eureka! We found some diagonal lines in the largest square.
if (l_bit || r_bit)
{
    return true;
}
```

Furthermore, let's optimize the code for space and performance by replacing the string for the dynamic array to fix the length char array, and the merged squares recording string with hash table/unordered_map. Why is unordered_map? The answer is the performance of unordered_map is far better than set/unordered_set.

```c_cpp
int                          n = src.length();
int                          y_bit = 0;
bool                         l_bit = true;
bool                         r_bit = true;
unordered_map<unsigned,char> merged;
while(n>>y_bit++);                                  //calculate the max length of the dynamic array
char x_route[n][n*y_bit], y_route[n][n*y_bit];      //set the max. length of a dynamic array with n*log2(n)
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

Now, we come to steps 3-6, finding the adjacent squares that can be merged. Assume we have a square at (x1,y1) with a side length of l1, if there is a square at(x2,y2) with a length of l2 fits the merging requirements. Simple calculation tells us:x2=x1+l1, and max(y1+l1,y2+l2)-min(y1,y2)=l1+l2. And when we standing on x row x1, then we need only check all the squares of x1+l1 data array have any feasible squares. If found, create a new record for x row x[x1], and put it into the merged set to avoid duplicating efforts later. The code should look like below. We repeat doing this for all x rows/columns if we find any l1+l2==n and return true. Otherwise, tell the caller sorry.
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

Some explanations on why we start backward and why start from n-3.
* Why we start backward
  When we start backward, all merged squares are on the right side and even if they could be merged with any squares before the current x row/column, we can handle it later while we backward to that points. It reduces the efforts to rewind the calculation in another round. Making us merge all feasible squares in one loop.
* Why start from n-3
  As in the previous scanning while the NxN loop, we get all obvious positive/negative diagonal lines with different lengths. So in the column, n-2 & n-1, only have length of 1 or 2 squares, which is very obvious ones that can be found by the NxN loop. We could save time for checking. From the n-3 column the square side length comes to 3, we would face the Fig. 4 situation, which cannot be found by the NxN loop.

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
         Fig. 4
```

Ok, we have reviewed all major parts of the solution, and let's put everything together, that's my complete solution of mine.
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

# Different Way to Solve the Scramble String Problem

Before we go further into the problem, let's put the strings into 2 dimension matrix like below in Fig. 1 with example strings "great" and "rgeat". And cross out the same characters in the appropriate positions of the matrix.
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
         Fig. 1
```

We can quickly figure out the exact sequence substring or reverse sequence substring all line up in diagonal lines of their small squares, and it also indicates both substrings are scrambled. Let's do the scramble and get the matrix below. Meanwhile, both strings are precisely the same, and what happens to the diagonal line is that it comes from top left to down right as a complete diagonal line of the whole square representing the target string. Let's call the diagonal line square the positive diagonal square, and the diagonal line from top right to down left the negative diagonal square.
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
         Fig. 2
```
So the solution is to find 2 of the squares corner by corner next to each other and reverse the string representing the negative diagonal square into positive, leading to the squares representing the substrings joining into a longer scrambled string. Repeat to find and merge such squares until we connect them into a total length of the positive diagonal square representing our target string. And that's the algorithm for our final solution. BTW, if we get a negative diagonal square, it is already a solution for us. You may look at it as another way to turn s2 into s1.

In fact, we don't have to scramble the strings to convert the negative/positive diagonal into positive/negative. To save time, we need to find them and mark the merged square as the positive/negative diagonal square. Now, let's check out our steps.

>[!NOTE]
>Steps:
>1. Find out all the crosses.
>2. Find out all the diagonal lines with the representing squares.
>3. Find the next to each square pair and merge them into one positive/negative diagonal square.
>4. Check after merge to determine whether we have our goal. If yes, return true.
>5. Record the new diagonal square into our database and repeat steps 3-5.
>6. If we have no luck finding suitable squares, we return sorry.

To improve our performance, we have some tips for you.
>[!TIP]
>1. If in the matrix with a row or a column without any cross, it means we have a character not shown in the other string, which should not be a scrambled string pair.
>2. If the strings are composed of the same set of characters with the same quantities of each character, which means they passed the tip 1 exam, and the string length is less than 4, they should be scrambled.
>3. If the matrix of strings comes with a positive/negative diagonal line or both, they are the same string or reverse string pair and scrambled. (the positive diagonal line can easily determined by comparing the strings if equal)
>4. To find all diagonal lines by their length, we can scan the cross in two directions, down-left or down-right, to get all the diagonal lines according to the cross with its lengths.

It looks like we can start with 1 and 2 steps.
In general, we will create an NxN matrix to store the state of the crosses and find another data structure to archive the cross-related diagonal lines' information, such as possible lengths and starting point position. Moreover, we also notice that the matrix would be sparse matrix in most cases. We need to figure out a more efficient data structure to store both types of information.
Let's mark the examining cross as "o" in Fig. 3, and it should have the information of coordinations data (0,0) and lengths of 1,2,3,4,5, and it's all associated with x equals 0.
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
             Fig. 3
```

So, we create an array to store all the information related to a certain x row in our diagram, which is a column. In this way, we have x[0] with a dynamic length array to represent coordination y and lengths. It should look like this:
```c_cpp
x[0]={(0,1),(0,1),(0,2),(0,3),(0,4),(0,5)}
```

Somehow, it's easy to understand and hard to manipulate by computing in a quick way. So, we redesigned the data structure as shown below. We can quickly access the x and y coordination and the length of the cross-associated diagonal.
```c_cpp
x[0]={1,2,3,4,5}
y[0]={0,0,0,0,0}
```

The last piece of the puzzle is what would be the dynamic array with less space consumed data structure as well as the quick accessing speed for x[i] and y[i]. That's a string! Let's create a n-dimension string array for x[i] and y[i]. Now, let's code on the steps 1 & 2 as below.

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
        //l_bit & r_bit to check whether the strings have positive/negative diagonal lines or both of the largest square
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

    //y_bit check the strings have the characters not appearing in other strings
    if (x_route[i].length() < 1)
    {
        return false;
    }
}
//some characters are missed in the target string or source string
if (y_bit != (1 << n) - 1)
{
    return false;
}
//Eureka! We found some diagonal lines in the largest square.
if (l_bit || r_bit)
{
    return true;
}
```

Furthermore, let's optimize the code for space and performance by replacing the string for the dynamic array to fix the length char array, and the merged squares recording string with hash table/unordered_map. Why is unordered_map? The answer is the performance of unordered_map is far better than set/unordered_set.

```c_cpp
int                          n = src.length();
int                          y_bit = 0;
bool                         l_bit = true;
bool                         r_bit = true;
unordered_map<unsigned,char> merged;
while(n>>y_bit++);                                  //calculate the max length of the dynamic array
char x_route[n][n*y_bit], y_route[n][n*y_bit];      //set the max. length of a dynamic array with n*log2(n)
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

Now, we come to steps 3-6, finding the adjacent squares that can be merged. Assume we have a square at (x1,y1) with a side length of l1, if there is a square at(x2,y2) with a length of l2 fits the merging requirements. Simple calculation tells us:x2=x1+l1, and max(y1+l1,y2+l2)-min(y1,y2)=l1+l2. And when we standing on x row x1, then we need only check all the squares of x1+l1 data array have any feasible squares. If found, create a new record for x row x[x1], and put it into the merged set to avoid duplicating efforts later. The code should look like below. We repeat doing this for all x rows/columns if we find any l1+l2==n and return true. Otherwise, tell the caller sorry.
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

Some explanations on why we start backward and why start from n-3.
* Why we start backward
  When we start backward, all merged squares are on the right side and even if they could be merged with any squares before the current x row/column, we can handle it later while we backward to that points. It reduces the efforts to rewind the calculation in another round. Making us merge all feasible squares in one loop.
* Why start from n-3
  As in the previous scanning while the NxN loop, we get all obvious positive/negative diagonal lines with different lengths. So in the column, n-2 & n-1, only have length of 1 or 2 squares, which is very obvious ones that can be found by the NxN loop. We could save time for checking. From the n-3 column the square side length comes to 3, we would face the Fig. 4 situation, which cannot be found by the NxN loop.

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
         Fig. 4
```

Ok, we have reviewed all major parts of the solution, and let's put everything together, that's my complete solution of mine.
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
# Different Way to Solve the Scramble String Problem

Before we go further into the problem, let's put the strings into 2 dimension matrix like below in Fig. 1 with example strings "great" and "rgeat". And cross out the same characters in the appropriate positions of the matrix.
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
         Fig. 1
```

We can quickly figure out the exact sequence substring or reverse sequence substring all line up in diagonal lines of their small squares, and it also indicates both substrings are scrambled. Let's do the scramble and get the matrix below. Meanwhile, both strings are precisely the same, and what happens to the diagonal line is that it comes from top left to down right as a complete diagonal line of the whole square representing the target string. Let's call the diagonal line square the positive diagonal square, and the diagonal line from top right to down left the negative diagonal square.
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
         Fig. 2
```
So the solution is to find 2 of the squares corner by corner next to each other and reverse the string representing the negative diagonal square into positive, leading to the squares representing the substrings joining into a longer scrambled string. Repeat to find and merge such squares until we connect them into a total length of the positive diagonal square representing our target string. And that's the algorithm for our final solution. BTW, if we get a negative diagonal square, it is already a solution for us. You may look at it as another way to turn s2 into s1.

In fact, we don't have to scramble the strings to convert the negative/positive diagonal into positive/negative. To save time, we need to find them and mark the merged square as the positive/negative diagonal square. Now, let's check out our steps.

>[!NOTE]
>Steps:
>1. Find out all the crosses.
>2. Find out all the diagonal lines with the representing squares.
>3. Find the next to each square pair and merge them into one positive/negative diagonal square.
>4. Check after merge to determine whether we have our goal. If yes, return true.
>5. Record the new diagonal square into our database and repeat steps 3-5.
>6. If we have no luck finding suitable squares, we return sorry.

To improve our performance, we have some tips for you.
>[!TIP]
>1. If in the matrix with a row or a column without any cross, it means we have a character not shown in the other string, which should not be a scrambled string pair.
>2. If the strings are composed of the same set of characters with the same quantities of each character, which means they passed the tip 1 exam, and the string length is less than 4, they should be scrambled.
>3. If the matrix of strings comes with a positive/negative diagonal line or both, they are the same string or reverse string pair and scrambled. (the positive diagonal line can easily determined by comparing the strings if equal)
>4. To find all diagonal lines by their length, we can scan the cross in two directions, down-left or down-right, to get all the diagonal lines according to the cross with its lengths.

It looks like we can start with 1 and 2 steps.
In general, we will create an NxN matrix to store the state of the crosses and find another data structure to archive the cross-related diagonal lines' information, such as possible lengths and starting point position. Moreover, we also notice that the matrix would be sparse matrix in most cases. We need to figure out a more efficient data structure to store both types of information.
Let's mark the examining cross as "o" in Fig. 3, and it should have the information of coordinations data (0,0) and lengths of 1,2,3,4,5, and it's all associated with x equals 0.
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
             Fig. 3
```

So, we create an array to store all the information related to a certain x row in our diagram, which is a column. In this way, we have x[0] with a dynamic length array to represent coordination y and lengths. It should look like this:
```c_cpp
x[0]={(0,1),(0,1),(0,2),(0,3),(0,4),(0,5)}
```

Somehow, it's easy to understand and hard to manipulate by computing in a quick way. So, we redesigned the data structure as shown below. We can quickly access the x and y coordination and the length of the cross-associated diagonal.
```c_cpp
x[0]={1,2,3,4,5}
y[0]={0,0,0,0,0}
```

The last piece of the puzzle is what would be the dynamic array with less space consumed data structure as well as the quick accessing speed for x[i] and y[i]. That's a string! Let's create a n-dimension string array for x[i] and y[i]. Now, let's code on the steps 1 & 2 as below.

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
        //l_bit & r_bit to check whether the strings have positive/negative diagonal lines or both of the largest square
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

    //y_bit check the strings have the characters not appearing in other strings
    if (x_route[i].length() < 1)
    {
        return false;
    }
}
//some characters are missed in the target string or source string
if (y_bit != (1 << n) - 1)
{
    return false;
}
//Eureka! We found some diagonal lines in the largest square.
if (l_bit || r_bit)
{
    return true;
}
```

Furthermore, let's optimize the code for space and performance by replacing the string for the dynamic array to fix the length char array, and the merged squares recording string with hash table/unordered_map. Why is unordered_map? The answer is the performance of unordered_map is far better than set/unordered_set.

```c_cpp
int                          n = src.length();
int                          y_bit = 0;
bool                         l_bit = true;
bool                         r_bit = true;
unordered_map<unsigned,char> merged;
while(n>>y_bit++);                                  //calculate the max length of the dynamic array
char x_route[n][n*y_bit], y_route[n][n*y_bit];      //set the max. length of a dynamic array with n*log2(n)
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

Now, we come to steps 3-6, finding the adjacent squares that can be merged. Assume we have a square at (x1,y1) with a side length of l1, if there is a square at(x2,y2) with a length of l2 fits the merging requirements. Simple calculation tells us:x2=x1+l1, and max(y1+l1,y2+l2)-min(y1,y2)=l1+l2. And when we standing on x row x1, then we need only check all the squares of x1+l1 data array have any feasible squares. If found, create a new record for x row x[x1], and put it into the merged set to avoid duplicating efforts later. The code should look like below. We repeat doing this for all x rows/columns if we find any l1+l2==n and return true. Otherwise, tell the caller sorry.
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

Some explanations on why we start backward and why start from n-3.
* Why we start backward
  When we start backward, all merged squares are on the right side and even if they could be merged with any squares before the current x row/column, we can handle it later while we backward to that points. It reduces the efforts to rewind the calculation in another round. Making us merge all feasible squares in one loop.
* Why start from n-3
  As in the previous scanning while the NxN loop, we get all obvious positive/negative diagonal lines with different lengths. So in the column, n-2 & n-1, only have length of 1 or 2 squares, which is very obvious ones that can be found by the NxN loop. We could save time for checking. From the n-3 column the square side length comes to 3, we would face the Fig. 4 situation, which cannot be found by the NxN loop.

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
         Fig. 4
```

Ok, we have reviewed all major parts of the solution, and let's put everything together, that's my complete solution of mine.
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
Finally, let's review the complexity of the code. NxN loop with searching the max diagonal lines, O(n^3). And the merging squares loop, O(n^2). In total, it's about O(n^3). 
