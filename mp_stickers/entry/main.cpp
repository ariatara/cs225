#include "../src/Image.h"
#include "../src/StickerSheet.h"

using namespace std;

int main() {
  //800 x 600 px
  Image rainbow;  rainbow.readFromFile("../mystickers/rainbow.png");

  //472 x 630
  Image kao1;  kao1.readFromFile("../mystickers/kao1.png");
  kao1.scale(0.5);

  //459 x 640
  Image kao2;  kao2.readFromFile("../mystickers/kao2.png");
  kao2.scale(0.5);

  //400 x 593
  Image kao3;  kao3.readFromFile("../mystickers/kao3.png");
  kao3.scale(0.5);

  //640 x 640
  Image kaoheart;  kaoheart.readFromFile("../mystickers/kaoheart.png");
  kaoheart.scale(0.3);

  //444 x 193
  Image kaosig;  kaosig.readFromFile("../mystickers/kaosignature.png");

  //440 x 186
  Image undead;  undead.readFromFile("../mystickers/undead.png");
  undead.scale(0.5);

  //580 x 220
  Image bbg;  bbg.readFromFile("../mystickers/bbg.png");
  bbg.scale(0.5);

  StickerSheet sheet(rainbow, 20);

  sheet.addSticker(kao1, 0, 0);
  sheet.addSticker(kao1, 20, 20);
  sheet.addSticker(kao1, 40, 40);


  sheet.addSticker(kao2, 240, 200);
  sheet.addSticker(kao2, 260, 220);
  sheet.addSticker(kao2, 280, 240);

  sheet.addSticker(kao3, 500, 50);
  sheet.addSticker(kao3, 520, 30);
  sheet.addSticker(kao3, 540, 10);
  
  sheet.addSticker(bbg, 240, 20);
  sheet.addSticker(bbg, 240, 40);
  sheet.addSticker(bbg, 240, 60);

  sheet.addSticker(kaoheart, 550, 350);
  sheet.addSticker(kaosig, 0, 400);
  sheet.addSticker(undead, 550, 500);






  Image result = sheet.render();

  result.desaturate(0.25);

  result.writeToFile("../myImage.png");

  return 0;
}
