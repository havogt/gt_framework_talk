import numpy as np

from functional.common import Field
from functional.ffront.decorator import field_operator, program
from functional.iterator.embedded import np_as_located_field
from functional.iterator.runtime import CartesianAxis, offset


IDim = CartesianAxis("IDim")
JDim = CartesianAxis("JDim")

Ioff = offset("Ioff")
Joff = offset("Joff")


@field_operator
def lap(
    in_field: Field[[IDim, JDim], "float"], a4: Field[[IDim, JDim], "float"]
) -> Field[[IDim, JDim], "float"]:
    return (
        a4 * in_field
        + in_field(Ioff[1])
        + in_field(Joff[1])
        + in_field(Ioff[-1])
        + in_field(Joff[-1])
    )


@field_operator
def laplap(
    in_field: Field[[IDim, JDim], "float"], a4: Field[[IDim, JDim], "float"]
) -> Field[[IDim, JDim], "float"]:
    return lap(lap(in_field, a4), a4)


@program
def laplap_program(
    in_field: Field[[IDim, JDim], "float"],
    a4: Field[[IDim, JDim], "float"],
    out_field: Field[[IDim, JDim], "float"],
):
    laplap(in_field, a4, out=out_field[2:-2, 2:-2])


def numpy_lap(inp):
    return (
        -4.0 * inp[1:-1, 1:-1]
        + inp[:-2, 1:-1]
        + inp[2:, 1:-1]
        + inp[1:-1, :-2]
        + inp[1:-1, 2:]
    )


def main():
    shape = (20, 20)
    as_ij = np_as_located_field(IDim, JDim)
    input = as_ij(np.fromfunction(lambda x, y: x**5 + y**5, shape))
    a4 = as_ij(np.ones(shape) * -4.0)  # TODO support scalar field

    result = as_ij(np.zeros_like(input))
    laplap_program(input, a4, result, offset_provider={"Ioff": IDim, "Joff": JDim})

    assert np.allclose(
        np.asarray(result)[2:-2, 2:-2], numpy_lap(numpy_lap((np.asarray(input))))
    )


if __name__ == "__main__":
    main()
