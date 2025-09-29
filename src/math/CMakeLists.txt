
file(GLOB files_library_math "./*.cpp")
add_library(library_math STATIC ${files_library_math})

target_compile_features(library_math PRIVATE cxx_std_17)

find_package (Eigen3 REQUIRED NO_MODULE)
target_compile_definitions(library_math PRIVATE USE_EIGEN)

target_link_libraries(library_math
	Eigen3::Eigen
)
