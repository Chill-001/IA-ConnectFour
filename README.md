# IA-ConnectFour
Project from an Artificial Inteligence class.
Group members:
  - Hugo Silva 202004447
  - João Malheiro 202004449
  - Tomás Ferreira 202006594
  
 Description:
  - The project consists in creating an interactive version of the connect four game (https://en.wikipedia.org/wiki/Connect_Four);
  - The player will be versing the computer, and the computer will be calculating the best move to take from the play done by the player.
  - Game is won once the player or the computer have an horizontal/vertical/diagonal composition of 4 pieces.
  - If there aren't any compositions of 4 pieces and the board is full, we call the draw.

 Computer:
  - For the AI will be using:
        - MinMax (https://en.wikipedia.org/wiki/Minimax);
        - Alpha-Beta (https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning);
        - MCTS (https://en.wikipedia.org/wiki/Monte_Carlo_tree_search);
  - We also will limit the depth of the search so the player has a chance to win and to make the game overall more fun and fair.
  
 Input/Output:
  - As said earlier the game is supposed to be quite interactive, so everything you need to input the program will tell you
  - As for the Output, once every play occurs the current board configuration will be shown, and the move taken by the computer, if it's his turn.
 
 Compilation:
  - The source code is in C++ so a simple <g++ -Wall file.cpp> should suffice.
  - Being that this is interactive, the code will not support file reading so, things like <g++ -Wall file.cpp plays.txt> will not be supported. 
