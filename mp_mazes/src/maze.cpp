/* Your code here! */
#include "maze.h"
#include "dsets.h"
#include <map>
#include <queue>
#include <algorithm>

using namespace std;
using namespace cs225;

SquareMaze::SquareMaze() {

}

void SquareMaze::makeMaze(int width, int height) {
    // Set maze width and height
    width_ = width;
    height_ = height;

    // Initialize vectors and set everything to true
    rightWallExists_.resize(width_, vector<bool>(height_));
    downWallExists_.resize(width_, vector<bool>(height_));
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            rightWallExists_[x][y] = true;
            downWallExists_[x][y] = true;
        }
    }

    DisjointSets dsets;
    dsets.addelements(width_ * height_);

    int cellCount = 0;
    while (cellCount < (width * height) - 1) {
        // Choose random x and y coordinates for breaking down walls
        int randomX = rand() % width_;
        int randomY = rand() % height_;

        // Determines whether we will break a bottom wall or a right wall
        int downWall = rand() % 2;
        if (downWall) {
            if (randomY < height_ - 1 && downWallExists_[randomX][randomY]) {
                int currentCell = randomX + (randomY * width_);
                int downCell = randomX + ((randomY + 1) * width_);
                
                int currentRoot = dsets.find(currentCell);
                int downRoot = dsets.find(downCell);
                
                // Merge the two cells because they will now be connected
                if (currentRoot != downRoot) {
                    dsets.setunion(currentCell, downCell);
                    setWall(randomX, randomY, downWall, false);
                    cellCount++;
                }
            }
        } else {
            // Same algorithm as before but for a right wall break
            if (randomX < width_ - 1 && rightWallExists_[randomX][randomY]) {
                int currentCell = randomX + (randomY * width_);
                int rightCell = (randomX + 1) + (randomY * width_);
                
                int currentRoot = dsets.find(currentCell);
                int rightRoot = dsets.find(rightCell);

                if (currentRoot != rightRoot) {
                    dsets.setunion(currentCell, rightCell);
                    setWall(randomX, randomY, downWall, false);
                    cellCount++;
                }
            }
        }
    }

}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    bool canTravel = false;

    // Check if within bounds
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return canTravel;
    }

    // For given direction, check if wall exists. If it doesn't, we can travel to the next cell.
    switch (dir) {
    case 0:
        canTravel = !rightWallExists_[x][y];
        break;
    case 1:
        canTravel = !downWallExists_[x][y];
        break;
    case 2:
        if (x != 0) {
            canTravel = !rightWallExists_[x - 1][y];
        }
        break;
    case 3:
        if (y != 0) {
            canTravel = !downWallExists_[x][y - 1];
        }
        break;
    default:
        break;
    }

    return canTravel;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {

    // Check bounds
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return;
    }

    // If direction is right, assign exists
    if (dir == 0) {
        rightWallExists_[x][y] = exists;
    }

    // If direction is downwards, assign exists
    if (dir == 1) {
        downWallExists_[x][y] = exists;
    }
}

vector<int> SquareMaze::solveMaze() {
    // Keep track of from which cell a cell is reached
    map<pair<int, int>, pair<int, int>> solutionSteps;

    // Keep track of from which direction a cell is reached
    map<pair<int, int>, int> solutionDirections;

    // BFS queue
    queue<pair<int, int>> searchQueue;

    // Store the last row's x coordinates
    vector<int> lastRowX;

    vector<vector<bool>> alreadyVisited;
    alreadyVisited.resize(width_, vector<bool>(height_));
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            alreadyVisited[x][y] = false;
        }
    }

    searchQueue.push(pair<int, int>(0, 0));

    while (!searchQueue.empty()) {
        pair<int, int> currentCell = searchQueue.front();
        searchQueue.pop();

        int x = currentCell.first;
        int y = currentCell.second;

        if (y == height_ - 1) {
            lastRowX.push_back(x);
        }

        // Record the step, direction, and if already visited
        if (canTravel(x, y, 0) && !alreadyVisited[x + 1][y]) {
            searchQueue.push(pair<int, int>(x + 1, y));
            solutionDirections[pair<int, int>(x + 1, y)] = 0;
            solutionSteps[pair<int, int>(x + 1, y)] = pair<int, int>(x, y);
            alreadyVisited[x + 1][y] = true;
        }

        if (canTravel(x, y, 1) && !alreadyVisited[x][y + 1]) {
            searchQueue.push(pair<int, int>(x, y + 1));
            solutionDirections[pair<int, int>(x, y + 1)] = 1;
            solutionSteps[pair<int, int>(x, y + 1)] = pair<int, int>(x, y);
            alreadyVisited[x][y + 1] = true;
        }

        if (canTravel(x, y, 2) && !alreadyVisited[x - 1][y]) {
            searchQueue.push(pair<int, int>(x - 1, y));
            solutionDirections[pair<int, int>(x - 1, y)] = 2;
            solutionSteps[pair<int, int>(x - 1, y)] = pair<int, int>(x, y);
            alreadyVisited[x - 1][y] = true;
        }

        if (canTravel(x, y, 3) && !alreadyVisited[x][y - 1]) {
            searchQueue.push(pair<int, int>(x, y - 1));
            solutionDirections[pair<int, int>(x, y - 1)] = 3;
            solutionSteps[pair<int, int>(x, y - 1)] = pair<int, int>(x, y);
            alreadyVisited[x][y - 1] = true;
        }
    }

    vector<int> solvedDirection;
    int smallestIndexX = width_ - 1;

    // Find the smallest x coordinate of the longest path
    while (lastRowX[smallestIndexX] == lastRowX[smallestIndexX - 1]) {
        smallestIndexX--;
    }

    pair<int, int> currentCell(lastRowX[smallestIndexX], height_ - 1);
    pair<int, int> startCell(0, 0);

    // From the last cell, trace the directions back to the start cell
    while (currentCell != startCell) {
        solvedDirection.push_back(solutionDirections[currentCell]);
        currentCell = solutionSteps[currentCell];
    }
    
    // Since the directions were stored from last cell to start cell, reverse
    reverse(solvedDirection.begin(), solvedDirection.end());


    return solvedDirection;
}

PNG* SquareMaze::drawMaze() const {

    // Factor to enlarge maze
    int cellDimension = 10;

    PNG* unsolvedMaze = new PNG((width_ * cellDimension + 1), (height_ * cellDimension + 1));

    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            
            // If x = 0, color that line black
            if (x == 0) {
                for (int c = 0; c < height_ * cellDimension + 1; c++) {
                    HSLAPixel& currentPixel = unsolvedMaze->getPixel(0, c);
                    currentPixel.l = 0.0;
                    currentPixel.a = 1.0;
                }
            }

            // If y = 0, color that line black
            if (y == 0) {
                for (int c = cellDimension; c < width_ * cellDimension + 1; c++) {
                    HSLAPixel& currentPixel = unsolvedMaze->getPixel(c, 0);
                    currentPixel.l = 0.0;
                    currentPixel.a = 1.0;
                }
            }

            // If there is a right wall, color it black
            if (rightWallExists_[x][y]) {
                for (int c = 0; c < cellDimension + 1; c++) {
                    HSLAPixel& currentPixel = unsolvedMaze->getPixel((x + 1) * cellDimension, y * cellDimension + c);
                    currentPixel.l = 0.0;
                    currentPixel.a = 1.0;
                }
            }

            // If there is a downward wall, color it black
            if (downWallExists_[x][y]) {
                for (int c = 0; c < cellDimension + 1; c++) {
                    HSLAPixel& currentPixel = unsolvedMaze->getPixel(x * cellDimension + c, (y + 1) * cellDimension);
                    currentPixel.l = 0.0;
                    currentPixel.a = 1.0;
                }
            }
        }
    }

    return unsolvedMaze;
}

PNG* SquareMaze::drawMazeWithSolution() {
    
    // Retrieve the solved path
    vector<int> solvedDirections = solveMaze();
    PNG* solvedMaze = drawMaze();
    
    int cellDimension = 10;

    // Starting coordinates
    int x = 5;
    int y = 5;

    for (unsigned i = 0; i < solvedDirections.size(); i++) {

        // Color path red according to direction
        switch (solvedDirections[i])
        {
        case 0:
            for (int j = 0; j < cellDimension; j++) {
                HSLAPixel& currentPixel = solvedMaze->getPixel(x, y);
                currentPixel.h = 0.0;
                currentPixel.s = 1.0;
                currentPixel.l = 0.5;
                currentPixel.a = 1.0;

                x++;
            }
            break;
        case 1:
            for (int j = 0; j < cellDimension; j++) {
                HSLAPixel& currentPixel = solvedMaze->getPixel(x, y);
                currentPixel.h = 0.0;
                currentPixel.s = 1.0;
                currentPixel.l = 0.5;
                currentPixel.a = 1.0;

                y++;
            }
            break;
        case 2:
            for (int j = 0; j < cellDimension; j++) {
                HSLAPixel& currentPixel = solvedMaze->getPixel(x, y);
                currentPixel.h = 0.0;
                currentPixel.s = 1.0;
                currentPixel.l = 0.5;
                currentPixel.a = 1.0;

                x--;
            }
            break;
        case 3:
            for (int j = 0; j < cellDimension; j++) {
                HSLAPixel& currentPixel = solvedMaze->getPixel(x, y);
                currentPixel.h = 0.0;
                currentPixel.s = 1.0;
                currentPixel.l = 0.5;
                currentPixel.a = 1.0;

                y--;
            }
            break;
        
        default:
            break;
        }
    }

    // Color end of path red
    HSLAPixel& currentPixel = solvedMaze->getPixel(x, y);
    currentPixel.h = 0.0;
    currentPixel.s = 1.0;
    currentPixel.l = 0.5;
    currentPixel.a = 1.0;

    // Getting to the lower edge to create the exit
    x -= 4;
    y += 5;

    // Create exit
    for (int i = 0; i < 9; i++) {
        HSLAPixel& exitPixel = solvedMaze->getPixel(x, y);
        exitPixel.l = 1.0;
        exitPixel.a = 1.0;
        x++;
    }

    return solvedMaze;
}