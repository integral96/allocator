cmake_minimum_required(VERSION 3.2)


#project(allocator_matrix)
project(allocator VERSION 0.0.0$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(${PROJECT_NAME} main.cpp)

add_library(lib${PROJECT_NAME} lib.cpp)
#add_library(MyMatrix${PROJECT_NAME} MyMatrix.cpp)

add_executable(test_version test_version.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(test_version PROPERTIES
     COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
     INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
     lib${PROJECT_NAME}
)

target_link_libraries(test_version
     ${Boost_LIBRARIES}
     lib${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT integral96@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(ip_filter_tests test_version)
