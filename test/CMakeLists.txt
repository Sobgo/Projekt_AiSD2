include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.3
        SYSTEM
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Add any libraries required by tests here
target_link_libraries(tests PRIVATE convex_hull)
target_link_libraries(tests PRIVATE bipartite_maximum_matching)
target_link_libraries(tests PRIVATE pattern_matching)
target_link_libraries(tests PRIVATE sssp_plane)
target_link_libraries(tests PRIVATE dynamic_optimal_path)
target_link_libraries(tests PRIVATE data_compression)

include(CTest)
include(Catch)
catch_discover_tests(tests)