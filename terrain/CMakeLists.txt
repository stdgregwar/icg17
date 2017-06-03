get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# these files are copied only once
file(GLOB TEXTURES
    Grid/*.jpg
    Grid/*.png
    Skybox/*.jpg
    Skybox/*.png
    Tree/*.png
   )
copy_files_once(${TEXTURES})

# list all the files you want to copy everytime
# you build (ie, you want the shaders there)
file(GLOB SHADERS
  ScreenQuad/*.glsl
  Grid/*.glsl
  Skybox/*.glsl
  Tree/*.glsl
  Particles/*.glsl)
deploy_shaders_to_build_dir(${SHADERS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS} ${CMAKE_THREAD_LIBS_INIT})
