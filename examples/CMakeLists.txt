configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource/input_recipe.txt
               ${CMAKE_CURRENT_BINARY_DIR}/resource/input_recipe.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource/output_recipe.txt
               ${CMAKE_CURRENT_BINARY_DIR}/resource/output_recipe.txt COPYONLY)

file(GLOB ELITE_C_EXAMPLE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

foreach(example_source IN LISTS ELITE_C_EXAMPLE_SOURCES)
    get_filename_component(example_target ${example_source} NAME_WE)

    add_executable(${example_target} ${example_source})
    target_link_libraries(${example_target} PRIVATE ${ELITE_C_WRAPPER_TARGET})
    target_include_directories(${example_target} PRIVATE ${PROJECT_SOURCE_DIR}/include)

    if(example_target STREQUAL "servoj_plan_example" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_link_libraries(${example_target} PRIVATE m)
    endif()
endforeach()
