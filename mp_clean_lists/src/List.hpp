/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */
#include <iostream>

template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  List<T>::ListIterator li = List<T>::ListIterator(head_);
  li.setTail(tail_);

  return li;
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  List<T>::ListIterator li = List<T>::ListIterator(NULL);
  li.setTail(tail_);

  return li;
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  while (head_ != NULL) {
    ListNode* temp = head_->next;
    delete head_;
    head_ = temp;
  }

  head_ = NULL;
  tail_ = NULL;
  length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode* nNode = new ListNode(ndata);
  
  if (length_ == 0) {
    head_ = nNode;
    tail_ = nNode;
  } else {
    nNode->next = head_;
    head_->prev = nNode;

    head_ = nNode;
  }
  
  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode* nNode = new ListNode(ndata);
  
   if (length_ == 0) {
    head_ = nNode;
    tail_ = nNode;
  } else {
    nNode->prev = tail_;
    tail_->next = nNode;

    tail_ = nNode;
  }

  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk temp splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode* List<T>::split(ListNode* start, int splitPoint) {
  /// @todo Graded in MP3.1

  if (splitPoint > length_ || splitPoint <= 0) {
    return head_;
  }

  for (int i = 0; i < splitPoint; i++) {
    start = start->next;
  }

  ListNode* otherTail = start->prev;

  start->prev = NULL;
  otherTail->next = NULL;

  return start;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  // @todo Graded in MP3.1
  ListNode* curr = head_;
  int count = 0;

  while (curr != NULL) {
    if (count == 2) {
      ListNode* first = curr->prev->prev;
      ListNode* second = curr->prev;
      ListNode* third = curr;

      first->next = third->next;

      if (third->next != NULL) {
        third->next->prev = first;
      }

      second->prev = first->prev;
      if (first->prev != NULL) {
        first->prev->next = second;
      }

      first->prev = third;

      third->next = first;

      if (head_ == first) {
        head_ = second;
      }

      if (tail_ == third) {
        tail_ = first;
      }

      curr = first;
      count = 0;
    } else {
      count++;
    }

    curr = curr->next;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2
  if (startPoint == endPoint || startPoint == NULL || endPoint == NULL) {
    return;
  }

  ListNode* beforeStart = startPoint->prev;
  ListNode* curr = startPoint;
  ListNode* afterEnd = endPoint->next;

  while (curr != endPoint) {
    ListNode* currentNext = curr->next;
    curr->next = curr->prev;
    curr->prev = currentNext;
    curr = currentNext;
  }

  startPoint->next = endPoint->next;

  endPoint->next = endPoint->prev;
  endPoint->prev = beforeStart;

  ListNode* temp = endPoint;
  endPoint = startPoint;
  startPoint = temp;

  if (beforeStart != NULL) {
    beforeStart->next = startPoint;
  }

  if (afterEnd != NULL) {
    afterEnd->prev = endPoint;
  }

}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
  if (n <= 1 || length_ <= 1) {
    return;
  }

  int blocks = length_ / n;
  ListNode* startPoint = head_;
  ListNode* endPoint = head_;

  for (int i = 0; i < blocks; i++) {
    for (int j = 1; j < n; j++) {
      endPoint = endPoint->next;
    }

    if (startPoint == head_) {
      reverse(head_, endPoint);
    } else if (endPoint == tail_) {
      reverse(startPoint, tail_);
    } else {
      reverse(startPoint, endPoint);
    }

    startPoint = endPoint->next;
    endPoint = endPoint->next;
  } 

  if ((length_ % n) != 0) {
    reverse(startPoint, tail_);
  }

}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  if (first == NULL) {
    return second;
  } else if (second == NULL) {
    return first;
  }

  ListNode* sortedHead;
  ListNode* firstHead = first;
  ListNode* secondHead = second;
  
  if (firstHead->data < secondHead->data) {
    sortedHead = firstHead;
    firstHead = firstHead->next;
  } else {
    sortedHead = secondHead;
    secondHead = secondHead->next;
  }

  ListNode* curr = sortedHead;
  while (firstHead != NULL && secondHead != NULL) {
    if (firstHead->data < secondHead->data) {
      curr->next = firstHead;
      firstHead->prev = curr;
      firstHead = firstHead->next;
    } else {
      curr->next = secondHead;
      secondHead->prev = curr;
      secondHead = secondHead->next;
    }

    curr = curr->next;
  }

  if (firstHead == NULL && secondHead != NULL) {
    curr->next = secondHead;
    secondHead->prev = curr;
  } else if (secondHead == NULL && firstHead != NULL) {
    curr->next = firstHead;
    firstHead->prev = curr;
  }

  return sortedHead;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  if (start->next == NULL) {
    return start;
  }

  ListNode* secondHead = split(start, (int) (chainLength / 2));

  start = mergesort(start, (int) (chainLength / 2));
  secondHead = mergesort(secondHead, (int) ((chainLength + 1)/ 2));

  start = merge(start, secondHead);
  return start;
}
