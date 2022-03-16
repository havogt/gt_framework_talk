import numpy as np
import matplotlib.pyplot as plt

import gt4py
from gt4py import gtscript
import gt4py.storage as gt_storage

backend = "gtc:cuda"
backend = "gtc:numpy"
backend = "gtc:dace"
dtype = np.float64


@gtscript.function
def lap(in_field):
    out_field = (
        -4.0 * in_field
        + in_field[1, 0, 0]
        + in_field[-1, 0, 0]
        + in_field[0, 1, 0]
        + in_field[0, -1, 0]
    )
    return out_field


@gt4py.gtscript.stencil(backend=backend, rebuild=True)
def laplap(in_field: gtscript.Field[dtype], out_field: gtscript.Field[dtype]):
    with computation(PARALLEL), interval(...):
        tmp = lap(in_field)
        out_field = lap(tmp)


def main():
    Nx = 64
    Ny = 64
    Nz = 64

    shape = (Nx, Ny, Nz)
    origin = (2, 2, 0)

    in_storage = gt_storage.from_array(
        np.fromfunction(lambda x, y, _: x**2 + y**2, shape, dtype=dtype),
        default_origin=origin,
        backend=backend,
    )
    out_storage = gt_storage.from_array(
        np.zeros(shape, dtype=dtype), default_origin=origin, backend=backend
    )

    laplap(in_storage, out_storage)

    # plt.imshow(np.asarray(out_storage)[:, :, 0])


if __name__ == "__main__":
    main()
