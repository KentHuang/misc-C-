#include <memory>
 
using std::allocator;

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

  size_type size() const { return limit - data; }
  T& operator[](size_type i) { return data[i]; }
  const T& operator[](size_type i) const { return data[i]; }

  iterator begin() { return data; }
  const_iterator begin() const { return data; }

  iterator end() { return limit; }
  const_iterator end() const { return limit; }

  Vec& operator=(const Vec&);
 
  void push_back(const T& val) {
    if (avail == limit)
      grow();
    unchecked_append(val);
  }

private:
  iterator data;	// first element in the Vec
  iterator avail; 	// point to one past the last constructed element
  iterator limit;	// one past the last available element in the Vec

  allocator<T> alloc;

  void create();
  void create(size_type, const T&);
  void create(const_iterator, const_iterator);

  void uncreate();

  void grow();
  void unchecked_append(const T&);
};


