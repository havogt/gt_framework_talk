#pragma once

#include <array>

using domain_t = std::array<std::array<std::size_t, 2>, 3>;

template <class Fun, class... Args>
void apply(domain_t domain, Fun &&fun, Args &&...args) {
  for (std::size_t i = domain[0][0]; i < domain[0][1]; ++i) {
    for (std::size_t j = domain[1][0]; j < domain[1][1]; ++j) {
      for (std::size_t k = domain[2][0]; k < domain[2][1]; ++k) {
        fun(std::forward<Args>(args)..., i, j, k);
      }
    }
  }
}

constexpr auto lap3 = [](auto const &in, auto &out, std::size_t i,
                         std::size_t j, std::size_t k) {
  out[i][j][k] = -4.0 * in[i][j][k] //
                 + in[i + 1][j][k]  //
                 + in[i - 1][j][k]  //
                 + in[i][j + 1][k]  //
                 + in[i][j - 1][k];
};

template <class In, class Out, class Tmp>
void laplap_naive3(std::array<std::size_t, 3> sizes, In const &input,
                   Out &output, Tmp &&tmp) {
  apply(domain_t{{{1, sizes[0] - 1}, {1, sizes[1] - 1}, {0, sizes[2]}}}, lap3,
        input, tmp);
  apply(domain_t{{{2, sizes[0] - 2}, {2, sizes[1] - 2}, {0, sizes[2]}}}, lap3,
        tmp, output);
}
