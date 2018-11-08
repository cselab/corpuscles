project(main)
cmake_minimum_required(VERSION 2.8.11)

find_package( CGAL QUIET COMPONENTS  )
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

include(${CGAL_USE_FILE} )
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()
endif()

add_executable( main  main.cpp )
add_to_cached_list(CGAL_EXECUTABLE_TARGETS main)
target_link_libraries(main   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
