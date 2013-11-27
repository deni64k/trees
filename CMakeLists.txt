cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(trees)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS                "-std=c++11 -fpermissive -Wall -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O4 -g -ggdb")
add_definitions(-D__STDC_CONSTANT_MACROS)

option(USE_JEMALLOC "Use jemalloc" OFF)

if(USE_JEMALLOC)
  find_package(jemalloc REQUIRED)
endif()

set(trees_SOURCES
  src/bss.cxx
  )

set(trees_LIBRARIES
  # -lprofiler
  )

if(USE_JEMALLOC)
  list(APPEND trees_LIBRARIES ${JEMALLOC_LIBRARIES})
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  )

add_executable(rbtree src/rbtree.cxx ${trees_SOURCES})
add_executable(avltree src/avltree.cxx ${trees_SOURCES})

target_link_libraries(rbtree ${trees_LIBRARIES})
target_link_libraries(avltree ${trees_LIBRARIES})

add_custom_target(rbtree.dot
                  COMMAND rbtree rbtree.dot
                  DEPENDS rbtree
                  WORKING_DIRECTORY ${trees_BINARY_DIR})
add_custom_target(avltree.dot
                  COMMAND avltree avltree.dot
                  DEPENDS avltree
                  WORKING_DIRECTORY ${trees_BINARY_DIR})

function(render_tree SRC DSC)
  execute_process(COMMAND dot "${SRC}"
                  COMMAND gvpr -c -f${trees_SOURCE_DIR}/extra/tree.gv
                  COMMAND neato -n -Tpng -o "${DST}"
                  WORKING_DIRECTORY ${trees_BINARY_DIR})
endfunction()
function(render_graph SRC DSC)
  execute_process(COMMAND dot "${SRC}"
                  COMMAND neato -n -Tpng -o "${DST}"
                  WORKING_DIRECTORY ${trees_BINARY_DIR})
endfunction()

# render_tree(rbtree.dot rbtree-as-tree.png)
# render_graph(rbtree.dot rbtree-as-graph.png)

# execute_process(COMMAND <cmd1> [args1...]]
#                                 [COMMAND <cmd2> [args2...] [...]]
#                                 [WORKING_DIRECTORY <directory>]
#                                 [TIMEOUT <seconds>]
#                                 [RESULT_VARIABLE <variable>]
#                                 [OUTPUT_VARIABLE <variable>]
#                                 [ERROR_VARIABLE <variable>]
#                                 [INPUT_FILE <file>]
#                                 [OUTPUT_FILE <file>]
#                                 [ERROR_FILE <file>]
#                                 [OUTPUT_QUIET]
#                                 [ERROR_QUIET]
#                                 [OUTPUT_STRIP_TRAILING_WHITESPACE]
#                                 [ERROR_STRIP_TRAILING_WHITESPACE])

# add_custom_command(OUTPUT rbtree-as-tree.png rbtree-as-graph.png
#                    COMMAND dot ARGS rbtree.dot | gvpr -c -ftree.gv | neato -n -Tpng -o
#                    MAIN_DEPENDENCY rbtree
#                    WORKING_DIRECTORY ${trees_BINARY_DIR})

# all: rbtree rbtree.png rbtree-graph.png \
#      avltree avltree.png avltree-graph.png

# rbtree: rbtree.cxx rbtree.hxx
#   $(CXX) -std=c++11 rbtree.cxx -o $@

# rbtree.dot: rbtree
#   ./rbtree 2>rbtree.dot

# rbtree.png: rbtree.dot tree.gv
#   dot rbtree.dot | gvpr -c -ftree.gv | neato -n -Tpng -o $@

# rbtree-graph.png: rbtree.dot tree.gv
#   dot rbtree.dot | neato -n -Tpng -o $@

# avltree: avltree.cxx avltree.hxx
#   $(CXX) -std=c++11 avltree.cxx -o $@

# avltree.dot: avltree
#   ./avltree 2>avltree.dot

# avltree.png: avltree.dot tree.gv
#   dot avltree.dot | gvpr -c -ftree.gv | neato -n -Tpng -o $@

# avltree-graph.png: avltree.dot tree.gv
#   dot avltree.dot | neato -n -Tpng -o $@

# main: main.cxx rbtree.hxx tree_traversing.hxx
#   $(CXX) -std=c++11 main.cxx -o $@

include("CMakeTests.txt")
