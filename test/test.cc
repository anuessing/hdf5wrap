#include <iostream>
#include <hdf5wrap/output.hh>

int main() {
  hdf5wrap::File file("test.h5");
  file.getGroup("group").insertAttribute<std::string>("foo", "bla");
  file.getGroup("group").getGroup("deep").insertAttribute("bar", 5.0);
  std::array<double, 5> data{{1.0,2.0,3.0,4.0,5.0}};
  file.getGroup("group").createDataset("data", data);
  std::vector<int> data2{{42,12,17,1,-12,3,1}};
  file.getGroup("group").getGroup("deep").createDataset("data", data2);
  file.getGroup("group").getDataset("data").insertAttribute("foo", 2);
  file.write();
}
