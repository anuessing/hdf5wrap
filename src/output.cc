#include <hdf5wrap/output.hh>
#include <cassert>

namespace hdf5wrap
{
  Group& DataContainer::getGroup(const std::string& name)
  {
    typedef std::map<std::string, std::shared_ptr<Group> >::iterator It;
    std::pair<It, bool> p = groups_.insert(std::make_pair(name, std::make_shared<Group>()));
    return *(p.first->second);
  }

  DatasetBase& DataContainer::getDataset(const std::string& name)
  {
    typedef std::map<std::string, std::shared_ptr<DatasetBase> >::iterator It;
    It it = datasets_.find(name);
    assert(it != datasets_.end());
    return *(it->second);
  }

  void Group::write(H5::CommonFG& parent, const std::string& name)
  {
    H5::Group group = parent.createGroup(name);
    Attributable::write(group);
    DataContainer::write(group);
  }

  void DataContainer::write(H5::CommonFG& parent)
  {
    for (const auto& g : groups_) {
      g.second->write(parent, g.first);
    }
    for (const auto& d : datasets_) {
      d.second->write(parent, d.first);
    }
  }

  void File::write()
  {
    H5::H5File file(filename_, H5F_ACC_TRUNC);
    DataContainer::write(file);
  };
}
