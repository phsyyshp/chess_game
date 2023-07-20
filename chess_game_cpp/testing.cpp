#include <iostream>
#include <string>
#include <vector>

std::vector<uint64_t> factor_mask(uint64_t mask) {
  std::vector<uint64_t> out_vec(__builtin_popcount(mask));
  int i = 0;
  int counter = 1;
  while (counter != 0) {
    counter = ((mask) & (mask - 1));
    // std::cout << counter << std::endl;
    out_vec[i] = mask ^ counter;
    mask = counter;
    i++;
  }
  return out_vec;
}

int main() {
  uint64_t mask = 0b10101010;
  std::vector<uint64_t> factors = factor_mask(mask);
  for (auto factor : factors) {
    std::cout << factor << " ";
  }
  std::cout << std::endl;
  return 0;
}
