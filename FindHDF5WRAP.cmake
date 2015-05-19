# cmake module which checks if hdf5wrap is available
#
# this will define
# HDF5WRAP_FOUND: whether hdf5wrap has been found
# HDF5WRAP_INCLUDE_DIRS: the hdf5wrap include directories
# HDF5WRAP_LIBRARIES: the hdf5wrap libraries
#
find_package(PkgConfig)
pkg_check_modules(PC_HDF5WRAP QUIET libhdf5wrap)

find_path(HDF5WRAP_INCLUDE_DIR hdf5wrap/output.hh HINTS ${PC_HDF5WRAP_INCLUDEDIR} ${PC_HDF5WRAP_INCLUDE_DIRS} ${HDF5WRAP_PREFIX} PATH_SUFFIXES include)
find_library(HDF5WRAP_LIBRARY hdf5wrap HINTS ${PC_HDF5WRAP_LIBDIR} ${PC_HDF5WRAP_PREFIX} PATH_SUFFIXES lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args("hdf5wrap" DEFAULT_MSG HDF5WRAP_LIBRARY HDF5WRAP_INCLUDE_DIR)

if (HDF5WRAP_FOUND)
  set(HDF5WRAP_LIBRARIES ${HDF5WRAP_LIBRARY})
  set(HDF5WRAP_INCLUDE_DIRS ${HDF5WRAP_INCLUDE_DIR})
  file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
    "Found hdf5wrap library:\n"
    "Include directory: ${HDF5WRAP_INCLUDE_DIRS}\n"
    "Library directory: ${HDF5WRAP_LIBRARIES}\n\n")
else(HDF5WRAP_FOUND)
  file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Could not find hdf5wrap library:\n"
    "Include directory: ${HDF5WRAP_INCLUDE_DIR}\n"
    "Library directory: ${HDF5WRAP_LIBRARY}\n\n")
endif(HDF5WRAP_FOUND)

mark_as_advanced(HDF5WRAP_INCLUDE_DIR HDF5WRAP_LIBRARY)
