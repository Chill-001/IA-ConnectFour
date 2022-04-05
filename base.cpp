#include <iostream>
#include <string.h>

#define setO(a) (memset(a,'O', sizeof(a)))
#define set0(a) (memset(a,0,sizeof(a)))

#define SIZE 42
#define ROW 6
#define COL 7
#define AI_PIECE 'R'
#define PLAYER_PIECE 'B'

bool turn;
int nturn;
int algo;
char starter;

void print_initial_info();
/**
 * Class Board will have:
 *  - Values of the current configuration of the board
 *  - Values blue and red have the current amount of blue points and red points in the board
 */
class Board {
    private:
        char configuration[ROW][COL];
        int currentBalls[COL];
        int blue, red;

        /** 
         * searches if a certain piece is a the (x,y) of the current pos
         * @return it is at (x,y), returns 1, if it isn't, returns 0
        */
        int search(int y, int x, int addy, int addx, char piece);

    public:
        // Constructor
        Board();

        // Methods
        bool isFinished();

        void play();
        // update board putting the token at the collumn indicated
        // returns true when is possible to put the token at given position
        bool update(int col);
        void printBoard();
        void winner(char winner);
};

int main() {
    Board board;

    print_initial_info();

    turn = (starter == 'c');

    int col;
    nturn = 0;
    while (!board.isFinished()) {
        if (turn) {
            printf("| Computer's turn: \n");
            board.play();
        } else {
            // player plays
            printf("| Your turn\n");
            do {
                printf("| Choose a column: ");
                scanf("%d", &col);
            } while (!board.update(col));
            printf("|\n| Your play:\n");
        }
        turn = !turn;
        nturn++;
        board.printBoard();
        printf("|\n");
    }

    return 0;
}

Board::Board() {
    setO(configuration);
    set0(currentBalls);
    blue = 0;
    red = 0;
}

// Way more effective starting from below
bool Board::isFinished() {
    char cur;
    if (turn) cur = AI_PIECE;
    else cur = PLAYER_PIECE;
    for (int i = ROW-1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            if (configuration[i][j] == AI_PIECE || configuration[i][j] == PLAYER_PIECE) {
                if (search(i,j,1,0,cur) == 4 || search(i,j,0,1,cur) == 4 || search(i,j,-1,-1,cur) == 4 || search(i,j,1,-1,cur) == 4 || search(i,j,0,-1,cur) == 4) {
                    winner(cur);
                    return true;
                }
            }
        }
    }
    return false;
}

void Board::winner(char winner) {
    bool win = (winner == AI_PIECE);
    if (win) printf("| Computer Won!\n");
    else printf("| Player Won!\n");
}

int Board::search(int y, int x, int addy, int addx, char piece) {
    if (x+addx < 0 || x+addx >= COL || y+addy < 0 || y+addy >= ROW) return 0;
    if (configuration[y+addy][x+addx] != piece) return 1;
    return 1+search(x+addx, y+addy, addx, addy, piece);
}

void Board::play() {
    if (nturn == 0) update(4);
}

bool Board::update(int col) {
    if (col < 1 || col > 7 || currentBalls[col-1] >= 6) {
        printf("| Invalid position\n");
        return false;
    }

    int row = 5-currentBalls[col-1];
    //cannot put anymore tokens on col
    if (row < 0) return false;
    //computer or player?
    if (turn) {
        configuration[row][col-1] = AI_PIECE;
    } else {
        configuration[row][col-1] = PLAYER_PIECE;
    }
    currentBalls[col-1]++;
    return true;
}

void Board::printBoard() {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("| %c |", configuration[i][j]);
        }
        printf("\n");
    }
}

void print_initial_info() {
    printf("| Player will always be blue (B), computer is red (R)\n"
    "| Define who starts the game \n|\n"
    "| p - Player Starts\n"
    "| c - Computer Starts\n");
    do {
        printf("| Who starts: ");
        scanf(" %c[^\n]", &starter);
    } while (starter != 'c' && starter != 'p');
    printf("|\n");

    printf("| Choose the algorithm for the AI:\n"
    "|\t1 - MinMax\n"
    "|\t2 - Alpha-Beta\n"
    "|\t3 - MCTS\n");
    do {
        printf("| Enter an option: ");
        scanf("%d", &algo);
    } while (algo < 1 || algo > 3);
    printf("|\n");
}
