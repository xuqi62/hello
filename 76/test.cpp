// 给你一个字符串 S、一个字符串 T，请在字符串 S 里面找出：包含 T 所有字符的最小子串。

// 示例：
// 输入: S = "ADOBECODEBANC", T = "ABC"
// 输出: "BANC"

// 说明：
//     如果 S 中不存这样的子串，则返回空字符串 ""。
//     如果 S 中存在这样的子串，我们保证它是唯一的答案。

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <limits.h>
using namespace std;

string minWindow(string s, string t)
{
    int res = INT_MAX;
    int start_pos = 0;
    unordered_map<char, int> window;
    unordered_map<char, int> needs;
    for(int i=0; i<t.size()-1; i++)
    {
        if(needs.count(t[i]) == 0)
            needs.insert(pair<char, int>{t[i], 1});
        else
        {
            needs[t[i]]++;
        }
    }

    int l=0;
    int r=0;
    int match_num = 0;
    while(r<s.size())
    {
        if(window.count(s[r]) == 0)
        {
            window.insert(pair<char, int>{s[r], 1});
        }
        else
        {
            window[s[r]]++;
        }
        if(window[s[r]] > needs[s[r]])
            match_num++;
        r++;

        while(match_num == needs.size())
        {
            if(r-l < res)
            {
                res = r-l;
                start_pos = l;
            }
            window[s[l]] --;
            if(window[s[l]] < needs[s[l]])
            {
                match_num--;
            }
            l++;
        }
    }

    return res == INT_MAX? "": s.substr(start_pos, res);

}

int main()
{
    string S = "ADOBECODEBANC";
    string T = "ABC";

    string res = minWindow(S, T);
    cout << res << endl;
    return 0;
}

