
file(GLOB files_gui "./*.cpp")
add_library(gui STATIC ${files_gui})

target_compile_features(gui PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} -Wno-write-strings")

find_package(wxWidgets REQUIRED COMPONENTS gl core base html xml adv stc aui ribbon propgrid)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

target_link_libraries(gui
	# ${PNG_LIBRARIES}
	# ${OPENGL_LIBRARIES}
	${wxWidgets_LIBRARIES}
)
