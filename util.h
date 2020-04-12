#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <string>


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

template <typename T>
void print_hex(T const &t)
{
  printf("0x%x", t);
  printf("\n");
};

template <typename T>
void print(T const &t)
{
  std::cout << t << std::endl;
};

template <typename T>
bool in_range(T const &t, T const &min, T const &max)
{
  return 
  (
    (t - min) <= (max - min)
  );
};

#endif