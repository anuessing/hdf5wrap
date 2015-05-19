#ifndef HDF5WRAP_WRITERS_HH
#define HDF5WRAP_WRITERS_HH

#include <array>
#include <vector>
#include <H5Cpp.h>

namespace hdf5wrap
{
  template <class T>
  struct AttributeTraits;

  template <>
  struct AttributeTraits<int> {
    typedef H5::IntType Type;
    static H5::PredType predType;
    static Type constructType(const int& v) {
      return Type(predType);
    }
    static const int * getHandle(const int& v) {
      return &v;
    }
  };

  template <>
  struct AttributeTraits<std::string> {
    typedef H5::StrType Type;
    static H5::PredType predType;
    static Type constructType(const std::string& v) {
      return Type(predType,v.size());
    }
    static const std::string& getHandle(const std::string& v) {
      return v;
    }
  };

  template <>
  struct AttributeTraits<double> {
    typedef H5::FloatType Type;
    static H5::PredType predType;
    static Type constructType(const double& v) {
      return Type(predType);
    }
    static const double * getHandle(const double& v) {
      return &v;
    }
  };

  template <class T>
  struct AttributeWriter {
    typedef AttributeTraits<T> Traits;
    static void write(H5::H5Object& dataset, const std::string& attributeName, const T& v)
    {
      H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
      typename Traits::Type datatype(Traits::constructType(v));
      H5::Attribute attribute = dataset.createAttribute(attributeName, datatype, dataspace);
      attribute.write(datatype, Traits::getHandle(v));
    }
  };

  template <>
  struct AttributeWriter<bool> {
    static void write(H5::H5Object& dataset, const std::string& attributeName, const bool& v)
    {
      int vi = v;
      AttributeWriter<int>::write(dataset, attributeName, vi);
    }
  };

  template <class T>
  struct DataSetWriter;

  template <class T, std::size_t N>
  struct DataSetWriter<std::array<T, N> >
  {
    typedef AttributeTraits<T> Traits;

    static H5::DataSet write(H5::CommonFG& parent, const std::string& name, const std::array<T, N>& value)
    {
      hsize_t dims[] = { N };
      H5::DataSpace dataspace(1, dims);
      typename Traits::Type datatype(Traits::constructType(value[0]));
      H5::DataSet dataSet = parent.createDataSet(name, datatype, dataspace);
      dataSet.write(&value[0], Traits::predType);
      return dataSet;
    }
  };

  template <class T>
  struct DataSetWriter<std::vector<T> >
  {
    typedef AttributeTraits<T> Traits;

    static H5::DataSet write(H5::CommonFG& parent, const std::string& name, const std::vector<T>& value)
    {
      hsize_t dims[] = { value.size() };
      H5::DataSpace dataspace(1, dims);
      typename Traits::Type datatype(Traits::constructType(value[0]));
      H5::DataSet dataSet = parent.createDataSet(name, datatype, dataspace);
      dataSet.write(&value[0], Traits::predType);
      return dataSet;
    }
  };
}

#endif // HDF5WRAP_WRITERS_HH
