include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(ImplicitMessaging4AZD-KEP ImplicitMessaging4AZD-KEP.cpp include/AzdKepController.cpp)
target_include_directories(ImplicitMessaging4AZD-KEP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ImplicitMessaging4AZD-KEP EIPScanner)