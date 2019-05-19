#include <chrono>
#include <iostream>
#include <thread>
void pause_thread(int n) {
  std::cout << "ttest\n";
  std::this_thread::sleep_for(std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}
int main() {
  std::thread a(pause_thread, 2);
  // pause_thread(3);
  a.join();
  return 0;
}
