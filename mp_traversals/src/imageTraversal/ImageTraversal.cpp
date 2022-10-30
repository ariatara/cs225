#include <cmath>
#include <iterator>
#include <iostream>

#include "../../lib/cs225/HSLAPixel.h"
#include "../../lib/cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

using namespace std;

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() : 
imageTraversal_(), sourceImage_(), startPoint_(), currentPoint_(), toleranceLimit_(0.0), endReached_(true) {
  /** @todo [Part 1] */
}

ImageTraversal::Iterator::Iterator(ImageTraversal& imageTraversal, PNG& sourceImage, Point startPoint, double toleranceLimit) : 
imageTraversal_(&imageTraversal), sourceImage_(sourceImage), startPoint_(startPoint), currentPoint_(startPoint), toleranceLimit_(toleranceLimit), 
endReached_(false) {
  for (unsigned i = 0; i < (sourceImage_.width() * sourceImage_.height()); i++) {
    alreadyVisited_.push_back(false);
  }
  
  if (!isAllowed(startPoint_)) {
    endReached_ = true;
    currentPoint_ = Point();
  }
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator& ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  if (endReached_) {
    return *this;
  }

  currentPoint_ = imageTraversal_->pop();
  alreadyVisited_[(currentPoint_.y * sourceImage_.width()) + currentPoint_.x] = true;

  Point rightNeighbor(currentPoint_.x + 1, currentPoint_.y);
  Point bottomNeighbor(currentPoint_.x, currentPoint_.y + 1);
  Point leftNeighbor(currentPoint_.x - 1, currentPoint_.y);
  Point topNeighbor(currentPoint_.x, currentPoint_.y - 1);

  if (isAllowed(rightNeighbor)) {
    if (!alreadyVisited_[(rightNeighbor.y * sourceImage_.width()) + rightNeighbor.x]) {

      imageTraversal_->add(rightNeighbor);
    }
  }
  
  if (isAllowed(bottomNeighbor)) {
    if (!alreadyVisited_[(bottomNeighbor.y * sourceImage_.width()) + bottomNeighbor.x]) {
      imageTraversal_->add(bottomNeighbor);
    }
  }

  if (isAllowed(leftNeighbor)) {
    if (!alreadyVisited_[(leftNeighbor.y * sourceImage_.width()) + leftNeighbor.x]) {
      imageTraversal_->add(leftNeighbor);
    }
  }

  if (isAllowed(topNeighbor)) {
    if (!alreadyVisited_[(topNeighbor.y * sourceImage_.width()) + topNeighbor.x]) {
      imageTraversal_->add(topNeighbor);
    }
  }

  while (!imageTraversal_->empty() && alreadyVisited_[(imageTraversal_->peek().y * sourceImage_.width()) + imageTraversal_->peek().x]) {
    imageTraversal_->pop();
  }

  if (imageTraversal_->empty()) {
    endReached_ = true;
    currentPoint_ = Point();
    return *this;
  }

  currentPoint_ = imageTraversal_->peek();

  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return currentPoint_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return !(endReached_ && other.endReached_);
}

bool ImageTraversal::Iterator::isAllowed(Point point) {
  if (point.x < 0 || point.x >= sourceImage_.width() || point.y < 0 || point.y >= sourceImage_.height()) {
    return false;
  } 

  double tolerance = calculateDelta(sourceImage_.getPixel(startPoint_.x, startPoint_.y), sourceImage_.getPixel(point.x, point.y));

  if (tolerance >= toleranceLimit_) {
    return false;
  }

  return true;
}

