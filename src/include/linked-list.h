#pragma once
#include <bstddef.h>

class ListItem {
 public:
  explicit ListItem(void *value);
  static void clean_up(ListItem *item);

  // remove this from the list
  void remove() const;

  ListItem *prev, *next;
  // wrapped value
  void *value;
};

class LinkedList {
 public:
  typedef void travel_callback(ListItem &);
  typedef bool compare_callback(const ListItem &);
  LinkedList();
  ~LinkedList();

  size_t length();

  ListItem *prepend(ListItem *item);
  ListItem *append(ListItem *item);

  void travel(travel_callback cb);
  void remove(compare_callback cb);

 private:
  ListItem *_head, *_tail;
  size_t _size;
};
