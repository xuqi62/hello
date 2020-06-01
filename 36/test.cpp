#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

bool isValidSudoku(vector<vector<char> >& board) {
    
    for(int i=0; i<board.size(); i++) {
        vector<int> vec(board.size(), 0);
        for(int j=0; j<board.size(); j++)
        {
            if(board[i][j] == '.') continue;
            if(vec[board[i][j]-'1']) return false;

            vec[board[i][j]-'1'] = 1;
        }
    }

    for(int i=0; i<board.size(); i++) {
        vector<int> vec(board.size(), 0);
        for(int j=0; j<board.size(); j++)
        {
            if(board[j][i] == '.') continue;
            if(vec[board[j][i]-'1']) return false;

            vec[board[j][i]-'1'] = 1;
        }
    }

    for(int i=0; i<board.size(); i++) {
        vector<int> vec(board.size(), 0);
        for(int j=0; j<board.size(); j++)
        {
            int x = i/3*3 + j/3;
            int y = (i%3)*3 + j%3;
            if(board[x][y] == '.') continue;

            if(vec[board[x][y] - '1']) return false;
            vec[board[x][y] - '1'] = 1;
        }
    }

    return true;
}

void df(vector<vector<char> >& board, );







int main()
{
    vector<vector<char> > board = {
        {'8', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '.', '.', '.', '.', '.', '6', '.'},
        {'1', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'4', '.', '.', '.', '6', '.', '.', '.', '.'},
        {'7', '.', '.', '.', '.', '.', '.', '.', '6'},
        {'.', '.', '.', '.', '2', '.', '.', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'},
    };

    printf(" vaild: %d \n", isValidSudoku(board));
    return 0;
}