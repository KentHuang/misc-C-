#include <iostream>
#include <memory>
 
using std::allocator;
using std::uninitialized_copy;
using std::uninitialized_fill;


template <class T> 
class Vec {
public:
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef size_t size_type;
  typedef T value_type;  

  Vec() { create(); }
  explicit Vec(size_type n, const T& val = T()) { create(n, val); }
  Vec(const Vec& v) { create(v.begin(), v.end()); }

  ~Vec() { uncreate(); }

  size_type size() const { return avail - data; }
  size_type capacity() const { return limit - data;  }
  T& operator[](size_type i) { return data[i]; }
  const T& operator[](size_type i) const { return data[i]; }

  iterator begin() { return data; }
  const_iterator begin() const { return data; }

  iterator end() { return avail; }
  const_iterator end() const { return avail; }

  Vec& operator=(const Vec&);
 
  void push_back(const T& val) {
    if (avail == limit)
      grow();
    unchecked_append(val);
  }

  iterator erase(iterator pos);
  iterator erase(iterator first, iterator end);

  void clear();

private:
  iterator data;  // first element in the Vec
  iterator avail;   // point to one past the last constructed element
  iterator limit; // one past the last available element in the Vec

  allocator<T> alloc;

  void create();
  void create(size_type, const T&);
  void create(const_iterator, const_iterator);

  void uncreate();

  void grow();
  void unchecked_append(const T&);
};


template <class T>
const T& max(const T& a, const T& b) {
  return (a < b) ? b : a;
}

template <class T>
Vec<T>& Vec<T>::operator=(const Vec& rhs) {
  if (&rhs != this) {
    uncreate();
    create(rhs.begin(), rhs.end());
  }
  return *this;
}

template <class T>
void Vec<T>::create() {
  data = avail = limit = 0;
}

template <class T>
void Vec<T>::create(size_type n, const T& val) {
  data = alloc.allocate(n);
  limit = avail = data + n;
  uninitialized_fill(data, limit, val);
}

template <class T> 
void Vec<T>::create(const_iterator i, const_iterator j) {
  data = alloc.allocate(j - i);
  limit = avail = uninitialized_copy(i, j, data);
}

template <class T> 
void Vec<T>::uncreate() {
  if (data) {
    iterator it = avail;
    while (it != data)
      alloc.destroy(--it);

    alloc.deallocate(data, limit - data);
  }
  data = limit = avail = 0;
}

template <class T>
void Vec<T>::grow() {
  size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

  iterator new_data = alloc.allocate(new_size);
  iterator new_avail = uninitialized_copy(data, avail, new_data);

  uncreate();

  data = new_data;
  avail = new_avail;
  limit = data + new_size;
}

template <class T> 
void Vec<T>::unchecked_append(const T& val) {
  alloc.construct(avail++, val);
}

template <class T>
T* Vec<T>::erase(iterator pos) {
  alloc.destroy(pos);
  if (pos+1 == avail)
    return --avail;
  iterator new_avail = uninitialized_copy(pos+1, avail, pos);
  alloc.destroy(new_avail);
  avail = new_avail;
  return pos;
}

template <class T>
T* Vec<T>::erase(iterator first, iterator last) {
  iterator temp = first;
  while (temp != last) {
    alloc.destroy(temp++);
  }
  iterator new_avail = uninitialized_copy(temp, avail, first);
  while (avail != new_avail)
    alloc.destroy(--avail);
  avail = new_avail;
  return first;
}

template <class T>
void Vec<T>::clear() {
  size_type n = limit - data;
  uncreate();
  data = avail = alloc.allocate(n);
  limit = data + n;
}

