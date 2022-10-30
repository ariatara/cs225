/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */

    if (curDim < 0 || curDim >= Dim) {
      return false;
    }

    if (first[curDim] == second[curDim]) {
      return first < second;
    }

    return first[curDim] < second[curDim];
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */

    double currentBestDistance = 0.0;
    double potentialBestDistance = 0.0;

    for (int i = 0; i < Dim; i++) {
      currentBestDistance += (target[i] - currentBest[i])*(target[i] - currentBest[i]);
      potentialBestDistance += (target[i] - potential[i])*(target[i] - potential[i]);
    }

    if (potentialBestDistance == currentBestDistance) {
      return potential < currentBest;
    }

    return potentialBestDistance < currentBestDistance;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */

    input_points_ = newPoints;

    if (input_points_.size() == 0) {
      return;
    } else if (input_points_.size() == 1) {
      root = new KDTreeNode(input_points_[0]);
    } else {
      int startIndex = 0;
      int endIndex = input_points_.size() - 1;
      int dimension = 0;

      int medianIndex = (startIndex + endIndex) / 2;

      select(startIndex, endIndex, medianIndex, dimension);

      root = new KDTreeNode(input_points_[medianIndex]);

      root->left = buildTree(startIndex, medianIndex - 1, (dimension + 1) % Dim);
      root->right = buildTree(medianIndex + 1, endIndex, (dimension + 1) % Dim);
    }

    size = input_points_.size();
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  root = copy(root, other.root);
  size = other.size;

}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode* subTreeRoot, KDTreeNode* otherSubTreeRoot) {
  if (otherSubTreeRoot != nullptr) {
    subTreeRoot = new KDTreeNode(otherSubTreeRoot->point);
  } else {
    subTreeRoot = nullptr;
    return subTreeRoot;
  }

  if (otherSubTreeRoot->left != nullptr) {
    subTreeRoot->left = copy(subTreeRoot->left, otherSubTreeRoot->left);
  }

  if (otherSubTreeRoot->right != nullptr) {
    subTreeRoot->right = copy(subTreeRoot->right, otherSubTreeRoot->right);
  }

  return subTreeRoot;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  clear(root);

  root = copy(root, rhs.root);
  size = rhs.size;

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */

  clear(root);
}

template <int Dim>
void  KDTree<Dim>::clear(KDTreeNode* subTreeRoot) {
  if (subTreeRoot == nullptr) {
    return;
  }

  if (subTreeRoot->left != nullptr) {
    clear(subTreeRoot->left);
  }

  if (subTreeRoot->right != nullptr) {
    clear(subTreeRoot->right);
  }

  delete subTreeRoot;
  subTreeRoot = nullptr;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(KDTreeNode* subTreeRoot, const Point<Dim>& query, int dimension) const {
  if (subTreeRoot->right == nullptr && subTreeRoot->left == nullptr) {
    return subTreeRoot->point;
  }

  Point<Dim> currentNearest;
  bool leftChecked = false;

  if (smallerDimVal(query, subTreeRoot->point, dimension)) {
    if (subTreeRoot->left != nullptr) {
      currentNearest = findNearestNeighbor(subTreeRoot->left, query, (dimension + 1) % Dim);
      leftChecked = true;
    } else {
      currentNearest = findNearestNeighbor(subTreeRoot->right, query, (dimension + 1) % Dim);
      leftChecked = false;
    }
  } else {
    if (subTreeRoot->right != nullptr) {
      currentNearest = findNearestNeighbor(subTreeRoot->right, query, (dimension + 1) % Dim);
      leftChecked = false;
    } else {
      currentNearest = findNearestNeighbor(subTreeRoot->left, query, (dimension + 1) % Dim);
      leftChecked = true;
    }
  }

  if (shouldReplace(query, currentNearest, subTreeRoot->point)) {
    currentNearest = subTreeRoot->point;
  }

  // check if need to search other side
  // check if split distance < radial distance (compute both)

  double radialDistance = 0.0;

  for (int i = 0; i < Dim; i++) {
    radialDistance += (query[i] - currentNearest[i])*(query[i] - currentNearest[i]);
  }

  double splitDistance = (query[dimension] - subTreeRoot->point[dimension])*(query[dimension] - subTreeRoot->point[dimension]);

  if (splitDistance <= radialDistance) {
    KDTreeNode* nextSubTreeRoot = leftChecked ? subTreeRoot->right : subTreeRoot->left;
    if (nextSubTreeRoot != nullptr) {
      Point<Dim> potentialNearest = findNearestNeighbor(nextSubTreeRoot, query, (dimension + 1) % Dim);

      if (shouldReplace(query, currentNearest, potentialNearest)) {
        currentNearest = potentialNearest;
      }
    }
  }

  return currentNearest;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    return findNearestNeighbor(root, query, 0);
}

template <int Dim>
int KDTree<Dim>::partition(int startIndex, int endIndex, int pivotIndex, int dimension) {
  Point<Dim> pivotValue = input_points_[pivotIndex];

  swap(input_points_[pivotIndex], input_points_[endIndex]);

  int currentPivotIndex = startIndex;

  for (int i = startIndex; i < endIndex; i++) {
    if (smallerDimVal(input_points_[i], pivotValue, dimension)) {
      swap(input_points_[currentPivotIndex], input_points_[i]);
      currentPivotIndex++;
    }
  }

  swap(input_points_[currentPivotIndex], input_points_[endIndex]);

  return currentPivotIndex;
}

template <int Dim>
void KDTree<Dim>::select(int startIndex, int endIndex, int pivotIndex, int dimension) {
  if (startIndex >= endIndex) {
    return;
  }

  int currentPivotIndex = partition(startIndex, endIndex, pivotIndex, dimension);

  if (currentPivotIndex > pivotIndex) {
    select(startIndex, currentPivotIndex - 1, pivotIndex, dimension);
  } else if (currentPivotIndex < pivotIndex) {
    select(currentPivotIndex + 1, endIndex, pivotIndex, dimension);
  }

  return;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(int startIndex, int endIndex, int dimension) {
  if (startIndex == endIndex) {
    return new KDTreeNode(input_points_[startIndex]);
  }

  int medianIndex = (startIndex + endIndex) / 2;

  select(startIndex, endIndex, medianIndex, dimension);

  KDTreeNode* subTreeRoot = new KDTreeNode(input_points_[medianIndex]);

  if (startIndex <= medianIndex - 1) {
    subTreeRoot->left = buildTree(startIndex, medianIndex - 1, (dimension + 1) % Dim);
  }

  if (endIndex >= medianIndex + 1) {
    subTreeRoot->right = buildTree(medianIndex + 1, endIndex, (dimension + 1) % Dim);
  }

  return subTreeRoot;
}
