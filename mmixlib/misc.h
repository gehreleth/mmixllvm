#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <string>
#include <atomic>
#include <vector>

namespace MmixLlvm {
std::string toMultibyte(const wchar_t* src);

std::wstring toWideChar(const char* src);

template <typename T>
std::vector<T> singletonSet(T t)
{
  std::vector<T> Vec;
  Vec.push_back(std::move(t));
  return Vec;
}

std::string genUniq(const char* prefix);

class SpinLock {
  std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) { ; }
  }

  bool try_lock() {
    return !locked.test_and_set(std::memory_order_acquire);
  }

  void unlock() {
    locked.clear(std::memory_order_release);
  }
};
};

#endif MISC_H_INCLUDED