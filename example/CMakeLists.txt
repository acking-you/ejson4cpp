aux_source_directory(. EX_SRC)

add_definitions(-DCONFIG_PATH="${PROJECT_SOURCE_DIR}/example/config.json" -DDATA_PATH="${PROJECT_SOURCE_DIR}/example/data.json")

foreach (s ${EX_SRC})
    get_filename_component(name ${s} NAME_WLE)
    add_executable(${name} ${s})
    target_link_libraries(${name} ejson)
endforeach ()