#include "/workspaces/cs225/mp_stickers/src/StickerSheet.h"

using namespace std;

StickerSheet::StickerSheet(const Image& picture, unsigned max) {
    maxStickers = max;

    base = new Image[maxStickers];
    for (int i = 0; i < maxStickers; i++) {
        images[i] = nullptr;
    }

    xCoords = new unsigned[maxStickers];
    yCoords = new unsigned[maxStickers];
    numStickers = 0;
}

StickerSheet::~StickerSheet() {
    clear();
}

StickerSheet::StickerSheet(const StickerSheet &other) {
    copy(other);
}

const StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
    if (this == &other) {
        return *this;
    }

    clear();
    copy(other);

    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
    if (max == maxStickers) {
        return;
    }

    Image** temp = new Image*[max];

    for (int i = 0; i < maxStickers; i++) {
        temp[i] = images[i];
    }

    delete[] images;
    images = new Image*[max];

    if (max < maxStickers) {
        for(int i = 0; i < maxStickers; i++) {
            images[i] = temp[i];
        }
    } else {
        for (int i = 0; i < max; i++) {
            images[i] = temp[i];
        }
    }

    maxStickers = max;

    delete[] temp;
    temp = nullptr;
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
    if (numStickers < maxStickers) {
        for (int i = 0; i < maxStickers; i++) {
            if (images[i] == nullptr) {
                images[i] = new Image(sticker);
                xCoords[i] = x;
                yCoords[i] = y;
                numStickers++;
                return i;
            }
        }
    }

    return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if (index < maxStickers) {
        xCoords[index] = x;
        yCoords[index] = y;
        return true;
    }

    return false;
}

void StickerSheet::removeSticker(unsigned index) {
    if (index < maxStickers && index >= 0) {
        delete images[index];
        images[index] = nullptr;
        
        xCoords[index] = 0;
        yCoords[index] = 0;

        numStickers--;
    }
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index < maxStickers) {
        return images[index];
    }

    return nullptr;
}

Image StickerSheet::render() const {
    unsigned xMax = base->width();
    unsigned yMax = base->height();

    for (int i = 0; i < numStickers; i++) {
        if (images[i] != nullptr) {
            unsigned x = xCoords[i] + images[i]->width();
            unsigned y = yCoords[i] + images[i]->height();

            if (x > xMax) {
                xMax = x;
            }

            if (y > yMax) {
                yMax = y;
            }
        }
    }

    base->resize(xMax, yMax);

    for (int i = 0; i < numStickers; i++) {
        if (images[i] != nullptr) {
            for (int x = xCoords[i]; x < (xCoords[i] + images[i]->width()); x++) {
                for (int y = yCoords[i]; x < (yCoords[i] + images[i]->height()); y++) {
                    HSLAPixel& basePixel = base->getPixel(x, y);
                    HSLAPixel& finalPixel = images[i]->getPixel(x - xCoords[i], y - yCoords[i]);
                    
                    if (finalPixel.a != 0) {
                        basePixel.h = finalPixel.h;
                        basePixel.s = finalPixel.s;
                        basePixel.l = finalPixel.l;
                        basePixel.a = finalPixel.a;
                    }
                }
            }
        }
    }

    return *base;
}

void StickerSheet::clear() {
    for (int i = 0; i < maxStickers; i++) {
        delete images[i];
        images[i] = nullptr;
    }

    delete[] images;
    delete[] xCoords;
    delete[] yCoords;
    delete[] base;

    images = nullptr;
    xCoords = nullptr;
    yCoords = nullptr;
    base = nullptr;
    numStickers = 0;
}

void StickerSheet::copy(const StickerSheet& other) {
    maxStickers = other.maxStickers;

    base = other.base;

    images = new Image*[maxStickers];

    xCoords = new unsigned[maxStickers];
    yCoords = new unsigned[maxStickers];
    numStickers = 0;

    for (int i = 0; i < maxStickers; i++) {
        images[i] = other.images[i];
        xCoords[i] = other.xCoords[i];
        yCoords[i] = other.yCoords[i];
    }
}