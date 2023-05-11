//
//  main.cpp
//  OthelloAssignment
//
//  Created by Najmuddin Nazary on 2023-03-23.
//

#include <iostream>
#include "Othello.hpp"

using namespace std;
#include <fstream>
#include <vector>





int main() {
    Game game(Player("player1"), Player("p3layer2"));
    game.start();
    return 0;
}
