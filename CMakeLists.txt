cmake_minimum_required(VERSION 3.5)
project(main)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/shaders")
set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*")
file(GLOB_RECURSE SHADERS_FILES RELATIVE "${PROJECT_SOURCE_DIR}" CONFIGURE_DEPENDS "${SHADERS_DIR}/*")
file(GLOB_RECURSE ASSETS_FILES RELATIVE "${PROJECT_SOURCE_DIR}" CONFIGURE_DEPENDS "${ASSETS_DIR}/*")

# file(GLOB_RECURSE ... CONFIGURE_DEPENDS) automatically collects all data in directory
# CONFIGURE_DEPENDS says that if something changed `make` command will recreate cmake
#
# Possible issues:
# Documentations says:
# The CONFIGURE_DEPENDS flag may not work reliably on all generators,
# or if a new generator is added in the future that cannot support it, projects using it will be stuck.
# Even if CONFIGURE_DEPENDS works reliably, there is still a cost to perform the check on every rebuild."
#
# Also someone says:
# > Globbing breaks all code inspection in things like CLion that
# > otherwise understand limited subsets of CMakeLists.txt and do not and never will support globbing as it is unsafe
#
# So if you have issues you have to list all files manually
# Like this
# set(SOURCE_FILES
#   main.cpp
#   otherfile.cpp
#   ...
# )



# Link to headers and compiled libraries

set(ADDITIONAL_INCLUDE_DIRS
      dependencies/include/GLAD
      dependencies/include/LiteMath
      dependencies/include/
)
set(ADDITIONAL_LIBRARY_DIRS dependencies/lib)
set(ADDITIONAL_RUNTIME_LIBRARY_DIRS dependencies/bin)


# Compile additional libraries from sources

set(COMPILED_LIBRARIES
    Hydra
    TinyXML2
    TinyGLTF
    GLAD)
add_subdirectory(dependencies/include/Hydra)
add_subdirectory(dependencies/include/TinyXML2)
add_subdirectory(dependencies/include/TinyGLTF)
add_subdirectory(dependencies/include/GLAD)

if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
else()
	find_package(glfw3 REQUIRED)
endif()

# Copy shaders and assets on build

foreach(resource ${SHADERS_FILES} ${ASSETS_FILES})
  set(res_in  "${PROJECT_SOURCE_DIR}/${resource}")
  set(res_out "${PROJECT_BINARY_DIR}/${resource}")

  add_custom_command(
    COMMENT "Moving updated resource-file '${resource}'"
    OUTPUT ${res_out}
    DEPENDS ${res_in}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${res_in}
    ${res_out}
  )
  string(REPLACE "/" "-" target_name ${resource})
  add_custom_target("${target_name}" ALL DEPENDS ${res_in} ${res_out})
endforeach()

# Build

find_package(OpenGL REQUIRED)
add_executable(main ${SOURCE_FILES})
target_include_directories(main PRIVATE ${OPENGL_INCLUDE_DIR} ${ADDITIONAL_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGLFW_INCLUDE_NONE")

if(WIN32)
  set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGLFW_DLL")
  add_custom_command(TARGET main POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/dependencies/bin" $<TARGET_FILE_DIR:main>)
  #set(CMAKE_MSVCIDE_RUN_PATH ${ADDITIONAL_RUNTIME_LIBRARY_DIRS})
  target_compile_options(main PRIVATE)
  target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw3dll ${COMPILED_LIBRARIES})
elseif(APPLE)
  set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_POSIX_C_SOURCE=200809L")
  target_compile_options(main PRIVATE -Wnarrowing)
  target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw "-framework OpenCL" dl ${COMPILED_LIBRARIES})
else()
  target_compile_options(main PRIVATE -Wnarrowing)
  target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw rt dl ${COMPILED_LIBRARIES})
endif()