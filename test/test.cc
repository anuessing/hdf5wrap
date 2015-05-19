#include <hdf5wrap/output.hh>

int main() {
  // create the output file
  hdf5wrap::File file("test.h5");
  // insert a string attribute into a group "group". Note that the
  // the template parameter has to be applied explicitly
  file.getGroup("group").insertAttribute<std::string>("foo", "bla");
  // insert another attribute into a nested group
  file.getGroup("group").getGroup("deep").insertAttribute("bar", 5.0);
  // create an array dataset
  std::array<double, 5> data{{1.0,2.0,3.0,4.0,5.0}};
  file.getGroup("group").createDataset("data", data);
  // insert an attribute to the array dataset
  file.getGroup("group").getDataset("data").insertAttribute("foo", 2);
  // create a vector dataset
  std::vector<int> data2{{42,12,17,1,-12,3,1}};
  file.getGroup("group").getGroup("deep").createDataset("data", data2);
  // write the hdf file
  file.write();
}
