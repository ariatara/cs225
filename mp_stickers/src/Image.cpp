#include <cmath>
#include "Image.h"

using namespace cs225;

void Image::lighten() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l += 0.1;
            if (pixel.l > 1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::lighten(double amount) {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l += amount;
            if (pixel.l > 1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::darken() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l -= 0.1;
            if (pixel.l < 0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::darken(double amount) {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.l -= amount;
            if (pixel.l < 0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::saturate() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s += 0.1;
            if (pixel.s > 1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::saturate(double amount) {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s += amount;
            if (pixel.s > 1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::desaturate() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= 0.1;
            if (pixel.s < 0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::desaturate(double amount) {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= amount;
            if (pixel.s < 0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::grayscale() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.s -= 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
            HSLAPixel& pixel = this->getPixel(w, h);
            pixel.h += degrees;
            if (pixel.h > 360) {
                pixel.h -= 360;
            } else if (pixel.h < 0) {
                pixel.h += 360;
            }
        }
    }
}

void Image::illinify() {
    for (unsigned int w = 0; w < this->width(); w++) {
        for (unsigned int h = 0; h < this->height(); h++) {
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

    for (unsigned int w = 0; w < newWidth; w++) {
        for (unsigned int h = 0; h < newHeight; h++) {
            img->PNG::getPixel(w, h).h = this->PNG::getPixel(floor(w / factor), floor(h / factor)).h;
            img->PNG::getPixel(w, h).s = this->PNG::getPixel(floor(w / factor), floor(h / factor)).s;
            img->PNG::getPixel(w, h).l = this->PNG::getPixel(floor(w / factor), floor(h / factor)).l;
            img->PNG::getPixel(w, h).a = this->PNG::getPixel(floor(w / factor), floor(h / factor)).a;
        }
    }

    this->resize(newWidth, newHeight);

    for (unsigned int w = 0; w < newWidth; w++) {
        for (unsigned int h = 0; h < newHeight; h++) {
            this->PNG::getPixel(w, h).h = img->PNG::getPixel(w, h).h;
            this->PNG::getPixel(w, h).s = img->PNG::getPixel(w, h).s;
            this->PNG::getPixel(w, h).l = img->PNG::getPixel(w, h).l;
            this->PNG::getPixel(w, h).a = img->PNG::getPixel(w, h).a;
        }
    }

    delete img;
    img = nullptr;

}

void Image::scale(unsigned width, unsigned height) {
    double widthFactor = ((double) width) / ((double) this->width());
    double heightFactor = ((double) height) / ((double) this->height());

    PNG* img = new PNG(width, height);

    if (widthFactor <= heightFactor) {
        scale(widthFactor);
    } else {
        scale(heightFactor);
    }

    delete img;
    img = nullptr;
}