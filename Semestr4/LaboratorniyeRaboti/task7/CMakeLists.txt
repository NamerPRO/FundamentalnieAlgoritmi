cmake_minimum_required(VERSION 3.0.0)
project(task7 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(
    task7
    main.cpp
    nbigint/bigint_multiplication.h
    nbigint/bigint.h
    nbigint/karacuba.h
    nbigint/karacuba.cpp
    nbigint/standard_bigint.cpp
    nbigint/standard_bigint.h
    nlogger/basics.cpp
    nlogger/basics.h
    nlogger/cpp_time.h
    nlogger/cpp_time.cpp
    nlogger/logger_builder.cpp
    nlogger/logger_builder.h
    nlogger/logger_helper.h
    nlogger/logger_helper.cpp
    nlogger/logger.cpp
    nlogger/logger.h
    nlogger/standard_logger_builder.cpp
    nlogger/standard_logger_builder.h
    nlogger/standard_logger.cpp
    nlogger/standard_logger.h
    nlogger/nlohmann/json.hpp
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
