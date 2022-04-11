//BASE CODE ADAPTED FROM https://github.com/bryanjsanchez/ConnectFour/blob/master/connectfour.py
#include <iostream>
#include <string.h>
#include <vector>

#define ROW 6
#define COL 7
#define LIMIT 3
#define AI 'R'
#define PLAYER 'B'
#define EMPTY 'O'

#define setO(a) (memset(a,EMPTY,sizeof(a)))
#define set0(a) (memset(a,0,sizeof(a)));

using namespace std;

class Board {
    public:
        char board[ROW][COL];
        int colTokens[COL];
        int AIpts, PLAYERpts, totalPts;

        Board() {
            setO(this->board);
            set0(colTokens);
            AIpts = 0; PLAYERpts = 0, totalPts = 0;
        }

        Board copy() {
            Board copy;

            copy.AIpts     = this->AIpts;
            copy.PLAYERpts = this->PLAYERpts;
            copy.totalPts  = this->totalPts;

            // copy board
            for (int i = 0; i < ROW; i++) {
                for (int j = 0; j < COL; j++) {
                    copy.board[i][j] = this->board[i][j];
                }
            }
            // copy colTokens
            for (int i = 0; i < COL; i++) {
                copy.colTokens[i] = this->colTokens[i];
            }
            return copy;
        }
};

//class related functions
bool  updateBoard(Board* board, int col, char player);
bool  verifyWin(Board board, char player);
bool  isTerminal(Board board);
bool  isValidCol(Board board, int col);
int   score(Board* board, char player);
Board cloneAndPlace(Board, char player, int col);
vector<int> validPos(Board board);

//aux functions
char choose_starting_player();
int  choose_search_algorithm();
int  calcPoints(int n, int sum, char player);
void draw_board(Board board);

//implementation
int main() {
    Board board;
    draw_board(board);
    Board child1 = cloneAndPlace(board, AI, 1);
    draw_board(child1);
    /*
    char player = choose_starting_player();

    int moves = 0;
    int algo  = choose_search_algorithm();

    bool isFinished = false;
    bool turn       = (player == 'p');

    draw_board(board);

    int col;
    while (!isFinished) {
        if (turn) {
            printf("|\n| $ Your turn $\n");
            do {
                printf("| Input a column: ");
                scanf("%d", &col);
            }while (!updateBoard(&board, col, PLAYER));
        } else {
            printf("|\n| $ AI's turn $\n");

            col = 1; // (temporary) col will be equal to the decision of the desired algorithm 

            switch(algo) {
                case 1:
                    //col = do minimax
                    break;
                case 2:
                    //col = do alpha/beta
                    break;
                case 3:
                    //col = do mcts
                    break;
            }

            updateBoard(&board, col, AI);
        }
        isFinished = isTerminal(board);
        draw_board(board);
        turn = !turn;
    }
    (turn)? printf("|\n| AI Won!\n") : printf("|\n| You Won!\n");
    printf("|\n| $ THANKS FOR PLAYING $\n\n");*/
    return 0;
}

//class related functions
bool updateBoard(Board* board, int col, char player) {
    if (col <= 0 || col > COL || board->colTokens[col-1] >= 6) return false; // out of bounds
    int row = (ROW-1) - board->colTokens[col-1];
    board->board[row][col-1] = player;
    board->colTokens[col-1]++;
    return true; 
}

bool verifyWin(Board board, char player) {
    //check horizontal line
    for (int row = ROW-1; row >= 0; row--) {
        for (int col = 0; col <= LIMIT; col++) {
            if (board.board[row][col] == player && board.board[row][col+1] == player &&
                board.board[row][col+2] == player && board.board[row][col+3] == player)
                    return true;
        }
    }

    //check vertical line
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col < COL; col++) {
            if (board.board[row][col] == player && board.board[row-1][col] == player &&
                board.board[row-2][col] == player && board.board[row-3][col] == player)
                    return true;
        }
    }

    // left to right diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col <= LIMIT; col++) {
            if (board.board[row][col] == player && board.board[row-1][col+1] == player &&
                board.board[row-2][col+2] == player && board.board[row-3][col+3] == player)
                    return true;
        }
    }

    // right to left diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = COL-1; col >= LIMIT; col--) {
            if (board.board[row][col] == player && board.board[row-1][col-1] == player &&
                board.board[row-2][col-2] == player && board.board[row-3][col-3] == player)
                    return true;
        }
    }

    int i;
    for (i = 0; i < COL; i++) if (board.colTokens[i] != 5) break;
    if (i == COL) return true; // draw

    return false;
}

bool isTerminal(Board board) {
    vector<int> locations = validPos(board);
    return verifyWin(board, AI) || verifyWin(board, PLAYER) || locations.size() == 0;
}

bool isValidCol(Board board, int col) { return board.board[0][col-1] == EMPTY; }

int score(Board* board, char player) {
    int n = 0, sum = 0;
    int pWeigth = (player == AI)? 1 : -1;
    // check horizontal
    for (int row = ROW-1; row >= 0; row--) {
        for (int col = 0; col <= LIMIT; col++) {
            // still need to test this better
            for (int k = 0; k < 4; k++) {
                if (board->board[row][col+k] != player && board->board[row][col+k] != EMPTY) {
                    sum = 0;
                    n = 0;
                    break;
                } else if (board->board[row+k][col+k] == EMPTY) {
                    sum += calcPoints(n, sum, player);
                    n = 0;
                } else {
                    n++;
                }
            }
            if (n == 4) return 512*pWeigth; // winner found;
            else if (player == AI) board->AIpts += sum + calcPoints(n, sum, player);
            else if (player == PLAYER) board->PLAYERpts += sum + calcPoints(n, sum, player);
        }
    }

    // check vertical
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col < COL; col++) {
        }
    }

    // left to right diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col <= LIMIT; col++) {
        }
    }

    // right to left diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = COL-1; col >= LIMIT; col--) {
        }
    }
    return 0;
}

Board cloneAndPlace(Board board, char player, int col) {
    Board child = board.copy();
    updateBoard(&child, col, player);
    return child;
}

vector<int> validPos(Board board) {
    vector<int> validCols;
    for (int i = 1; i <= COL; i++) {
        if (isValidCol(board, i)) validCols.push_back(i);
    }
    return validCols;
}

// aux functions
char choose_starting_player() {
    char mode;
    printf("| AI is Red (R), You are blue (B)\n"
    "| -> p (Player)\n"
    "| -> a (AI)\n");

    do {
        printf("| Who starts: ");
        scanf(" %c", &mode);
    } while (mode != 'p' && mode != 'a');
    printf("|\n");
    return mode;
}

int choose_search_algorithm() {
    int mode;
    printf("| Choose algorithm for the AI:\n"
    "|\t1 - MinMax\n");
    
    do {
        printf("| algo: ");
        scanf("%d", &mode);
    } while (mode != 1);
    return mode;
}

int calcPoints(int n, int sum, char player) {
    int pWeight = (player == AI)? 1 : -1;
    switch (n) {
        case 1:
            return 1*pWeight;
            break;
        case 2:
            return 10*pWeight;
            break;
        case 3:
            return 50*pWeight;
            break;
    }
    return 0;
}

void draw_board(Board board) {
    printf("|\n|");
    // Column numbers
    for (int n = 0; n < COL; n++) {
        if (n == 0) printf(" %d ", n+1);
        else printf("  %d ", n+1);
    }
    printf("\n");

    //board
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (j == 0) printf("| %c |", board.board[i][j]);
            else printf(" %c |", board.board[i][j]);
        }
        printf("\n");
    }
}
