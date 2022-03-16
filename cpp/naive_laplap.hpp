#pragma once

#include <array>

template <class In, class Out, class Tmp>
void laplap_naive(std::array<std::size_t, 3> sizes, In const &input,
                  Out &output, Tmp &&tmp) {
  for (std::size_t i = 1; i < sizes[0] - 1; ++i) {
    for (std::size_t j = 1; j < sizes[1] - 1; ++j) {
      for (std::size_t k = 0; k < sizes[2]; ++k) {
        tmp[i][j][k] = -4.0 * input[i][j][k] //
                       + input[i + 1][j][k]  //
                       + input[i - 1][j][k]  //
                       + input[i][j + 1][k]  //
                       + input[i][j - 1][k];
      }
    }
  }
  for (std::size_t i = 2; i < sizes[0] - 2; ++i) {
    for (std::size_t j = 2; j < sizes[1] - 2; ++j) {
      for (std::size_t k = 0; k < sizes[2]; ++k) {
        output[i][j][k] = -4.0 * tmp[i][j][k] //
                          + tmp[i + 1][j][k]  //
                          + tmp[i - 1][j][k]  //
                          + tmp[i][j + 1][k]  //
                          + tmp[i][j - 1][k];
      }
    }
  }
}
