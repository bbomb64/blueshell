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

inline std::vector<bool> bytes_to_bits(std::vector<u8> const &v)
{
  std::vector<bool> ret;
  for (int j = 0; j < v.size(); j++)
  {
    for (int i = 0; i < 8; i++)
    {
      ret.push_back((v[j] >> i) & 0x01);
    }
  }
  return ret;
}

inline u8 bits_to_byte(std::vector<bool> const &v)
{
  u8 byte = 0;
  for (int i = 0; i < 8; i++)
  {
    byte += (v[i] << (7 - i));
  }
  return byte;
}

#endif