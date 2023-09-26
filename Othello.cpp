
//  Created by Najmuddin Nazary on 2023-03-23.

#include <iostream>
#include "Othello.hpp"
using namespace std;
#include <fstream>
#include <vector>

/* -----------------------------------------board cpp----------------------------------------------------*/
bool Board::loadMapFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "unable to open file: " << filename << endl;
        return false;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            char p;
            if (file >> p) {
                if (p == '*') {
                    positions[row][col] = new UnplayablePosition();
                } else {
                    PlayablePosition* position = new PlayablePosition();
                    position->setPiece(p == 'B' ? Position::BLACK :
                                  p == 'W' ? Position::WHITE :
                                  Position::EMPTY);
                    positions[row][col] = position;
                }
            } else {
                cout << "invalid map format" << endl;
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;
}
const vector<vector<Position*>>& Board::getPositions() const {
    return positions;
}



Board::Board() {
    for (int row = 0; row < 8; ++row) {
        vector<Position*> row_positions;
        for (int col = 0; col < 8; ++col) {
            PlayablePosition* position = new PlayablePosition();
            row_positions.push_back(position);
        }
        positions.push_back(row_positions);
    }
    positions[3][3]->setPiece(Position::WHITE);
    positions[3][4]->setPiece(Position::BLACK);
    positions[4][3]->setPiece(Position::BLACK);
    positions[4][4]->setPiece(Position::WHITE);
}


Board::Board(const string& baordCondition) {
    positions.resize(8, vector<Position*>(8, nullptr));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char p = baordCondition[i * 8 + j];
            if (p == '*') {
                positions[i][j] = new UnplayablePosition();
            } else {
                PlayablePosition *position = new PlayablePosition();
                if (p == 'B') {
                    position->setPiece(Position::BLACK);
                } else if (p == 'W') {
                    position->setPiece(Position::WHITE);
                } else {
                    position->setPiece(Position::EMPTY);
                }
                positions[i][j] = position;
            }
        }
    }
}


void Board::drawBoard() const {
    for (size_t i = 0; i < positions.size(); ++i) {
        const auto& row = positions[i];
        for (size_t j = 0; j < row.size(); ++j) {
            const auto& position = row[j];
            if (position->canPlay()) {
                cout << position->getPiece();
            } else {
                cout << UnplayablePosition::UNPLAYABLE;
            }
        }
        cout << endl;
    }
}


void Board::takeTurn(Game& game, Player& current) {
    int row, col;
    cout << current.getName() << " turn. Please use (0-7) for row and column repectively :" <<endl;
    cin >> row >> col;

    char playerPiece = (current.getName() == game.getFirstPlayer().getName())
    ? Position::BLACK : Position::WHITE;

    if (row < 0 || row >= 8 || col < 0 || col >= 8 ||
        !positions[row][col]->canPlay() || positions[row][col]->getPiece()
        != Position::EMPTY) {

        cout << "Invalid move." << endl;
    } else {
        positions[row][col]->setPiece(playerPiece);
        flipPieces(row, col, playerPiece);
        cout << "Valid move." << endl;
    }
}



bool Board::validMoves(char playerPiece) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (positions[row][col]->canPlay()) {
                return true;
            }
        }
    }
    return false;
}
void Board::flipPieces(int row, int col, char playerpiece) {
    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    char opponentpiece = (playerpiece == Position::BLACK)

    ? Position::WHITE : Position::BLACK;

    for (int i= 0; i< 8; ++i)
    {
        int newrow = row + directions[i][0];
        int newcol = col + directions[i][1];

        bool foundopponentpiece = false;

        while (isInBounds(newrow, newcol) && positions[newrow][newcol]->getPiece()
               == opponentpiece) {
            newrow += directions[i][0];
            newcol += directions[i][1];
            foundopponentpiece = true;
        }

        if (foundopponentpiece && isInBounds(newrow, newcol) &&
            positions[newrow][newcol]->getPiece() == playerpiece) {
            newrow -= directions[i][0];
            newcol -= directions[i][1];

            while (newrow != row || newcol != col) {
                positions[newrow][newcol]->setPiece(playerpiece);
                newrow -= directions[i][0];
                newcol -= directions[i][1];
            }
        }
    }
}



/* -----------------------------------------Game cpp----------------------------------------------------*/

void Game::start() {
    int choice;
    bool game = true;

    while (game) {
        cout << "Othello Game Menu:" << endl;
        cout << "1. Start a New Game" <<endl ;
        cout << "2. Load a Game" <<endl;
        cout << "3. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string player1Name, player2Name;
                cout << "Enter the name of Player 1:";
                cin >> player1Name;
                cout << "Enter the name of Player 2:";
                cin >> player2Name;

                Player player1(player1Name);
                Player player2(player2Name);

                int mapoption;
                cout << "Choose the map type:" <<endl;
                cout << "1. 4-piece game" <<endl;
                cout << "2. 16-piece game" <<endl;
                cout << "Enter your choice" <<endl;
                cin >> mapoption;

                string initialMapFilename;
                if (mapoption == 1) {
                    initialMapFilename = "map1.txt";
                } else if (mapoption == 2) {
                    initialMapFilename = "map2.txt";
                } else {
                    cout << "Invalid choice. Please try again"<<endl;
                    break;
                }

                Board boardMap;
                if (boardMap.loadMapFromFile(initialMapFilename)) {
                    Game game(player1, player2, boardMap);
                    game.play();
                } else {
                    cout << "Failed to load the initial map file." << endl;
                }
                break;
            }
            case 2: {
                string filename;
                cout << "Enter the filename of the saved game:"<< endl;
                cin >> filename;

                Game loadedGame = Game::load(filename);
                loadedGame.play();
                break;
            }

            case 3:
                game = false;
                cout << "Exiting the game!"<< endl;
                break;

            default:
                cout << "Invalid choice. Please try again." <<endl;
        }
    } }


void Game::save() {
    ofstream file("save.txt");
    if (!file) {
        cout << "Unable to save the game." <<endl;
        return;
    }
    file << "First player: " << first.getName() << endl;
    file << "Second player: " << second.getName() <<endl;
    file << "Current player: " << current->getName() <<endl;

    file<< "Board state:" <<endl;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            char piece = board.getPositions()[row][col]->getPiece();
            if (piece == Position::EMPTY) {
                file << ".";
            } else {
                file << piece;
            }
        }
        file << endl;
    }

    cout<< "Game saved successfully." <<endl;
    file.close();
}



bool Game::gameOver() {
    bool player1validmoves = false;
    bool player2validmoves = false;

    for (int i = 0; i < 8 && !(player1validmoves && player2validmoves); i++) {
        for (int j = 0; j < 8 && !(player1validmoves && player2validmoves); j++) {
            if (board.getPositions()[i][j]->getPiece() ==
                Position::EMPTY && board.getPositions()[i][j]->canPlay()) {
                player1validmoves = true;
                player2validmoves = true;
            }
        }
    }

    int totalPiece = board.counter().first + board.counter().second;

    return (!player1validmoves && !player2validmoves) || totalPiece == 64;
}


    Game Game::load(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string playerName;
            getline(file, playerName);
            Player first(playerName);
            getline(file, playerName);
            Player second(playerName);
            getline(file, playerName);

            string boardcondition;
            getline(file, boardcondition);
            Board loadedBoard(boardcondition);
            file.close();

            Game loadedGame(first, second);
            loadedGame.current = (playerName == loadedGame.first.getName())
            ? &loadedGame.first : &loadedGame.second;

            return loadedGame;
        } else {
            cout << "Unable to open file." << endl;
            throw runtime_error("Failed to open the file.");
        }
    }

void Game::play() {
    string input;
    bool forfeit = false;

    while (!gameOver() && !forfeit) {
        board.drawBoard();
        cout << "Enter 's' to save the game, 'm' to make a move or 'f' to forfeit (case sensitive :" <<endl;
        cin >> input;

        if (input == "s") {
            save();
        } else if (input == "m") {
            board.takeTurn(*this, *current);
            switchTurns();
        } else if (input == "f") {
            forfeit = true;
            cout << current->getName() << " has forfeited the game." << endl;
            switchTurns();
        } else {
            cout << "Invalid input. Please enter 's', 'm' or 'f'." << endl;
        }
    }
    cout << "Game over." << endl;
    auto pieces = board.counter();

    if (forfeit) {
        cout << current->getName() << " wins because the opponent forfeited the match." << endl;
    } else if (pieces.first > pieces.second) {
        cout << first.getName() << " wins with " << pieces.first << " pieces." << endl;
    } else if (pieces.first < pieces.second) {
        cout << second.getName() << " wins with " << pieces.second << " pieces." << endl;
    } else {
        cout << "It's a draw with " << pieces.first << " pieces each." << endl;
    }
}
