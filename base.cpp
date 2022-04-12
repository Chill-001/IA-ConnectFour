/**
 * CONNECT FOUR BY Hugo Silva, João Malheiro, Tomás Ferreira
 * The base code of the functions and classes was heavily inspired by: https://github.com/bryanjsanchez/ConnectFour
 * Project for AI class 
 */
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

/**
 * Board class
 * @param board the literal representation of the game board
 * @param colTokens has the information about the amount of tokens each column of the board has
 * @param (AI/PLAYERpts) represent the amount of points each player has on a certain board
 */
class Board {
    public:
        char board[ROW][COL];
        int colTokens[COL];
        int AIpts, PLAYERpts;

        /// Simple Constructor
        Board() {
            setO(this->board);
            set0(colTokens);
            AIpts = 0; PLAYERpts = 0;
        }

        /// Copies the current board to a new one (reseting points)
        Board copy() {
            Board copy;

            copy.AIpts     = 0;
            copy.PLAYERpts = 0;

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
/**
 * puts the given player token on the given column of a given board
 * @return true if it was possible to update, false otherwise 
 */
bool  updateBoard(Board* board, int col, char player);

/**
 * Verifies if the current board is a terminal one where the ai or the player has won  
 * @return true if its a winning board, false otherwise
 */
bool  verifyWin(Board board, char player);

/**
 * Verifies if the board is a terminal board (win or draw) 
 * @return true if its terminal, false otherwise 
 */
bool  isTerminal(Board board);

/**
 * Verifies if the current column given is valid for the current board
 * @return true if column valid, false otherwise 
 */
bool  isValidCol(Board board, int col);

/// Calculates the points that the given player has on the given board
void   score(Board* board, char player);

/**
 * Clones the given board and puts a new token on the given column (generator of childs in short)
 * @return a new child board originated fromthe given board 
 */
Board cloneAndPlace(Board board, char player, int col);

/**
 * Given a board, calculates all the columns that can receive new tokens
 * @return vector<int> that has all the indexes possible to add tokens
 */
vector<int> validPos(Board board);

//aux functions
/// Simple IO function to read which player starts playing
char choose_starting_player();

/// Simple IO function to read which algorithm will be used
int  choose_search_algorithm();

/// Simple IO function to read how many searches the search algorithm needs to do
int  choose_search_depth();

/// To be used by score, returns amount of points depending on n value
int  calcPoints(int n, int sum, char player);

/// Simple IO function to draw the board
void draw_board(Board board);

//implementation
int main() {
    Board board;
    char player = choose_starting_player();
    int algo    = choose_search_algorithm();
    int depth   = choose_search_depth(); 

    int moves = 0;
    bool isFinished = false;
    bool turn       = (player == 'p');

    printf("| -=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    draw_board(board);

    int col;
    while (!isFinished) {
        if (turn) {
            printf("|\n| $ Your turn $\n");
            do {
                printf("| Input a column: ");
                scanf("%d", &col);
            }while (!updateBoard(&board, col, PLAYER));
            score(&board, PLAYER);
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
            score(&board, AI);
        }
        isFinished = isTerminal(board);
        draw_board(board);
        turn = !turn;
        moves++;
    }
    (turn)? printf("|\n| AI Won!\n") : printf("|\n| You Won!\n");
    printf("| %d Moves done!\n", moves);
    printf("|\n| $ THANKS FOR PLAYING $\n\n");
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
    return false;
}

bool isTerminal(Board board) {
    vector<int> locations = validPos(board);
    return verifyWin(board, AI) || verifyWin(board, PLAYER) || locations.size() == 0;
}

bool isValidCol(Board board, int col) { return board.board[0][col-1] == EMPTY; }

void score(Board* board, char player) {
    int sum = 0, n = 0;
    // check horizontal
    for (int row = ROW-1; row >= 0; row--) {
        for (int col = 0; col <= LIMIT; col++) {
            for (int k = 0; k < 4; k++) {
                if (board->board[row][col+k] != player && board->board[row][col+k] != EMPTY) {
                    sum = 0;
                    n = 0;
                    break;
                } else if (board->board[row][col+k] == EMPTY) {
                    sum += calcPoints(n, sum, player);
                    n = 0;
                } else {
                    n++;
                }
            }
            if (n == 4) {
                if (player == AI) board->AIpts = 512;
                else board->PLAYERpts = -512;
                return;
            } // winner found;
            else if (player == AI) board->AIpts += sum + calcPoints(n, sum, player);
            else if (player == PLAYER) board->PLAYERpts += sum + calcPoints(n, sum, player);
            sum = 0; n = 0;
        }
    }

    // check vertical
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col < COL; col++) {
            for (int k = 0; k < 4; k++) {
                if (board->board[row-k][col] != player && board->board[row-k][col] != EMPTY) {
                    sum = 0;
                    n = 0;
                    break;
                } else if (board->board[row-k][col] == EMPTY) {
                    sum += calcPoints(n, sum, player);
                    n = 0;
                } else {
                    n++;
                }
            }
            if (n == 4) {
                if (player == AI) board->AIpts = 512;
                else board->PLAYERpts = -512;
                return;
            } // winner found;
            else if (player == AI) board->AIpts += sum + calcPoints(n, sum, player);
            else if (player == PLAYER) board->PLAYERpts += sum + calcPoints(n, sum, player);
            sum = 0; n = 0;
        }
    }

    // left to right diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = 0; col <= LIMIT; col++) {
            for (int k = 0; k < 4; k++) {
                if (board->board[row-k][col+k] != player && board->board[row-k][col+k] != EMPTY) {
                    sum = 0;
                    n = 0;
                    break;
                } else if (board->board[row-k][col+k] == EMPTY) {
                    sum += calcPoints(n, sum, player);
                    n = 0;
                } else {
                    n++;
                }
            }
            if (n == 4) {
                if (player == AI) board->AIpts = 512;
                else board->PLAYERpts = -512;
                return;
            } // winner found;
            else if (player == AI) board->AIpts += sum + calcPoints(n, sum, player);
            else if (player == PLAYER) board->PLAYERpts += sum + calcPoints(n, sum, player);
            sum = 0; n = 0;
        }
    }

    // right to left diagonal
    for (int row = ROW-1; row >= LIMIT; row--) {
        for (int col = COL-1; col >= LIMIT; col--) {
            for (int k = 0; k < 4; k++) {
                if (board->board[row-k][col-k] != player && board->board[row-k][col-k] != EMPTY) {
                    sum = 0;
                    n = 0;
                    break;
                } else if (board->board[row-k][col-k] == EMPTY) {
                    sum += calcPoints(n, sum, player);
                    n = 0;
                } else {
                    n++;
                }
            }
            if (n == 4) {
                if (player == AI) board->AIpts = 512;
                else board->PLAYERpts = -512;
                return;
            } // winner found;
            else if (player == AI) board->AIpts += sum + calcPoints(n, sum, player);
            else if (player == PLAYER) board->PLAYERpts += sum + calcPoints(n, sum, player);
            sum = 0; n = 0;
        }
    }
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
    "|\t1 - MinMax\n"
    "|\t2 - Alpha-Beta\n"
    "|\t3 - MCTS\n");
    
    do {
        printf("| algo: ");
        scanf("%d", &mode);
    } while (mode < 1 || mode > 3);
    printf("|\n");
    return mode;
}

int  choose_search_depth() {
    int depth;
    printf("| Choose difficulty:\n"
    "|\t1 - Easy\n"
    "|\t2 - Normal\n"
    "|\t3 - Hard\n"
    "|\t4 - Very Hard\n");

    do {
        printf("| diff: ");
        scanf("%d", &depth);
    } while (depth < 1 || depth > 4);
    printf("|\n");
    return 3*depth;
}

int calcPoints(int n, int sum, char player) {
    int pWeight = (player == AI)? 1 : -1;
    switch (n) {
        case 1:
            return 1*pWeight;
        case 2:
            return 10*pWeight;
        case 3:
            return 50*pWeight;
    }
    return 0;
}

void draw_board(Board board) {
    printf("|");
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
    printf("| -=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}
