#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <stack>
using namespace std;



int trap(vector<int>& height) {
    int ret = 0;
    stack<int> stk;
    for(int i=0; i<height.size(); i++)
    {
        while(stk.size() && height[stk.top()]<height[i])
        {
            int cur = stk.top();
            stk.pop();
            if(!stk.size()) break;

            int left = stk.top();
            ret += (min(height[left], height[i]) - height[cur]) * (i - left-1);    
        }
        stk.push(i);
    }

    return ret;
}


int main()
{
    vector<int> height = {0,1,0,2,1,0,1,3,2,1,2,1};

    printf("%d \n", trap(height));


    return 0;
}