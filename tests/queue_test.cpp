#include "queue.hpp"
#include <catch2/catch.hpp>
#include <string>
TEST_CASE("a queue", "[queue]") {
  queue<std::string>::node q;
  // std::string a("hello");
  // auto aa = std::make_unique<std::string>("bla");
  // auto a2 = std::make_unique<std::string>("bla");
  // q.push(std::move(aa));
  // q.push(std::move(a2));
  // std::unique_ptr<std::string> b = q.pop();
  // REQUIRE((*b) == std::string("bla"));
  SECTION("sanity?") {

    auto b = std::make_unique<std::string>("hello");
    auto addr = b.get();
    auto a = func(std::move(b));
    REQUIRE(*a == std::string("hello"));
    REQUIRE(a.get() == addr);
  }
  SECTION("node?") {
    auto b = std::make_unique<std::string>("hello");
    auto addr = b.get();
    q.set(std::move(b));
    auto a = q.get();
    REQUIRE(*a == std::string("hello"));
    REQUIRE(a.get() == addr);
  }
  SECTION("two nodes") {
    // queue<std::string>::node q2;
    auto q2 = std::make_unique<queue<std::string>::node>();
    auto addr = q2.get();
    q.setnext(std::move(q2));
    auto a = q.getnext();
    REQUIRE(a.get() == addr);
  }
  SECTION("nested nodes") {
    auto a = std::make_unique<std::string>("string a");
    auto b = std::make_unique<std::string>("string b");
    auto q2 = std::make_unique<queue<std::string>::node>();
    auto q3 = std::make_unique<queue<std::string>::node>();
    (*q2).set(std::move(a));
    q3->set(std::move(b));
    q2->setnext(std::move(q3));
    auto temp = std::move(q2);
    q2 = temp->getnext();
    REQUIRE(*q2->get() == std::string("string b"));
  }
  SECTION("a queue") {
    auto a = std::make_unique<std::string>("string a");
    auto addr_a = a.get();
    auto b = std::make_unique<std::string>("string b");
    auto addr_b = b.get();
    queue<std::string> fifo;
    fifo.push(std::move(a));
    fifo.push(std::move(b));
    REQUIRE(fifo.pop().get() == addr_a);
    REQUIRE(fifo.pop().get() == addr_b);
  }
}