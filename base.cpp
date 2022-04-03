#include <iostream>
#include <string.h>

#define setO(a) (memset(a,'O', sizeof(a)))
#define set0(a) (memset(a,0,sizeof(a)))

#define SIZE 42
#define ROW 7
#define COL 6

bool turn;
/**
 * Class Board will have:
 *  - Values of the current configuration of the board
 *  - Values blue and red have the current amount of blue tokens and red tokens in the board
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
};

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
    if (turn) {
        configuration[currentBalls[col]*ROW + col];
    } else {

    }
    currentBalls[col]++;
    return;
}

int main() {
    Board board;
    char starter;

    printf("| Player will always be blue (B), computer is red (R)\n");
    printf("| Define who starts the game \n|\n");
    printf("| p - Player Starts\n");
    printf("| c - Computer Starts\n");
    printf("| input: ");
    scanf("%c", &starter);

    if (starter == 'c') turn = true;
    else turn = false;

    int col;
    while (!board.isFinished()) {
        if (turn) {
            board.play();
        } else {
            // player plays
            scanf("%d", &col);
            board.update(col);
        }
        turn = !turn;
    }

    return 0;
}