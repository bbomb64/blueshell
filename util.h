#ifndef UTIL_H
#define UTIL_H

#include <iostream>

template <typename T>
void print_vec(std::vector<T> const &v)
{
  std::cout << "{ ";
  for (auto i : v)
  {
    printf("0x%x", i);
    printf(", ");
  }
  std::cout << " }" << std::endl;
};

#endif