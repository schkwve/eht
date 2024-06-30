project(eht_tests)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE libs::eht)

add_test(eht_tests ${PROJECT_NAME})
