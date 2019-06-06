#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <queue>
#include <set>
#include <thread>
#include <utility>
using namespace std;
mutex testex;
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

// auto testqueue() {
//   std::queue<std::unique_ptr<testclass>> bla;
//   auto pt1 = make_unique<testclass>();
//   bla.emplace(pt1);
//   return bla.front();
// }
void testlockifelse() {
  lock_guard<mutex> lock(testex);

  lock_guard<mutex> lock2(testex);
}

void window_test() {
  initscr();
  refresh();
  int x = 0, y = 0;
  getmaxyx(stdscr, y, x);
  WINDOW *one = newwin(y, x / 2, 0, 0);
  WINDOW *two = newwin(y, x / 2, 0, x / 2);
  std::thread tone([&one]() {
    for (int loop = 0; loop < 20; loop++) {
      mvwaddstr(one, loop, 0, "thread one");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      wrefresh(one);
    }
    delwin(one);
  });
  std::thread ttwo([&two]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    for (int loop = 0; loop < 20; loop++) {
      mvwaddstr(two, loop, 0, "thread two");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      wrefresh(two);
    }
    delwin(two);
  });

  tone.join();
  ttwo.join();
  endwin();
}
void basic_hello_window() {
  initscr();
  endwin();
  delwin(stdscr);
}
int main() {
  basic_hello_window();
  // ExitProgram();
  // _nc_free_and_exit();
  cout << "main about to pop\n";
}
