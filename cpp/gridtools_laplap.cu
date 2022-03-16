#include <iostream>

#include "gridtools/common/halo_descriptor.hpp"
#include "gridtools/common/host_device.hpp"
#include "gridtools/stencil/cartesian.hpp"
#include "gridtools/stencil/frontend/cartesian/tmp_arg.hpp"
#include "gridtools/stencil/frontend/make_grid.hpp"
#include "gridtools/stencil/frontend/run.hpp"
#include "gridtools/storage/builder.hpp"
#include "gridtools/storage/sid.hpp"

#include "gridtools/stencil/gpu.hpp"
#include "gridtools/storage/gpu.hpp"

using namespace gridtools;
using namespace stencil;

struct lap {
  using in = cartesian::in_accessor<0, extent<-1, 1, -1, 1>>;
  using out = cartesian::inout_accessor<1>;

  using param_list = make_param_list<in, out>;

  template <class Eval> GT_FUNCTION static void apply(Eval &&eval) {
    eval(out()) = -4.0 * eval(in()) + eval(in(1, 0)) + eval(in(-1, 0)) +
                  eval(in(0, 1)) + eval(in(0, -1));
  }
};

template <class Grid, class In, class Out>
void laplap_gridtools(Grid const &grid, In &&in, Out &&out) {
  auto spec = [](auto in, auto out) {
    GT_DECLARE_TMP(double, tmp);
    return execute_parallel()
        .ij_cached(tmp)
        .stage(lap{}, in, tmp)
        .stage(lap{}, tmp, out);
  };

  run(spec, gpu{}, grid, in, out);
}

int main() {
  constexpr std::size_t Nx = 64;
  constexpr std::size_t Ny = 64;
  constexpr std::size_t Nz = 20;

  auto builder =
      storage::builder<storage::gpu>.type<double>().dimensions(Nx, Ny,
                                                                      Nz);

  auto input = builder.build();
  auto result = builder.build();

  halo_descriptor di{2, 2, 2, Nx - 2 - 1, Nx};
  halo_descriptor dj{2, 2, 2, Nx - 2 - 1, Nx};
  auto grid = make_grid(di, dj, Nz);

  laplap_gridtools(grid, input, result);


  std::cout << "done!" << std::endl;
}
