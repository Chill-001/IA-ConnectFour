#include <iostream>
#include <string.h>

#define setO(a) (memset(a,'O', sizeof(a)))
#define set0(a) (memset(a,0,sizeof(a)))

#define SIZE 42
#define ROW 7
#define COL 6

bool turn;
int algo;
char starter;

void print_initial_info();
/**
 * Class Board will have:
 *  - Values of the current configuration of the board
 *  - Values blue and red have the current amount of blue points and red points in the board
 */
class Board {
    public:
        char configuration[SIZE];
        int currentBalls[COL];
        int blue, red;

        // Constructor
        Board();

        // Methods
        bool isFinished();
        void play();
        void update(int col);
        void printBoard();
};

int main() {
    Board board;

    print_initial_info();

    turn = (starter == 'c');

    int col, i = 0;
    while (/*!board.isFinished()*/ i < 2) {
        if (turn) {
            printf("| Computer's turn: \n");
            board.play();
        } else {
            // player plays
            printf("| Your turn\n"
            "| Choose a column: ");
            scanf("%d", &col);
            board.update(col);
        }
        printf("| Turn [%d]\n", turn);
        printf("|\n");
        turn = !turn;
        i++;
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

bool Board::isFinished() {
    return true;
}

void Board::play() {
    return;
}

void Board::update(int col) {
    /*if (turn) {
        configuration[currentBalls[col]*ROW + col];
    } else {

    }
    currentBalls[col]++;*/
    return;
}

void Board::printBoard() {
    for (int i = 0; i < SIZE; i++) {
        if (i != 0 && i%7 == 0) printf("\n");
        printf("| %c |", configuration[i]);
    }
    printf("\n");
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
