#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <list>
#include <fstream>
#include <Windows.h>
using namespace std;

#include "pugixml-1.5/src/pugixml.hpp"
#include "pugixml-1.5/src/pugiconfig.hpp"
#include "pugixml-1.5/src/pugixml.cpp"
using namespace pugi;

string OutputFolder = "BaseDirectory";
string TestSet = OutputFolder + "\\Test Set Directories";
string Bin = OutputFolder + "\\bin";
string Source = OutputFolder + "\\src";

// Create test directories
bool CreateMainDirectories() {	
	if (CreateDirectory(OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		if (CreateDirectory(TestSet.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
			cout << "Successfully create directory: " << TestSet << endl;
		} else {
			cout << "Failed to create directory: " << TestSet << endl;
			return false;
		}

		if (CreateDirectory(Bin.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
			cout << "Successfully create directory: " << Bin << endl;
		} else {
			cout << "Failed to create directory: " << Bin << endl;
			return false;
		}

		if (CreateDirectory(Source.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
			cout << "Successfully create directory: " << Source << endl;
		} else {
			cout << "Failed to create directory: " << Source << endl;
			return false;
		}
	} else {
		cout << "Failed to create directory: " << OutputFolder << endl;
		return false;
	}
	return true;

}

// Create test case directories
bool CreateTestCaseDir(string testname) {
	if (CreateDirectory((TestSet + "\\" + testname).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		cout << "Successfully create directory: " << testname << endl;
	} else {
		cout << "Failed to create directory: " << testname << endl;
		return false;
	}
	return true;
}

// Check for file accessibility
bool AccessFile(const std::string& fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}//end if

// Parsing xml file
int getNodeInfo(xml_node root, bool alltest, string testsetid,
	string email, list<string> filename) {
	bool good = false;
	int dir = 1;
	string attribute1, attribute2, attribute3, attribute4;
	string value1, value2, value3, value4;
	for (pugi::xml_node_iterator node1 = root.begin(); node1 != root.end(); ++node1) {
		attribute1 = node1->name();
		value1 = node1->child_value();
		if (attribute1.compare("report-to") == 0) {
			if (email.compare(value1) == 0)
				cout << "Email match: " << value1 << endl;
			else cout << "Email doesn't match!" << endl;
		} else if (attribute1.compare("test-case") == 0) {
			pugi::xml_node test_case_node = root.child("test-case");
			for (pugi::xml_node_iterator node2 = test_case_node.begin(); node2 != test_case_node.end(); ++node2) {
				attribute2 = node2->name();
				value2 = node2->child_value();
				if (attribute2.compare("name") == 0) {
					if (testsetid.compare(value2) == 0) {
						cout << "Test Set Id match: " << value2 << endl;
						dir = CreateTestCaseDir(value2);
						if (dir == false) return EXIT_FAILURE;
					} else cout << "Test Set Id doesn't match!" << endl;
				} else if (attribute2.compare("input-file") == 0) {
					pugi::xml_node input_file_node = test_case_node.child("input-file");
					for (pugi::xml_node_iterator node3 = input_file_node.begin(); node3 != input_file_node.end(); ++node3) {
						attribute3 = node3->name();
						value3 = node3->child_value();
						if (attribute3.compare("name-for-input") == 0) {
							bool inputfile = AccessFile(value3);
							if (inputfile == true) {
								cout << "Input File: " << value3 << " is accessible!" << endl;
							}
							else cout << "Input File: " << value3 << " is not accessible!" << endl;
						} else if (attribute3.compare("copy-from") == 0) {
							bool incopyfrom = AccessFile(value3);
							if (incopyfrom == true) {
								cout << "Copy From Input File: " << value3 << " is accessible!" << endl;
							}
							else cout << "Copy From Input File: " << value3 << " is not accessible!" << endl;
						}//end if-else
					}//end for
				} else if (attribute2.compare("output-file") == 0) {
					pugi::xml_node output_file_node = test_case_node.child("output-file");
					for (pugi::xml_node_iterator node4 = output_file_node.begin(); node4 != output_file_node.end(); ++node4) {
						attribute4 = node4->name();
						value4 = node4->child_value();
						if (attribute4.compare("name-for-output") == 0) {
							bool outputfile = AccessFile(value4);
							if (outputfile == true) {
								cout << "Output File: " << value4 << " is accessible!" << endl;
							}
							else cout << "Output File: " << value4 << " is not accessible!" << endl;
						} else if (attribute4.compare("compare-to") == 0) {
							bool outcopyfrom = AccessFile(value4);
							if (outcopyfrom == true) {
								cout << "Compare To Output File: " << value4 << " is accessible!" << endl;
							} else cout << "Compare To Output File: " << value4 << " is not accessible!" << endl;
						}//end if-else
					}//end for
				}//end if-else
			}//end for
		}//end if-else
	}//end for
}//end method





int main(int argCount, char *argValues[]) {
	list<string> filename;
	list<string> compilerflags;
	string programid;
	string testsetid;
	string email;
	char *file;
	bool alltest;
	bool foldercreated;

	// Check if the command typed in has enough components
	if (argCount < 2) {
		cerr << "Usage: " << argValues[0] << " [-p #] [-t #] [-e #] [- #] filename" << endl;
		cerr << "Exiting" << endl;
		return EXIT_FAILURE;
	}//end if

	// Parsing command line for information
	string currentArgument;
	for (int i = 1; i < argCount; i++) {
		currentArgument = argValues[i];
		if (std::string(argValues[i]).find("-p") != string::npos) {
			// handle -p argument
			programid = argValues[++i];
			cout << "Find a -p argument: " << programid << endl;
		}
		else if (currentArgument.find("-t") != string::npos) {
			// handle -t argument
			testsetid = argValues[++i];
			cout << "Find a -t argument: " << testsetid << endl;
		}
		else if (currentArgument.find("-e") != string::npos) {
			// handle -e argument
			email = argValues[++i];
			cout << "Find a -e argument: " << email << endl;
		}
		else if (currentArgument.find("-") != string::npos) {
			// handle compiler flags argument
			compilerflags.push_back(argValues[i]);
			cout << "Find compiler flags argument: " << argValues[i] << endl;
		}
		else if (currentArgument.find(" ") == string::npos) {
			// handle source file(s) argument
			filename.push_back(currentArgument);
			cout << "Find a filename: " << currentArgument << endl;
		}//end if-else
	}//end for

	// Search for programid
	/*    bool prog = AccessFile(programid);
	if (prog == true) {
	cout << "Find " << programid << endl;
	file = (char*)programid.c_str();
	} else {
	cout << "ProgramID " << programid << " is not accessible, can not run the test!" << endl;
	return EXIT_FAILURE;
	}//end if-else
	*/

	// Search for source files
	list<string>::iterator i;
	for (i = filename.begin(); i != filename.end(); ++i) {
		bool sourcefiles = AccessFile(*i);
		if (sourcefiles == true) {
			cout << "Find " << *i << endl;
		}
		else {
			cout << "Source file " << *i << " is not accessible!" << endl;
		}//end if-else
	}//end for

	// Parsing XML file to compare with command line tags
	file = (char*)programid.c_str();
	pugi::xml_document doc;
	xml_parse_result res;
	if (!(res = doc.load_file(file)))
	{
		cout << "Error loading file XML: " << res.description() << endl;
		system("PAUSE");
		return -1;
	}//end if

	foldercreated = CreateMainDirectories();
	if (foldercreated == false) return EXIT_FAILURE;

	pugi::xml_node root = doc.child("test-set");
	// Send the file
	if (testsetid.empty()) alltest = false;
	else alltest = true;
	getNodeInfo(root, alltest, testsetid, email, filename);
	cin.get();
	return EXIT_SUCCESS;
}//end main


