#ifndef queue_h
#define queue_h
#include <memory>
#include <mutex>

template <class T> class queue {
public:
  struct node {
    std::unique_ptr<T> item;
    std::unique_ptr<node> next;
    node() : item(nullptr), next(nullptr) {}
    std::unique_ptr<T> get() { return std::move(this->item); }
    void set(std::unique_ptr<T> &&arg) { item = std::move(arg); }
    void setnext(std::unique_ptr<node> arg) { next = std::move(arg); }
    std::unique_ptr<node> getnext() { return std::move(this->next); }
    const std::unique_ptr<node> &refnext() { return this->next; }

    void clear() {
      if (next) {
        next->clear();
        next.reset();
      }
      item.reset();
    }
  };
  queue() : first(std::make_unique<node>()) {}
  void push(std::unique_ptr<T> arg) {
    std::lock_guard<std::mutex> lck(mtx);
    if (last == nullptr) {
      first = std::make_unique<node>();
      first->set(std::move(arg));
      last = first.get();
    } else {
      last->setnext(std::make_unique<node>());
      last = last->refnext().get();
      last->set(std::move(arg));
    }
  }
  // ~queue() { first->clear(); }
  std::unique_ptr<T> pop() {
    std::lock_guard<std::mutex> lck(mtx);
    if (!first) {
      return std::make_unique<T>();
    }
    if (first.get() == last) {
      last = nullptr;
      auto temp = std::move(first->get());
      first.reset();
      return temp;
    }
    auto temp = std::move(first);
    first = std::move(temp->getnext());
    return std::move(temp->get());
  }
  bool empty() const {
    // std::lock_guard<std::mutex> lck(mtx);
    if (last == nullptr)
      return true;
    return false;
  }

private:
  std::mutex mtx;
  std::unique_ptr<node> first;
  node *last = nullptr;
};
template <typename T> std::unique_ptr<T> func(std::unique_ptr<T> arg) {
  return arg;
}

#endif /* end of include guard: queue_h */
