#include <queuep/queuep.h>
#include <iostream>

QueueP::QueueP(const QueueP& copy) : head_(nullptr) {
  if (copy.Empty()) return;
  head_ = std::make_unique<Node>();
  head_->value = copy.Peek();
  Node *current_this = head_.get();
  const Node *current_copy = copy.head_->next.get();
  while (current_copy != nullptr) {
    current_this->next = std::make_unique<Node>();
    current_this = current_this->next.get();
    current_this->value = current_copy->value;
    current_copy = current_copy->next.get();
  }
  return;
}

QueueP::QueueP(QueueP&& move) : head_(std::exchange(move.head_, nullptr)) {
  return;
}

QueueP& QueueP::operator=(const QueueP& rhs) {
  QueueP copy(rhs);
  std::swap(*this, copy);
  return *this;
}

QueueP& QueueP::operator=(QueueP&& rhs) {
  QueueP move(rhs);
  std::swap(*this, move);
  return *this;
}

QueueP::~QueueP() {
  Clear();
  return;
}

void QueueP::Push(const int value) {
  std::unique_ptr<Node> new_node = std::make_unique<Node>();
  new_node->value = value;
  if (head_ == nullptr || head_->value > value) {
    new_node->next = std::move(head_);
    head_ = std::move(new_node);
    return;
  }
  Node *current = head_.get();
  while (current->next != nullptr && value >= current->next->value) {
    current = current->next.get();
  }
  new_node->next = std::move(current->next);
  current->next = std::move(new_node);
  return;
}

void QueueP::Pop() {
  if (head_ == nullptr) throw std::out_of_range("QueueP error: tried to pop from an empty QueueP");
  head_ = std::move(head_->next);
  return;
}

int QueueP::Peek() const {
  if (head_ == nullptr) throw std::out_of_range("QueueP error: tried to peek in an empty QueueP");
  return head_->value;
}

bool QueueP::Empty() const {
  return head_ == nullptr;
}

void QueueP::Clear() {
  while (head_ != nullptr) {
    head_ = std::move(head_->next);
  }
  return;
}

std::ostream& QueueP::WriteTo(std::ostream& ostream) const {
  const Node* current = head_.get();
  ostream << kOpenBracket << ' ';
  while (current != nullptr) {
    ostream << current->value << ' ';
    current = current->next.get();
  }
  ostream << kCloseBracket;
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const QueueP &data) {
  return data.WriteTo(ostream);
}
