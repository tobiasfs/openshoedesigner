
add_executable(openshoedesigner main.cpp)

file(GLOB files_openshoedesigner "./*.cpp")
list(REMOVE_ITEM files_openshoedesigner "main.cpp")
target_sources(openshoedesigner PRIVATE ${files_openshoedesigner})

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(UsewxWidgets)


target_link_libraries(openshoedesigner 
  PRIVATE
	${wxWidgets_LIBRARIES}
	library_3d
	library_math
	library_system
	gui
	project
)

add_subdirectory(3D)
add_subdirectory(gui)
add_subdirectory(math)
add_subdirectory(project)
add_subdirectory(system)
