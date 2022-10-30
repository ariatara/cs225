/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

    if (theTiles.empty()) {
        return nullptr;
    }

    vector<Point<3>> averagePoints;

    map<Point<3>, TileImage*> averagePointsTileMap;

    for (vector<TileImage>::iterator tileIterator = theTiles.begin(); tileIterator != theTiles.end(); tileIterator++) {
        LUVAPixel pixel = tileIterator->getAverageColor();

        Point<3> point = convertToXYZ(pixel);

        averagePoints.push_back(point);

        averagePointsTileMap[point] = &(*tileIterator);
    }

    KDTree<3> averageColorKDTree = KDTree(averagePoints);

    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    for (int y = 0; y < theSource.getRows(); y++) {
        for (int x = 0; x < theSource.getColumns(); x++) {

            Point<3> query = convertToXYZ(theSource.getRegionColor(y, x));
            Point<3> nearest = averageColorKDTree.findNearestNeighbor(query);

            TileImage* nearestTile = averagePointsTileMap[nearest];

            canvas->setTile(y, x, nearestTile);
        }
    }

    return canvas;
}

