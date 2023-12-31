cmake_minimum_required(VERSION 3.13)
set(PROJECT_ID net_simulation)

project(${PROJECT_ID})

set(CMAKE_CXX_STANDARD 17)


include(GoogleTest)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Werror=switch)


include_directories(include)
include_directories(mocks)


set(SOURCE_FILES
        src/storage_types.cpp
        src/package.cpp

        src/helpers.cpp
        src/nodes.cpp

        src/factory.cpp

        src/reports.cpp

        src/simulation.cpp
    )
add_executable(netsim ${SOURCE_FILES} main.cpp)

# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS
        test/netsim_unitest.cpp

        test/test_package.cpp
        test/test_storage_types.cpp

        test/test_nodes.cpp

        test/test_Factory.cpp
        test/test_factory_io.cpp

        test/test_reports.cpp

        test/test_simulate.cpp
        )

set(EXEC_TEST ${PROJECT_ID}__test)
add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

#set(GTEST_ROOT C:/Users/Goon/Desktop/Studia_zdalne/ZPO/googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )

add_subdirectory(${GTEST_ROOT} googletest-master)

target_link_libraries(${EXEC_TEST} gmock)

set(target_name netsim)

target_compile_definitions(${target_name} PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)
target_compile_definitions(${EXEC_TEST} PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)