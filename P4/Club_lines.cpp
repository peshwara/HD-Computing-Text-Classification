

	// reading a text file
#include <iostream>
#include <fstream>
#include <string>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>        // std::time  // Needed for the true randomization
#include <cstdlib>      // std::rand, std::srand
#include <unordered_map>
#include <iterator>
	using namespace std;
	void add_to_mem(string category, string input);

	unordered_map<string, string> MEMORY;



	int main() {


		string testing_data_file = "D:\\Users\\Hariharan\\Desktop\\testfiles\\52\\noshort.txt";
		vector<string> vecstr;
		string line;
		ifstream myfile(testing_data_file);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				//cout << line << "xx\n";
				vecstr.push_back(line);
			}
			myfile.close();
		}

		else cout << "Unable to open file";
		
		std::vector<string> path = vecstr;
		std::vector<string>::const_iterator i;

		for (i = path.begin(); i != path.end(); ++i) {
			string str = (string)*i;

			string strWords[2];
			short counter = 0;
			for (short i = 0; i < str.length(); i++) {
				if (str[i] == '\t')
					counter++;
				else
					strWords[counter] += str[i];
			}

			add_to_mem(strWords[0], strWords[1]);


		}
		ofstream outfile;
		outfile.open("D:\\Users\\Hariharan\\Desktop\\testfiles\\52\\noshort_52.txt");
		//outfile << "Final output is: \n\n";
		cout << "Final output is: \n\n";
		typedef std::unordered_map< std::string, std::string > type_t;
		typedef type_t::iterator type_t_iter;

		for (type_t_iter i = MEMORY.begin(); i != MEMORY.end(); ++i) {

			//cout << i->first << "\t" << i->second << "\n";
			outfile << i->first << "\t" << i->second << "\n";

		}
		cout << "file written";
		
		getchar();
		return 0;
	}



	void add_to_mem(string category, string input) {

		if (MEMORY.find(category) == MEMORY.end()) {
			// Meaning that you have not found the data . or if this is the first occurence.
			MEMORY.insert(std::pair<string, string>(category, input));
		}
		else {
			//Meaning that you have found the data andd we need to append the data to the existing data.
			string temp_assoc = MEMORY.find(category)->second;
			//std::transform(input.begin(), input.end(), temp_assoc.begin(), temp_assoc.begin(), std::plus<int>());
			//cout << newvalue << ".......\n";
			MEMORY.erase(category);
			MEMORY.insert(std::pair<string, string>(category, temp_assoc + " " + input));
		}

	}