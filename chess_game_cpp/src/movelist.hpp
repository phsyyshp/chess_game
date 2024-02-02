#pragma once

#include "Move.hpp"
#include "constants.hpp"
// TODO add iterator

class MoveList {
public:
  MoveList() : lastSetIdx(-1) {}

  void push_back(Move move);
  Move operator[](int n) const { return elements[n]; }
  int size() const;

  void swap(int i, int j);
  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Move;
    using pointer = Move *;
    using reference = Move &;

    iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    // Prefix increment
    iterator &operator++() {
      m_ptr++;
      return *this;
    }

    // Postfix increment
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const iterator &a, const iterator &b) {
      return a.m_ptr == b.m_ptr;
    };
    friend bool operator!=(const iterator &a, const iterator &b) {
      return a.m_ptr != b.m_ptr;
    };

  private:
    pointer m_ptr;
  };

  // Begin and end functions for iterator
  iterator begin() { return iterator(&elements[0]); }
  iterator end() { return iterator(&elements[lastSetIdx + 1]); }

private:
  int lastSetIdx;

  std::array<Move, 256> elements;
};