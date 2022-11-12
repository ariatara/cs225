/* Your code here! */

#include <vector>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

using namespace std;
using namespace cs225;

class SquareMaze {
    public:
        SquareMaze();

        void makeMaze(int width, int height);

        bool canTravel(int x, int y, int dir) const;

        void setWall(int x, int y, int dir, bool exists);

        vector<int> solveMaze();

        PNG* drawMaze() const;

        PNG* drawMazeWithSolution();

    private:

    int width_;

    int height_;

    // Keep track for which cell right wall exists
    vector<vector<bool>> rightWallExists_;

    // Keep track for which cell bottom wall exists
    vector<vector<bool>> downWallExists_;
};