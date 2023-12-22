cmake_minimum_required(VERSION 3.13)
set(PROJECT_ID TSP_Implementation)

project(${PROJECT_ID})
set(CMAKE_CXX_STANDARD 17)


include(GoogleTest)


include_directories(
        include
)

set(SOURCE_FILES
        src/tsp_setup.cpp
        src/TSP-template.cpp
        )

add_executable(tsp ${SOURCE_FILES} main.cpp)

# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS
        test/tsp_unittest.cpp
        )

set(EXEC_TEST ${PROJECT_ID}__test)
add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

#set(GTEST_ROOT D:/CLion/mdig-agh-ccpp_clion-8da2d03dbbc9/googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )

add_subdirectory(${GTEST_ROOT} googletest-master)

target_link_libraries(${EXEC_TEST} gmock)