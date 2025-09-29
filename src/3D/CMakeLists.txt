
file(GLOB files_library_3d "./*.cpp" "./glad/*.c")
add_library(library_3d STATIC ${files_library_3d})

target_compile_features(library_3d PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} -Wno-write-strings")

find_package(PNG)
if(PNG_FOUND)
	target_compile_definitions(library_3d PRIVATE USE_LIBPNG)
endif()
find_package(JPEG)
if(JPEG_FOUND)
	target_compile_definitions(library_3d PRIVATE USE_LIBJPEG)
endif()

find_package (Eigen3 REQUIRED NO_MODULE)
target_compile_definitions(library_3d PRIVATE USE_EIGEN)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_package(wxWidgets REQUIRED COMPONENTS gl core base html xml adv ribbon propgrid)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

target_link_libraries(library_3d
	${PNG_LIBRARIES}
	${JPEG_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${wxWidgets_LIBRARIES}
	Eigen3::Eigen
	library_math
)
