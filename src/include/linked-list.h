#pragma once
#include <bstddef.h>

template<typename T>
class ListItem {
  explicit ListItem(T &value);

  ListItem *prev, *next;
  // wrapped value
  T &value;
};

template<typename T>
class LinkedList {
 public:
  typedef ListItem<T> Item;
  typedef void travel_callback(Item &);
  typedef bool compare_callback(const Item &);
  LinkedList();
  ~LinkedList();

  size_t length();

  Item *prepend(Item *item);
  Item *append(Item *item);

  void travel(travel_callback cb);
  void remove(compare_callback cb);

 private:
  Item *_head, *_tail;
  size_t _size;
};
