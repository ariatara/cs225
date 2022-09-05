#include <cmath>
#include "mp_stickers/src/Image.h"

using namespace cs225;

void Image::lighten() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l += 0.1;
            if (pixel.l > 1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::lighten(double amount) {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l += amount;
            if (pixel.l > 1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::darken() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l -= 0.1;
            if (pixel.l < 0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::darken(double amount) {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l -= amount;
            if (pixel.l < 0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::saturate() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s += 0.1;
            if (pixel.s > 1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::saturate(double amount) {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s += amount;
            if (pixel.s > 1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::desaturate() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= 0.1;
            if (pixel.s < 0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::desaturate(double amount) {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= amount;
            if (pixel.s < 0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::grayscale() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.h = fmod((pixel.h + degrees), 360);
        }
    }
}

void Image::illinify() {
    for (int w = 0; w < this->width(); w++) {
        for (int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            if (pixel.h > 113 && pixel.h < 294) {
                pixel.h = 216;
            } else {
                pixel.h = 11;
            }
        }
    }
}

void Image::scale(double factor) {
    if (factor == 1.0) {
        return;
    }

    unsigned int newWidth = this->width() * factor;
    unsigned int newHeight = this->height() * factor;

    PNG* img = new PNG(newWidth, newHeight);

    for (int w = 0; w < newWidth; w++) {
        for (int h = 0; h < newHeight; h++) {
            img->PNG::getPixel(w, h).h = this->PNG::getPixel(w / factor, h / factor).h;
            img->PNG::getPixel(w, h).s = this->PNG::getPixel(w / factor, h / factor).s;
            img->PNG::getPixel(w, h).l = this->PNG::getPixel(w / factor, h / factor).l;
            img->PNG::getPixel(w, h).a = this->PNG::getPixel(w / factor, h / factor).a;
        }
    }

    this->resize(newWidth, newHeight);

    for (int w = 0; w < newWidth; w++) {
        for (int h = 0; h < newHeight; h++) {
            this->PNG::getPixel(w, h).h = img->PNG::getPixel(w, h).h;
            this->PNG::getPixel(w, h).s = img->PNG::getPixel(w, h).s;
            this->PNG::getPixel(w, h).l = img->PNG::getPixel(w, h).l;
            this->PNG::getPixel(w, h).a = img->PNG::getPixel(w, h).a;
        }
    }

}

void Image::scale(unsigned width, unsigned height) {
    unsigned int widthFactor = width / this->width();
    unsigned int heightFactor = height / this->height();
    unsigned int factor;

    if (widthFactor > heightFactor) {
        factor = heightFactor;
    } else {
        factor = widthFactor;
    }

    PNG* img = new PNG(width, height);

    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            img->PNG::getPixel(w, h).h = this->PNG::getPixel(w / factor, h / factor).h;
            img->PNG::getPixel(w, h).s = this->PNG::getPixel(w / factor, h / factor).s;
            img->PNG::getPixel(w, h).l = this->PNG::getPixel(w / factor, h / factor).l;
            img->PNG::getPixel(w, h).a = this->PNG::getPixel(w / factor, h / factor).a;
        }
    }

    this->resize(width, height);

    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            this->PNG::getPixel(w, h).h = img->PNG::getPixel(w, h).h;
            this->PNG::getPixel(w, h).s = img->PNG::getPixel(w, h).s;
            this->PNG::getPixel(w, h).l = img->PNG::getPixel(w, h).l;
            this->PNG::getPixel(w, h).a = img->PNG::getPixel(w, h).a;
        }
    }
}