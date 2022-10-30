
#include <iostream>

#include "../lib/cs225/PNG.h"
#include "../src/FloodFilledImage.h"
#include "../src/Animation.h"

#include "../src/imageTraversal/DFS.h"
#include "../src/imageTraversal/BFS.h"

#include "../src/colorPicker/RainbowColorPicker.h"
#include "../src/colorPicker/GradientColorPicker.h"
#include "../src/colorPicker/GridColorPicker.h"
#include "../src/colorPicker/SolidColorPicker.h"
#include "../src/colorPicker/MyColorPicker.h"

using namespace cs225;
using namespace std;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.
  PNG png;
  png.readFromFile("../kaoru.png");
  
  FloodFilledImage image(png);

  HSLAPixel color(83, 0.57, 0.4);
  HSLAPixel color2(338, 0.56, 0.34);
  HSLAPixel color3(233, 0.51, 0.53);

  SolidColorPicker solid(color);
  SolidColorPicker solid2(color2);
  SolidColorPicker solid3(color3);

  DFS dfs(png, Point(123, 10), 0.35);
  image.addFloodFill(dfs, solid);

  DFS dfs2(png, Point(225, 150), 0.35);
  image.addFloodFill(dfs2, solid);

  DFS dfs3(png, Point(75, 170), 0.35);
  image.addFloodFill(dfs3, solid2);

  DFS dfs4(png, Point(200, 180), 0.35);
  image.addFloodFill(dfs4, solid3);


  Animation animation = image.animate(1000);

  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("../myFloodFill.png");
  animation.write("../myFloodFill.gif");


  return 0;
}
