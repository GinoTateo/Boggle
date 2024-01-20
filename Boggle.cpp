
#include "Dictionary.h"
#include "Boggle.h"


// This function is done.
Dictionary Boggle::GetDictionary() {
    return dict;
}

// This function is done.
Dictionary Boggle::WordsFound() {
    return wordsFound;
}

Boggle::Boggle() {                                                  // Constructor

    dict.LoadDictionaryFile("dictionary.txt");              // Loads "dictionary.txt" into dict

    for (int i = 0; i < BOARD_SIZE; ++i) {                           // Sets visited and board to default
        visited[i][i] = 0;
        board[i][i] = "";
    }



}

Boggle::Boggle(string filename) {

    dict.LoadDictionaryFile(filename);                  // Loads filename into dict

    for (int i = 0; i < BOARD_SIZE ; ++i) {             // Sets visited and board to default
        visited[i][i] = 0;
        board[i][i] = "";
    }
}

void Boggle::SetBoard(string (*board)[4]) {             // Sets this->board to the string board
    for (int i = 0; i < BOARD_SIZE ; ++i){
        for (int x = 0; x < BOARD_SIZE ; ++x) {
            this->board[i][x] = board[i][x];
        }
    }
}

void Boggle::SolveBoard(bool printBoard, ostream &output) {
    wordsFound.MakeEmpty();


    string currPrefix;
    int numofsteps = 0;



    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    for (int i = 0; i < 4 ; ++i){
        for (int x = 0; x < 4 ; ++x) {

            SolveBoardHelper(i, x, numofsteps, currPrefix, printBoard);
//            up = i + 1;
//            down = i - 1;
//            left = x - 1;
//            right = x + 1;
//
//            if (up <= BOARD_SIZE){
//                SolveBoardHelper(up, x);
//            }
//            if (down >= BOARD_SIZE) {
//                SolveBoardHelper(down, x);
//            }
//            if (left >= BOARD_SIZE) {
//                SolveBoardHelper(i, left);
//            }
//            if (right <= BOARD_SIZE) {
//                SolveBoardHelper(i, right);
//            }
        }
    }
}

void Boggle::SaveSolve(string filename) {

   // SolveBoardHelper();

}

void Boggle::PrintBoard(ostream &output) {

    cout << "  ";
    for (int i = 0; i < 4 ; ++i){
        for (int x = 0; x < 4 ; ++x) {
            cout << board[i][x] << "  ";
            cout << "\t";
            cout << board[i][x] << "  ";
        }
        cout << endl;
        cout << "  ";
    }

}

void Boggle::SolveBoardHelper(int i, int x, int numofsteps, string currPrefix, bool printBoard) {           // Recursive function to solve

// Does not currently work

    // Make sure we inbounds
    if(i >= BOARD_SIZE || x >= BOARD_SIZE)
        return;

    if(i < 0 || x < 0)
        return;

    if(visited[i][x] != 0)
        return;

    if(!dict.IsPrefix(currPrefix))
        return;

    // If Isword
    // or use
    // IsPrefix
    if(dict.IsWord(currPrefix)) {
        if(!wordsFound.IsWord(currPrefix)) {
            if(!printBoard) {
                wordsFound.AddWord(currPrefix);
            }
            else {
                wordsFound.AddWord(currPrefix);
                printBoard = false;
            }
        }
    }

    currPrefix = currPrefix + board[i][x];
    visited[i][x] = ++numofsteps;


    // Calls to every position one off
    SolveBoardHelper(i , x - 1, numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i , x + 1,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i - 1 , x,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i + 1 , x,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i + 1 , x + 1,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i + 1 , x - 1,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i - 1 , x + 1,  numofsteps, currPrefix, printBoard);
    SolveBoardHelper(i - 1 , x - 1,  numofsteps, currPrefix, printBoard);

    visited[i][x] = 0;

}



