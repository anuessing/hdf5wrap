include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(hdf5wrap output.cc writers.cc)
target_link_libraries(hdf5wrap ${HDF5_LIBRARIES})

add_subdirectory(include)
install(TARGETS hdf5wrap DESTINATION lib)
