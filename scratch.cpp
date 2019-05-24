#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <thread>
#include <utility>
using namespace std;
class testclass {
public:
  testclass() { cout << "testclass constr\n"; }
  ~testclass() { cout << "testclass desctr\n"; }
  testclass(testclass &&) { cout << "move constr\n"; }
  testclass(const testclass &) { cout << "copy constr\n"; }
};
using tptr = unique_ptr<testclass>;

map<char, tptr> testmap;

void testfunc1() {
  tptr one = make_unique<testclass>();
  testmap.insert(make_pair('a', std::move(one)));
  testclass b(*testmap.begin()->second.get());
}
// using now() = chrono::system_clock::now();
void testchrono() {
  auto limit = chrono::system_clock::now() + chrono::seconds(2);
  while (chrono::system_clock::now() < limit)
    ;
  cout << "times up\n";
  // cout << chrono::system_clock::now().time_since_epoch();
  // cout << now << endl;
}

auto testqueue() {
  std::queue<std::unique_ptr<testclass>> bla;
  auto pt1 = make_unique<testclass>();
  bla.emplace(pt1);
  return bla.front();
}

int main() {
  std::unique_ptr<testclass> = testqueue();
  cout << "main about to pop\n";
}
