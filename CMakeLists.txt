set (CMAKE_CXX_STANDARD 23)

find_package (Seastar REQUIRED)

add_executable (my_app
  my_app.cc)

add_executable (my_app2 
  my_app2.cc)

target_link_libraries (my_app
  Seastar::seastar)

target_link_libraries (my_app2
  Seastar::seastar)

