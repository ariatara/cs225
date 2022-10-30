#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../lib/cs225/PNG.h"
#include "../lib/cs225/HSLAPixel.h"
#include "../lib/lodepng/lodepng.h"

#include "../src/List.h"
using namespace cs225;
using namespace std;

int main() {
  List<int> list;

  list.insertBack(2);
  list.insertBack(5);
  list.insertBack(3);

  List<int>::ListIterator iter = list.end();
  std::cout << "hi" << std::endl;
  iter--;
  std::cout << "hih" << std::endl;
  iter--;

  std::cout << "bruh" << std::endl;
  iter++;
  iter++;


  std::cout << "howdy" << std::endl;
}

/* template <typename T>
void List<T>::print() {
  std::cout << "printing" << std::endl;
  ListNode* curr = head_;

  while (curr != NULL) {
    std::cout << curr->data << std::endl;
    curr = curr->next;
  }
} */