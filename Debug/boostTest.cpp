#include <vector>
/*
 * Step 1:  The boost include files....
 */
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

int main (int argCount, char *argValues[]) {

	vector<int> myvec;
	ifstream infile("test.xml");
	/*
         * Quick demo of the BOOST FOREACH
	 */
	for (int i = 1; i < 100; i++) {
		myvec.push_back(i);
	}

	BOOST_FOREACH(int x, myvec) {
	  cout << x << endl;
	}

	/*
         * Step 2: Open the XML file and parse it...
	 */
	using boost::property_tree::ptree;
	ptree propTree;
	if (infile.good()) {  // don't need to open the file -- boost will
	  read_xml("test.xml", propTree, boost::property_tree::xml_parser::trim_whitespace);
	}
        /*
         * Step 3: get the test set name and test case name
         *         from the property tree.
	 */
	std::string testSetName = propTree.get<std::string>("test-set", "No test set name");

	std::string testCaseName = propTree.get<std::string>("test-set.test-case.name", "No test case name");
	cout << "Test set name = " << testSetName << endl 
	     << "Test Case name = " << testCaseName << endl;

	/*
         * get all the nodes below a tag
         */
	
	BOOST_FOREACH(ptree::value_type &v, propTree.get_child("test-set")) {
	  cout << "\""<< v.first.data() << "\"\t\"" << v.second.data() << "\""<< endl;
	}

	vector<string> reportToAddresses;
	BOOST_FOREACH(ptree::value_type& value, propTree.get_child("test-set.report-to")) {
	  cout << "report to address : "<< "\"" << value.second.data() << "\"" << endl;
	  reportToAddresses.push_back(value.second.data());
	}
	return EXIT_SUCCESS;
}