file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/commons/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/PKB/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/QPS/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/SP/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/commons/*.h" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/commons/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/PKB/*.h" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/PKB/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/QPS/*.h" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/QPS/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/SP/*.h" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/SP/*.hpp")
add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)

