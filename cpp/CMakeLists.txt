cmake_minimum_required(VERSION 3.16.2)

project(gt_framework_talk LANGUAGES CXX CUDA)

add_compile_options(-fsanitize=address,undefined)
add_compile_options(-Wall)
add_link_options(-fsanitize=address,undefined)

add_executable(naive_laplap naive_laplap.cc)
target_compile_features(naive_laplap PRIVATE cxx_std_17)

include(FetchContent)
FetchContent_Declare(
    gridtools
    GIT_REPOSITORY https://github.com/GridTools/gridtools.git
    GIT_TAG        master
    )
FetchContent_MakeAvailable(GridTools)

add_executable(gridtools_laplap gridtools_laplap.cu)
target_link_libraries(gridtools_laplap GridTools::gridtools GridTools::stencil_gpu GridTools::storage_gpu)
gridtools_setup_target(gridtools_laplap CUDA_ARCH sm_75)

add_executable(test_cuda test_cuda.cu)
target_link_libraries(test_cuda GridTools::gridtools GridTools::stencil_gpu GridTools::storage_gpu)
gridtools_setup_target(test_cuda CUDA_ARCH sm_75)
