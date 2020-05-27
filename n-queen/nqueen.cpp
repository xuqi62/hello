#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

using namespace std;

static int64_t get_now_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_usec + tv.tv_sec*1000000ll;
}

//判断在cur[row][col]位置放一个皇后，是否是合法的状态
//已经保证了每行一个皇后，只需要判断列是否合法以及对角线是否合法。
bool isValid(vector<string> &cur, int row, int col)
{
    int n = cur.size();
    // printf("======(%d %d)============= \n", row, col);
    // for(int k=0; k<n; k++)
    // {
    //     for(int j=0; j<n; j++)
    //         printf("%c ", cur[k][j]);
    //     printf("\n");
    // }
    // printf("=================== \n");
    //列
    for(int i = 0; i < row; i++)
        if(cur[i][col] == 'Q')
            return false;

    //右对角线(只需要判断对角线上半部分，因为后面的行还没有开始放置)
    for(int i = row-1, j=col-1; i >= 0 && j >= 0; i--,j--)
        if(cur[i][j] == 'Q')
            return false;

    //左对角线(只需要判断对角线上半部分，因为后面的行还没有开始放置)
    for(int i = row-1, j=col+1; i >= 0 && j < cur.size(); i--,j++)
        if(cur[i][j] == 'Q')
            return false;

    return true;
}

void DFS(vector<string> vec, int row, vector<vector<string> >& ret)
{
    int n = vec.size();
    if(row == n)
    {
        ret.push_back(vec);
        return;
    }

    for(int i=0; i<n; i++)
    {
        if(isValid(vec, row, i) == true)
        {
            vec[row][i] = 'Q';
            DFS(vec, row+1, ret);
            vec[row][i] = '.';
        }
    }
}

vector<vector<string> > solveNQueens(int n) {
    vector<vector<string> > ret;

    vector<string> vec(n, string(n, '.'));
    DFS(vec, 0, ret);

    return ret;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("./test n \n");
        return -1;
    }

    printf("====> n: %d \n", atoi(argv[1]) );
    int64_t start = get_now_us();
    vector<vector<string> > ret;
    for(int i=1; i<10; i++)
        ret = solveNQueens(i);
    int64_t end = get_now_us();
    printf("time: % " PRId64 " \n", end-start);
    printf("reture size: %d \n", (int)ret.size());


    return 0;
}
