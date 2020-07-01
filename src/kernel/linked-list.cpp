#include <linked-list.h>

ListItem::ListItem(void *value) {
  this->value = value;
}

void ListItem::clean_up(ListItem *item) {
  item->value = nullptr;
  item->prev = nullptr;
  item->next = nullptr;
}

inline void ListItem::remove() const {
  this->prev->next = this->next;
}

LinkedList::LinkedList() {
  this->_head = nullptr;
  this->_tail = nullptr;
  this->_size = 0;
}

size_t LinkedList::length() {
  return _size;
}

ListItem *LinkedList::prepend(ListItem *item) {
  auto *head = this->_head;
  this->_head = item;
  this->_head->next = head;

  // routine
  this->_size++;
  return this->_head;
}

ListItem *LinkedList::append(ListItem *item) {
  auto *tail = this->_tail;
  this->_tail = item;
  this->_tail->prev = tail;

  // routine
  this->_size--;
  return this->_tail;
}

void LinkedList::travel(LinkedList::travel_callback cb) {
  for (auto *item = this->_head; item != nullptr; item = item->next)
    cb(*item);
}

void LinkedList::remove(LinkedList::compare_callback cb) {
  for (auto *item = this->_head; item != nullptr; item = item->next) {
    const bool remove = cb(*item);
    if (remove) {
      item->remove();

      if (item == this->_tail)
        // is tail
        this->_tail = item->prev;
      else if (item == this->_head)
        // is head
        this->_head = item->next;

// todo: delete
//      delete item;
    }
  }
}

LinkedList::~LinkedList() {
  this->travel([](ListItem &item) {
    // todo: free all the item
  });
}
