#include <cstddef>
#include <iostream>

#include "naive_laplap.hpp"
#include "naive_laplap2.hpp"

int main() {
  constexpr std::size_t Nx = 64;
  constexpr std::size_t Ny = 64;
  constexpr std::size_t Nz = 64;

  auto input = new double[Nx][Ny][Nz];
  auto result = new double[Nx][Ny][Nz];
  auto result2 = new double[Nx][Ny][Nz];
  auto tmp = new double[Nx][Ny][Nz];

  constexpr std::array<std::size_t, 3> sizes{Nx, Ny, Nz};
  laplap_naive(sizes, input, result, tmp);
  laplap_naive2(sizes, input, result2, tmp);

  for (std::size_t i = 2; i < Nx - 2; ++i) {
    for (std::size_t j = 2; j < Ny - 2; ++j) {
      for (std::size_t k = 0; k < Nz; ++k) {
        if (result[i][j][k] != result2[i][j][k]) {
          std::cout << "error at " << i << "/" << j << "/" << k << std::endl;
          exit(1);
        }
      }
    }
  }

  delete[] input;
  delete[] tmp;
  delete[] result;
  delete[] result2;

  std::cout << "done!" << std::endl;
}
