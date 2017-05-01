add_executable(app main.cpp)
target_link_libraries(app landfill)
set_target_properties(app PROPERTIES LINK_FLAGS "-Wl,--enable-new-dtags")
