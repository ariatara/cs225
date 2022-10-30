
#include <iostream>

class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    // @TODO: graded in MP3.1
    ListNode* position_;
    ListNode* tail_;
    int started = 0;

  public:
    ListIterator() : position_(NULL) { }
    ListIterator(ListNode* x) : position_(x) { }

    void setTail(ListNode& x) {
        tail_ = x;
    }


    // Pre-Increment, ++iter
    ListIterator& operator++() {
        // @TODO: graded in MP3.1
        started = 1;
        position_ = position_->next;
        return *this;
    }
    
    // Post-Increment, iter++
    ListIterator operator++(int) {
        // @TODO: graded in MP3.1
        started = 1;
        ListNode* temp = position_;
        position_ = position_->next;
        return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {
        // @TODO: graded in MP3.1
        started = 1;
        position_ = position_->prev;
        return *this;
    }

    // Post-Decrement, iter--
    ListIterator operator--(int) {
        // @TODO: graded in MP3.1
        std::cout << position_ << std::endl;
        std::cout << tail_->data << std::endl;
        if (started == 0) {
            std::cout << "entered" << std::endl;
            started = 1;
            return ListIterator(tail_);
        }
        ListNode* temp = position_;
        position_ = position_->prev;
        return ListIterator(temp);
    }

    bool operator!=(const ListIterator& rhs) {
        // @TODO: graded in MP3.1
        return position_ != rhs.position_;
    }

    bool operator==(const ListIterator& rhs) {
        return position_ == rhs.position_;
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
