#ifndef HDF5WRAP_OUTPUT_HH
#define HDF5WRAP_OUTPUT_HH

#include <typeinfo>
#include <map>
#include <memory>
#include <H5Cpp.h>
#include <hdf5wrap/writers.hh>

namespace hdf5wrap
{
  class AttributeBase
  {
  public:
    virtual void write(H5::H5Object& parent, const std::string& name) = 0;
    virtual ~AttributeBase() {}
  };

  template <class T>
  class TypeAttribute : public AttributeBase
  {
  public:
    void write(H5::H5Object& parent, const std::string& name)
    {
      AttributeWriter<std::string>::write(parent, name, typeid(T).name());
    }
  };

  template <class T>
  class ValueAttribute : public AttributeBase
  {
  public:
    explicit ValueAttribute(const T& value)
        : value_(value)
    {
    }

    void write(H5::H5Object& parent, const std::string& name)
    {
      AttributeWriter<T>::write(parent, name, value_);
    }

  private:
    T value_;
  };

  class Attributable
  {
  public:
    template <class T>
    void insertAttribute(const std::string& name, const T& value)
    {
      attributes_[name] = std::unique_ptr<AttributeBase>(new ValueAttribute<T>(value));
    }

    template <class T>
    void insertAttribute(const std::string& name)
    {
      attributes_[name] = std::unique_ptr<AttributeBase>(new TypeAttribute<T>());
    }

    void write(H5::H5Object& parent)
    {
      for (const auto& a : attributes_) {
        a.second->write(parent, a.first);
      }
    }

  private:
    std::map<std::string, std::unique_ptr<AttributeBase> > attributes_;
  };

  class DatasetBase : public Attributable
  {
  public:
    virtual void write(H5::CommonFG& parent, const std::string& name) = 0;
    virtual ~DatasetBase() {}
  };

  template <class T>
  class Dataset : public DatasetBase
  {
  public:
    explicit Dataset(const T& value)
        : value_(value)
    {
    }

    void write(H5::CommonFG& parent, const std::string& name)
    {
      H5::DataSet dataset = DataSetWriter<T>::write(parent, name, value_);
      Attributable::write(dataset);
    }

  private:
    T value_;
  };

  class Group;

  //! \brief a class which contains sets of groups and datasets
  class DataContainer
  {
  public:
    Group& getGroup(const std::string& name);
    DatasetBase& getDataset(const std::string& name);
    void write(H5::CommonFG& parent);

    template <class T>
    void createDataset(const std::string& name, const T& value)
    {
      datasets_[name] = std::shared_ptr<DatasetBase>(new Dataset<T>(value));
    }

  private:
    std::map<std::string, std::shared_ptr<Group> > groups_;
    std::map<std::string, std::shared_ptr<DatasetBase> > datasets_;
  };

  //! \brief a group in a hdf5 file
  class Group : public DataContainer, public Attributable
  {
  public:
    void write(H5::CommonFG& parent, const std::string& name);

  private:
  };

  /**
   * a hdf5 file consists mainly of datasets and groups. both of them can have attributes.
   * a dataset is a multidimensional array, a group is a set of datasets and groups.
   */
  class File : public DataContainer
  {
  public:
    explicit File(const std::string& filename)
        : filename_(filename)
    {
    }

    void write();

  private:
    std::string filename_;
  };
}

#endif // HDF5WRAP_OUTPUT_HH
