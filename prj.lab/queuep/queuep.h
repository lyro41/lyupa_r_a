#ifndef QUEUEP
#define QUEUEP

#include <memory>
#include <exception>
#include <iosfwd>

// priority queue where an element with the least priority has the greatest value
class QueueP {
 public:
  // default constructor
  QueueP() = default;
  // copy constructor
  QueueP(const QueueP& copy);
  // move constructor
  QueueP(QueueP&& move);

  // copy assignment
  QueueP& operator=(const QueueP& rhs);
  // move assignment
  QueueP& operator=(QueueP&& rhs);

  // destructor
  ~QueueP();

  // add an element with given value to the priority queue
  void Push(const int value);
  // remove the greatest priority element from the queue
  void Pop();
  // get the greatest priority element's value
  int Peek() const;
  // check if the priority queue is empty
  bool Empty() const;
  // remove all the elements from the priority queue
  void Clear();

  std::ostream& WriteTo(std::ostream& ostream) const;

 private:
  struct Node {
    std::unique_ptr<Node> next{ nullptr };
    int value{ 0 };
  };

  void Swap(QueueP &other);

  static const char kOpenBracket{ '{' };
  static const char kCloseBracket{ '}' };

  // pointer to the greatest priority element
  std::unique_ptr<Node> head_{ nullptr };
};

std::ostream& operator<<(std::ostream& ostream, const QueueP &data);

#endif // QueueP
