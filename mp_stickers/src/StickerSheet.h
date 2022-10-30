/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once

#include "Image.h"
#include <vector>

class StickerSheet {
    public:
        StickerSheet(const Image& picture, unsigned max);
        ~StickerSheet();
        StickerSheet(const StickerSheet &other);
        const StickerSheet& operator=(const StickerSheet &other);
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, unsigned x, unsigned y);
        bool translate(unsigned index, unsigned x, unsigned y);
        void removeSticker(unsigned index);
        Image* getSticker(unsigned index);
        Image render() const;
    
    private:
        unsigned maxStickers;
        Image* base;
        Image** images;
        unsigned* xCoords;
        unsigned* yCoords;
        unsigned numStickers;
        void clear();
        void copy(const StickerSheet& other);

};