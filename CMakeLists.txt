cmake_minimum_required(VERSION 3.12...3.31)
project(my_database CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)
  # Tries TRY_DIR first, falls back to downloading userver from GitHub using CPM.
  download_userver(TRY_DIR third_party/userver)
endif()

# Include directories
include_directories(src)

userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/checker/abstract_checker.hpp
    src/checker/python_checker.hpp
    src/checker/python_checker.cpp

    src/user_component/submit_solution.hpp
    src/user_component/submit_solution.cpp

    src/user_component/test_decoder.hpp
    src/user_component/test_decoder.cpp

    src/admin_component/load_task.hpp
    src/admin_component/load_task.cpp

    src/checker_components/submission_feedback.hpp
    src/checker_components/submission_feedback.cpp

    src/checker_components/problem.hpp
    src/checker_components/execution_status.hpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/checker_test/python_checker_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)

# Functional Tests
userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

# Copy bash checkers
configure_file(src/checker/bash_checker_components/python.bash ${CMAKE_BINARY_DIR}/python.bash COPYONLY)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
