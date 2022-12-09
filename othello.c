// included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "othello.h"

// main function
int main()
{
    // calls to welcomeScreen function
    welcomeScreen();

    // calls to clearScreen function
    clearScreen();

    // calls to playGame function
    playGame();

    // signals that program closed successfully
    return 0;
}

// outputs welcome screen
void welcomeScreen()
{
    // outputs game banner
    printf("\t\t OOOO  TTTTTT HH  HH EEEEEE LL     LL      OOOO  \n");
    printf("\t\tOO  OO   TT   HH  HH EE     LL     LL     OO  OO \n");
    printf("\t\tOO  OO   TT   HHHHHH EEEE   LL     LL     OO  OO \n");
    printf("\t\tOO  OO   TT   HH  HH EE     LL     LL     OO  OO \n");
    printf("\t\t OOOO    TT   HH  HH EEEEEE LLLLLL LLLLLL  OOOO  \n");
    printf("\n\n");

    // outputs game rules
    printf("OTHELLO GAME RULES:\n");
    printf("\t1. A square 8 x 8 board\n");
    printf("\t2. 64 discs colored black (X) on one side and white (O) on the opposite side.\n");
    printf("\t3. The board will start with 2 black discs (X) and 2 white discs (O) at the center of the board.\n");
    printf("\t4. They are arranged with black (X) forming a North-East to South-West direction. White (O) is forming a North-West to South-East direction\n");
    printf("\t5. The goal is to get the majority of color discs on the board at the end of the game.\n");
    printf("\t6. Each player gets 32 discs and black (X) always starts the game.\n");
    printf("\t7. The game alternates between white (O) and black (X) until one player can not make a valid move to outflank the opponent or both players have no valid moves\n");
    printf("\t8. When a player has no valid moves, they pass their turn and the opponent continues.\n");
    printf("\t9. A player cannot voluntarily forfeit their turn.\n");
    printf("\t10. When both players can not make a valid move the game ends\n");
}

// clears screen
void clearScreen()
{
    char enter;

    // prompts user for input
    printf("\n\t\t\t Hit <ENTER> to continue!\n");

    // holds for user input
    scanf("%c", &enter);

    // clears users screen
    system("cls");          // Windows implementation
    //system("clear");      // UNIX implementation
}

// handles the steps of playing the game
void playGame()
{
    // creates the two players using the Player struct
    struct Player playerX;
    struct Player playerO;
    // sets the current player to player x using the global constant
    int currentPlayer = PLAYER_X;
    int loop = ZERO;

    // a two-dimensional array representing the game board in rows and columns
    char board[ROW][COL];

    // gets player x's name
    printf("Player X, please enter your name:\n");
    scanf("%s", playerX.playerName);
    playerX.playChar = 'X';
    playerX.playerNum = PLAYER_X;
    playerX.discCount = 2;

    // gets player o's name
    printf("Player O, please enter your name:\n");
    scanf("%s", playerO.playerName);
    playerO.playChar = 'O';
    playerO.playerNum = PLAYER_O;
    playerO.discCount = 2;

    printf("%s and %s, let's play Othello!\n", playerX.playerName, playerO.playerName);

    // calls the initializeBoard function along with the current board
    initializeBoard(board);

    int endGame = FALSE;

    // loops until end of game
    while(endGame == FALSE)
    {
        // calls function to check if the games ended
        endGame = isEndGame(board);

        if(endGame == TRUE)
        {
            gameOver(board, playerX, playerO);
            break;
        }

        // calls the displayBoard function along with the current board
        displayBoard(board);

        // checks if the current player is player x
        if(currentPlayer == PLAYER_X)
        {
            // checks for a valid move for player x
            if(validMoveExists(board, playerX.playChar) == TRUE)
            {
                // calls function makeMove for player x
                makeMove(&playerX, board);

                // sets current player to be player o
                currentPlayer = PLAYER_O;
            } else
                // sets current player to be player o
                currentPlayer = PLAYER_O;
        }
        else if(currentPlayer == PLAYER_O)  // if the player isn't player x, player o plays their turn
        {
            // checks for a valid move for player o
            if(validMoveExists(board, playerO.playChar) == TRUE)
            {
                // calls function makeMove for player o
                makeMove(&playerO, board);

                // sets current player to be player x
                currentPlayer = PLAYER_X;
            } else
                // sets current player to be player x
                currentPlayer = PLAYER_X;
        }

        // updates the disc count for both players
        updateDiscCount(board, &playerX);
        updateDiscCount(board, &playerO);

        // displays the stats of both players
        displayStats(playerX);
        displayStats(playerO);
    }
}

// initializes the board by setting all board-spaces to spaces and assigning O and X's starting squares
void initializeBoard(char board[ROW][COL])
{
    // scrolls through the 2-dimensional array
    for(int i = 0; i < ROW; ++i)
        for(int j = 0; j < COL; ++j)
        {
            // checks if currently at O starting space, if it is then it assigns that space to O
            if((i == 3 && j == 3) || (i == 4 && j == 4))
                board[i][j] = 'O';
            // checks if currently at X starting space, if it is then it assigns that space to X
            else if((i == 3 && j == 4) || (i == 4 && j == 3))
                board[i][j] = 'X';
            // if is neither an O nor X starting point, assigns the board-space to space (' ')
            else
                board[i][j] = SPACE;
        }
}

// displays the current state of the game board
void displayBoard(char board[ROW][COL])
{
    // prints top row of board
    printf("|-----------------------------------------------------|\n");
    printf("|     |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |\n");
    printf("|-----------------------------------------------------|\n");

    // handles going down the rows
    for(int i = 0; i < ROW; ++i)
    {
        // numbers each row
        printf("|  %d  |", i + 1);

        // handles going down the columns
        for (int j = 0; j < COL; ++j)
        {
            // prints the corresponding space
            printf("  %c  |", board[i][j]);
        }

        // prints line at bottom of each row
        printf("\n|-----------------------------------------------------|\n");
    }
}

// handles the process of each player making a move
void makeMove(struct Player *player, char board[ROW][COL])
{
    char move[THREE];
    int valid = FALSE;

    // loop to check is players move is valid
    while(valid == FALSE)
    {
        // prompts player to make move
        printf("%s, please enter your move (e.g. B6)\n", player->playerName);

        // stores players move
        scanf("%s", move);

        // prompts user with their move
        printf("You entered: %s\n", move);

        // sets an integer, length, to how many characters is used in move
        int length;
        length = (int)strlen(move);

        if(length == TWO)   // insures 2 characters were used in move and sets valid to true
            valid = isValid(move, board, player);   // makes sure the square's valid
        else                // if two characters weren't used, keeps valid to false
            valid = FALSE;

        if(valid == FALSE)  // if move wasn't valid, it lets the player know before starting the loop again
            printf("Your move input was not valid\n\n");
        else if(valid == TRUE)
            printf("Your move input was valid\n\n");
    }
}

// checks if move is valid and returns the integer "valid"
int isValid(char move[THREE], char board[ROW][COL], struct Player *player)
{
    int valid = FALSE;

    // checks if board space exists and is empty
    if(isOpen(move, board))
        valid = isOutFlank(move, board, player->playChar);

    // if the board space exists and is empty, it updates the board
    if(valid == TRUE)
        updateBoard(move, board, player);

    return valid;
}

// checks that board space exists and is empty
int isOpen(char move[THREE], char board[ROW][COL])
{
    int open = FALSE;

    // gets the row and column of move
    int rowInt = getMoveRow(move);
    int colInt = getMoveCol(move);

    // makes sure moves are valid, and if they are gives open a value of true
    if(rowInt != INVALID && colInt != INVALID && board[rowInt][colInt] == SPACE)
        open = TRUE;
    else
        open = FALSE;

    return open;
}

// gets which row the players move is in
int getMoveRow(char move[THREE])
{
    int row;

    // assigns row with the players move at index[1]
    row = move[1] - '0'; // the - '0' is to make move[1] accurate to its int form instead of char

    // checks if players move is between 1-8
    if(row > 0 && row < 9)
        --row;  // if it is, increments row down by one to line up with arrays index
    else
        row = INVALID;  // if not, sets row to invalid

    return row;
}

// gets which column the players move is in
int getMoveCol(char move[THREE])
{
    char col;
    int colNum;

    // sets col to player's move at index 0
    col = move[0];

    // checks which letter is player's move, assigns colNum to index accordingly
    switch(tolower(col))
    {
        case 'a':
            colNum = 0;
            break;
        case 'b':
            colNum = 1;
            break;
        case 'c':
            colNum = 2;
            break;
        case 'd':
            colNum = 3;
            break;
        case 'e':
            colNum = 4;
            break;
        case 'f':
            colNum = 5;
            break;
        case 'g':
            colNum = 6;
            break;
        case 'h':
            colNum = 7;
            break;
        default:
            colNum = INVALID;   // if letter doesn't exist, sets colNum to invalid
    }

    return colNum;
}

// simply displays the players Information
void displayStats(struct Player player)
{
    printf("Players name:\t\t\t%s\n", player.playerName);
    printf("Players number:\t\t\t%d\n", player.playerNum);
    printf("Players disc count:\t\t%d\n", player.discCount);
    printf("Players character:\t\t%c\n\n", player.playChar);
}

// checks if move flanks opponent (honestly kind of useless since check horizontal and vertical already do this)
int isOutFlank(char move[THREE], char board[ROW][COL], char playChar)
{
    // gets moves
    int flank = FALSE;
    int rowInt = getMoveRow(move);
    int colInt = getMoveCol(move);

    // checks horizontal, vertical, and diagonal and sees if they return a disc count greater than 0
    if(checkHorizontal(rowInt, colInt, board, playChar) > 0)
        flank = TRUE;

    if(checkVertical(rowInt, colInt, board, playChar) > 0)
        flank = TRUE;

    if(checkDiagonal(rowInt, colInt, board, playChar) > 0)
        flank = TRUE;

    return flank;
}

// checks the horizontal axis for a valid move and flank, if it detects one it counts the new disc count and returns it
int checkHorizontal(int row, int col, char board[ROW][COL], char playChar)
{
    int discCount = 0;
    int flank = FALSE;

    // if there's a space to the left, it stops searching and flank stays false,
    // if there's a character the same as the players, it stops searching and flank becomes true
    for(int i = 1; col - i >= 0; ++i) {
        if (board[row][col - i] == SPACE)
            break;

        if (board[row][col - i] == playChar) {
            flank = TRUE;
            break;
        }
    }

    // if it was a flank, this goes through the row and counts all the discs that are going to be switched
    for(int i = 1; col - i >= 0; ++i) {
        if(flank == TRUE && board[row][col - i] != playChar && board[row][col - i] != SPACE)
            ++discCount;
    }

    flank = FALSE;

    // if there's a space to the right, it stops searching and flank stays false,
    // if there's a character the same as the players, it stops searching and flank becomes true
    for(int i = 1; col + i < COL; ++i) {
        if (board[row][col + i] == SPACE)
            break;

        if (board[row][col + i] == playChar) {
            flank = TRUE;
            break;
        }
    }

    // if it was a flank, this goes through the row and counts all the discs that are going to be switched
    for(int i = 1; col + i < COL; ++i)
    {
        if (flank == TRUE && board[row][col + i] != playChar && board[row][col + i] != SPACE)
            ++discCount;
    }

    return discCount;
}

// checks the vertical axis for a valid move and flank, if it detects one it counts the new disc count and returns it
int checkVertical(int row, int col, char board[ROW][COL], char playChar)
{
    int discCount = 0;
    int flank = FALSE;

    // if there's a space below, it stops searching and flank stays false,
    // if there's a character the same as the players, it stops searching and flank becomes true
    for(int i = 1; row - i >= 0; ++i) {
        if (board[row - i][col] == SPACE)
            break;

        if (board[row - i][col] == playChar) {
            flank = TRUE;
            break;
        }
    }

    // if it was a flank, this goes through the row and counts all the discs that are going to be switched
    for(int i = 1; row - i >= 0; ++i) {
        if (flank == TRUE && board[row - i][col] != playChar && board[row - i][col] != SPACE)
            ++discCount;
    }

    flank = FALSE;

    // if there's a space above, it stops searching and flank stays false,
    // if there's a character the same as the players, it stops searching and flank becomes true
    for(int i = 1; row + i < ROW; ++i) {
        if (board[row + i][col] == SPACE)
            break;

        if (board[row + i][col] == playChar) {
            flank = TRUE;
            break;
        }
    }

    // if it was a flank, this goes through the row and counts all the discs that are going to be switched
    for(int i = 1; row + i < ROW; ++i) {
        if (flank == TRUE && board[row + i][col] != playChar && board[row + i][col] != SPACE)
            ++discCount;
    }

    if(discCount > 0)
        return discCount;

    return 0;
}

// checks the diagonal axis for a valid move and flank, if it detects one it counts the new disc count and returns it
int checkDiagonal(int row, int col, char board[ROW][COL], char playChar)
{
    int discCount = 0;
    int flank = FALSE;

    // checks the bottom right of selected move
    for(int i = 1; row + i < ROW && col + i < COL; ++i)
    {
        // if it's a space, stop searching
        if(board[row + i][col + i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row + i][col + i] == playChar)
        {
            flank = TRUE;
            break;
        }

        // tally's all new disc that can be changed
        if(board[row + i][col + i] != ' ' && board[row + i][col + i] != playChar)
            ++discCount;
    }

    // checks top left of selected move
    for(int i = 1; row - i >= 0 && col - i >= 0; ++i)
    {
        // if it's a space, stop searching
        if(board[row - i][col - i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row - i][col - i] == playChar)
        {
            flank = TRUE;
            break;
        }

        // tally's all new disc that can be changed
        if(board[row - i][col - i] != ' ' && board[row - i][col - i] != playChar)
            ++discCount;
    }

    // checks bottom left of selected move
    for(int i = 1; row + i < ROW && col - i >= 0; ++i)
    {
        // if it's a space, stop searching
        if(board[row + i][col - i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row + i][col - i] == playChar)
        {
            flank = TRUE;
            break;
        }

        // tally's all new disc that can be changed
        if(board[row + i][col - i] != ' ' && board[row + i][col - i] != playChar)
            ++discCount;
    }

    // checks top right of selected move
    for(int i = 1; row - i >= 0 && col + i < COL; ++i)
    {
        // if it's a space, stop searching
        if(board[row - i][col + i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row - i][col + i] == playChar)
        {
            flank = TRUE;
            break;
        }

        // tally's all new disc that can be changed
        if(board[row - i][col + i] != ' ' && board[row - i][col + i] != playChar)
            ++discCount;
    }

    // returns disc count if it's a flank and the count is greater than 0
    if(discCount > 0 && flank == TRUE)
        return discCount;

    return 0;
}

// updates the game board
void updateBoard(char move[THREE], char board[ROW][COL], struct Player *player)
{
    // gets the row and column from the move
    int rowInt = getMoveRow(move);
    int colInt = getMoveCol(move);

    // checks horizontal, vertical, and diagonal axis then updates them
    if(checkHorizontal(rowInt, colInt, board, player->playChar) > 0)
        updateHorizontal(rowInt, colInt, board, player);

    if(checkVertical(rowInt, colInt, board, player->playChar) > 0)
        updateVertical(rowInt, colInt, board, player);

    if(checkDiagonal(rowInt, colInt, board, player->playChar) > 0)
        updateDiagonal(rowInt, colInt, board, player);
}

// updates the horizontal axis with new pieces
void updateHorizontal(int row, int col, char board[ROW][COL], struct Player *player)
{
    board[row][col] = player->playChar;

    // checks the left if it's neither the players own character nor a space and changes them to the players character
    for(int i = 1; col - i >= 0; ++i) {
        if(board[row][col - i] != player->playChar && board[row][col - i] != SPACE)
            board[row][col - i] = player->playChar;
    }

    // checks the right if it's neither the players own character nor a space and changes them to the players character
    for(int i = 1; col + i < COL; ++i) {
        if(board[row][col + i] != player->playChar && board[row][col + i] != SPACE)
            board[row][col + i] = player->playChar;
    }
}

// updates the vertical axis with new pieces
void updateVertical(int row, int col, char board[ROW][COL], struct Player *player)
{
    board[row][col] = player->playChar;

    // checks above and if it's neither the players own character nor a space and changes them to the players character
    for(int i = 1; row - i >= 0; ++i) {
        if(board[row - i][col] != player->playChar && board[row - i][col] != SPACE)
            board[row - i][col] = player->playChar;
    }

    // checks below and if it's neither the players own character nor a space and changes them to the players character
    for(int i = 1; row + i < ROW; ++i) {
        if(board[row + i][col] != player->playChar && board[row + i][col] != SPACE)
            board[row + i][col] = player->playChar;
    }
}

// updates the diagonal axis with new pieces
void updateDiagonal(int row, int col, char board[ROW][COL], struct Player *player)
{
    int flank = FALSE;

    board[row][col] = player->playChar;

    // checks the bottom right of selected move
    for(int i = 1; row + i < ROW && col + i < COL; ++i)
    {
        // if it's a space, stop searching
        if(board[row + i][col + i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row + i][col + i] == player->playChar)
        {
            flank = TRUE;
            break;
        }

        // updates the board with player's characters
        if(board[row + i][col + i] != ' ' && board[row + i][col + i] != player->playChar)
            board[row + i][col + i] = player->playChar;
    }

    // checks top left of selected move
    for(int i = 1; row - i >= 0 && col - i >= 0; ++i)
    {
        // if it's a space, stop searching
        if(board[row - i][col - i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row - i][col - i] == player->playChar)
        {
            flank = TRUE;
            break;
        }

        // updates the board with player's characters
        if(board[row - i][col - i] != ' ' && board[row - i][col - i] != player->playChar)
            board[row - i][col - i] = player->playChar;
    }

    // checks bottom left of selected move
    for(int i = 1; row + i < ROW && col - i >= 0; ++i)
    {
        // if it's a space, stop searching
        if(board[row + i][col - i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row + i][col - i] == player->playChar)
        {
            flank = TRUE;
            break;
        }

        // updates the board with player's characters
        if(board[row + i][col - i] != ' ' && board[row + i][col - i] != player->playChar)
            board[row + i][col - i] = player->playChar;
    }

    // checks top right of selected move
    for(int i = 1; row - i >= 0 && col + i < COL; ++i)
    {
        // if it's a space, stop searching
        if(board[row - i][col + i] == ' ')
            break;

        // if it's the players own character, mark as flank and stop searching
        if(board[row - i][col + i] == player->playChar)
        {
            flank = TRUE;
            break;
        }

        // updates the board with player's characters
        if(board[row - i][col + i] != ' ' && board[row - i][col + i] != player->playChar)
            board[row - i][col + i] = player->playChar;
    }
}

// updates players disc count
void updateDiscCount(char board[ROW][COL], struct Player *player)
{
    player->discCount = 0;

    // scrolls through the array and increases disc count by 1 for every disc owned by the player
    for(int i = 0; i < ROW; ++i)
        for(int j = 0; j < COL; ++j)
        {
            if(board[i][j] == player->playChar)
                ++player->discCount;
        }
}

// checks if it is the end of the game
int isEndGame(char board[ROW][COL])
{
    // These are used for testing game conditions
    //setNoMoveBoard(board);
    //setFullBoard(board);
    //displayBoard(board);

    // if board is full or there or no more legal moves, marks the game as ended
    if(getDiscCount(board) == SQUARES)
        return TRUE;

    if(validMoveExists(board, 'X') == FALSE && validMoveExists(board, 'O') == FALSE)
        return TRUE;

    return FALSE;
}

// counts amount of disks on board
int getDiscCount(char board[ROW][COL])
{
    int disc = 0;

    // scrolls through board, counts all board spaces that aren't blank
    for(int i = 0; i < ROW; ++i)
        for(int j = 0; j < COL; ++j)
            if(board[i][j] != ' ')
                ++disc;

    // returns disc count
    return disc;
}

// checks if any valid moves exist
int validMoveExists(char board[ROW][COL], char playerChar)
{
    int spaces = 0;
    int result = FALSE;

    // scrolls through the board
    for(int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            // tally up total amount of blank board spaces
            if (board[i][j] == ' ')
                ++spaces;
            // if there are spaces, check if any have valid moves left
            if (spaces > 0) {
                if (checkDiagonal(i, j, board, playerChar) == 0 && checkHorizontal(i, j, board, playerChar) == 0 &&
                    checkVertical(i, j, board, playerChar) == 0)
                    result = FALSE;
                else {
                    result = TRUE;
                    break;
                }
            }
        }

        // stop scrolling through array if valid move is found
        if(result == TRUE)
            break;
    }

    return result;
}

// message displayed when game is over
void gameOver(char board[ROW][COL], struct Player playerX, struct Player playerO)
{
    // end game banner
    printf("\n *****************************************************\n");
    printf(" ********************** GAME OVER ********************\n");
    printf(" *****************************************************\n\n");

    // displays board
    displayBoard(board);

    // updates disc count for both players
    updateDiscCount(board, &playerX);
    updateDiscCount(board, &playerO);

    // displays stats for both players
    displayStats(playerX);
    displayStats(playerO);

    // holds terminal for windows only
    // system("pause");
}

// used for testing by setting a full board
void setFullBoard(char board[ROW][COL])
{
    for(int row = ZERO; row < ROW; row++)
    {
        for(int col = ZERO; col < COL; col++)
        {
            if(row % TWO == ZERO && col % TWO == ZERO)
                board[row][col] = 'X';
            else
                board[row][col] = 'O';
        }
    }
}

// used for testing by setting a board with no valid moves left
void setNoMoveBoard(char board[ROW][COL])
{
    // row 0
    // {'X','X','X','X','X','O','X','O'}
    board[0][0] = 'X';
    board[0][1] = 'X';
    board[0][2] = 'X';
    board[0][3] = 'X';
    board[0][4] = 'X';
    board[0][5] = 'O';
    board[0][6] = 'X';
    board[0][7] = 'O';
    // row 1
    // {'O','X','X','O','O','O','X','O'}
    board[1][0] = 'O';
    board[1][1] = 'X';
    board[1][2] = 'X';
    board[1][3] = 'O';
    board[1][4] = 'O';
    board[1][5] = 'O';
    board[1][6] = 'X';
    board[1][7] = 'O';
    // row 2
    // {'X','X','X','X','X','X','X','O'}
    board[2][0] = 'X';
    board[2][1] = 'X';
    board[2][2] = 'X';
    board[2][3] = 'X';
    board[2][4] = 'X';
    board[2][5] = 'X';
    board[2][6] = 'X';
    board[2][7] = 'O';
    // row 3
    // {'X','X','X','X','X','X','X','O'}
    board[3][0] = 'X';
    board[3][1] = 'X';
    board[3][2] = 'X';
    board[3][3] = 'X';
    board[3][4] = 'X';
    board[3][5] = 'X';
    board[3][6] = 'X';
    board[3][7] = 'O';
    // row 4
    // {'O','X','X','O','X','X','X','O'}
    board[4][0] = 'O';
    board[4][1] = 'X';
    board[4][2] = 'X';
    board[4][3] = 'O';
    board[4][4] = 'X';
    board[4][5] = 'X';
    board[4][6] = 'X';
    board[4][7] = 'O';
    // row 5
    // {'O','O','X','O','O','X','X','O'}
    board[5][0] = 'O';
    board[5][1] = 'O';
    board[5][2] = 'X';
    board[5][3] = 'O';
    board[5][4] = 'O';
    board[5][5] = 'X';
    board[5][6] = 'X';
    board[5][7] = 'O';
    // row 6
    // {'O','O','X','X','O','O','X','O'}
    board[6][0] = 'O';
    board[6][1] = 'O';
    board[6][2] = 'X';
    board[6][3] = 'x';
    board[6][4] = 'O';
    board[6][5] = 'O';
    board[6][6] = 'X';
    board[6][7] = 'O';
    // row 7
    // {'O','O','O','O','O','O','O',' '}
    board[7][0] = 'O';
    board[7][1] = 'O';
    board[7][2] = 'O';
    board[7][3] = 'O';
    board[7][4] = 'O';
    board[7][5] = 'O';
    board[7][6] = 'O';
    board[7][7] = ' ';
}