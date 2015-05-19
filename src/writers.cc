#include <hdf5wrap/writers.hh>

namespace hdf5wrap
{
  H5::PredType AttributeTraits<int>::predType = H5::PredType::NATIVE_INT;
  H5::PredType AttributeTraits<double>::predType = H5::PredType::NATIVE_DOUBLE;
  H5::PredType AttributeTraits<std::string>::predType = H5::PredType::C_S1;
}
