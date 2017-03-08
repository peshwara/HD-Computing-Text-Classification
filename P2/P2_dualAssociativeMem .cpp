
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

#define PRINT_ITEM_MEMORY       1;                      //Prints item memory if 1
#define GENERATE_TRAINING_DATA  0;                      //Creates Associative memory if 1
const int       DIMEN = 10000;                     //Vector dimension
const int       NGRAM = 7;                        //Ngram-characters read from the training and test files

vector<string> ReadFileByBlocks(const char* filename);

vector<string> ReadFile(const char* filename);
vector<int> splitText_continuous(int n, string data);
vector<int> splitText_space(string data);
vector<int> circ_rotate(vector<int> vect, int ror);
void generateItemMemory();
vector<int> MAP(vector<vector<int> > tot_data);
int myrandom(int i);
vector<int> code_words(string pch);
vector<int> result_words(vector<int> word, vector<int> result);
void add_to_assoc_mem(string category, vector<int> input);
float testing_data(string file_name);
string final_comparison(vector<int> test);
double norm(int *vec, int n);
double cosAngle(int *u, int *v, int n);

unordered_map<char, vector<int>>    ITEM_MEMORY;        //map with alphabets as key and vector of size d as value
unordered_map<string, vector<int>>    ASSOCIATIVE_MEMORY;


int main() {
	generateItemMemory();
	DIR *dir;
	struct dirent *ent;
	char folder_name[] = "D:\\Users\\Hariharan\\Desktop\\testfiles\\P2\\training";
	string testing_data_file = "D:\\Users\\Hariharan\\Desktop\\testfiles\\P2\\testing.txt";
	//generateItemMemory();
	std::vector<string> path;
	if ((dir = opendir(folder_name)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			string str = string(ent->d_name);
			if (str.size() > 4) {
				if (str.substr(str.size() - 4, 100).compare(".txt") == 0) {
					std::string full_folder = folder_name + std::string("\\") + str;
					//cout<<folder_name<<"\\"<<str<<"\n";
					cout << full_folder << "\n";
					//	path = ReadFileByBlocks(full_folder.c_str());
					//cout<<path;
					path = ReadFile(full_folder.c_str());
					std::vector<string>::const_iterator i;

					int news_counter = 0;
					for (i = path.begin(); i != path.end(); ++i) {
						//	std::cout << *i << "\n-----";
						string str = (string)*i;

						string strWords[2];
						short counter = 0;
						for (short i = 0; i < str.length(); i++) {
							if (str[i] == '\t')
								counter++;
							else
								strWords[counter] += str[i];
						}

						//cout << "Key is--" << strWords[0] << "\n";
						//cout << "Value --" << strWords[1] << "\n\n";
						//splitText_continuous(3,strWords[1]);
						//vector<int> to_assoc_mem = splitText_space(strWords[1]);
						vector<int> to_assoc_mem = splitText_continuous(NGRAM, strWords[1]);
						//cout << "awesome";
						
							add_to_assoc_mem(strWords[0], to_assoc_mem);
						
						cout << "\n" << ++news_counter << " lines added\n";
					}
					cout << "FILE IS READ...\n";
					//Now, we need to read the testing data and compare with the training data.
					float percentage = testing_data(testing_data_file);
					//Now, display the percentage of success
					cout << "Success percentage is: " << percentage;
				}
			}

			//  printf ("%s%s\n", folder_name,ent->d_name);
			// printf("next.....");
		}
		closedir(dir);

		/*
		//testing the circ buffer======================================================
		vector<int> vect;
		for (int i = 1; i < 10; ++i) vect.push_back(i);
		vector<int> result = circ_rotate(vect, 4);
		//std::rotate(vect.begin(),vect.begin()+3,vect.end());
		for (std::vector<int>::iterator it = result.begin(); it != result.end(); ++it)
		std::cout << ' ' << *it;
		//==============================================================================
		*/

	}
	else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
	getchar();
}

vector<string> ReadFileByBlocks(const char* filename) {
	vector<string> vecstr;

	ifstream fin(filename, ios_base::in);
	if (fin.is_open()) {
		char* buffer = new char[10000];
		while (fin.read(buffer, 10000)) {
			string s(buffer);
			vecstr.push_back(s);
		}

		// if the bytes of the block are less than 1024,
		// use fin.gcount() calculate the number, put the va
		// into var s
		//--string s(buffer, fin.gcount());
		//--vecstr.push_back(s);

		delete[] buffer;
		fin.close();
	}
	else {
		cerr << "Cannot open file:" << filename << endl;
	}

	return vecstr;

}

vector<string> ReadFile(const char* filename) {
	ifstream inFile(filename);
	vector<string> vecstr;
	if (!inFile) {
		cerr << "File -- " << filename << " --not found." << endl;
		return vecstr;
	}


	// Using getline() to read one line at a time.
	string line;
	while (getline(inFile, line)) {
		if (line.empty()) continue;

		// Using istringstream to read the line into integers.
		/*	istringstream iss(line);
		int sum = 0, next = 0;
		while (iss >> next) sum += next;
		outFile << sum << endl;
		*/

		vecstr.push_back(line);
	}

	inFile.close();
	//	outFile.close();
	return vecstr;
}

vector<int> splitText_continuous(int n, string data) {
	string pch;
	vector<int> result_space;
	vector<int> assoc_mem;
	for (unsigned int i = 0; i < data.length() - n + 1; ++i) {
		pch = data.substr(i, n);
		result_space = code_words(pch);
		assoc_mem = result_words(result_space, assoc_mem);
	}
	return assoc_mem;
}

vector<int> splitText_space(string str) {
	//const char *str = data.c_str();
	vector<int> result_space;
	vector<int> assoc_mem;
	char * writable = new char[str.size() + 1];
	std::copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	char * pch;
	pch = strtok(writable, " ");
	while (pch != NULL) {
		//cout << pch;
		//cout << "----\n";
		//Write the code for taking the vector MAP output and add it to the result 
		result_space = code_words(pch);
		//cout << "Res:\t";
		//for (std::vector<int>::iterator it = result_space.begin(); it != result_space.end(); ++it)
		//	std::cout << ' ' << *it;
		//cout << "\n";
		pch = strtok(NULL, " ");
		assoc_mem = result_words(result_space, assoc_mem);
		//	cout << "Asso_Mem:\t";
		//for (std::vector<int>::iterator it = assoc_mem.begin(); it != assoc_mem.end(); ++it)
		//std::cout << ' ' << *it;
		//cout << "\n";
	}

	//cout << "\nFinal result: \n";
	//for (std::vector<int>::iterator it = assoc_mem.begin(); it != assoc_mem.end(); ++it)
	//	std::cout << ' ' << *it;
	//cout << "\n";
	return assoc_mem;
	// now, I need to addd to the associative memory ....


	/*	for(unsigned int i = 0; i < data.length()-n+1; ++i) {
	cout<<data.substr(i,n);
	cout<<"----";
	}*/
}

vector<int> code_words(string pch) {
	vector<int> char_data;
	vector<int> result;
	vector<vector<int>> word_data;
	for (unsigned int i = 0; i < pch.length(); i++) {
		char c = pch[i]; //this is your character
						 //cout << c << "\t";
						 //for (std::vector<int>::iterator it = ITEM_MEMORY.find(c)->second.begin(); it != ITEM_MEMORY.find(c)->second.end(); ++it)
						 //std::cout << ' ' << *it;
						 //char_data.push_back( ITEM_MEMORY.find(c)->second);
						 //cout << ;

		char_data = ITEM_MEMORY.at(tolower(c));

		//	for (std::vector<int>::iterator it = char_data.begin(); it != char_data.end(); ++it)
		//		std::cout << *it << " ";
		//	cout << "\n";
		word_data.push_back(char_data);
		//		char_data.clear;
	}
	result = MAP(word_data);
	return result;

}

vector<int> circ_rotate(vector<int> vect, int ror) {
	vector<int> vect1 = vect;
	std::rotate(vect1.begin(), vect1.begin() + ror, vect1.end());
	return vect1;
}
int myrandom(int i) {
	return  rand() % i;
}


void generateItemMemory() {
	srand(unsigned(time(0)));                           //This will ensure randomized number by help of time.
	vector<int> dvector;
	char alph[27] = { 'a', 'b', 'c', 'd', 'e',
		'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
		'v', 'w', 'x', 'y', 'z', ' '
	};

	for (int i = 0; i < DIMEN / 2; i++) {
		dvector.push_back(1);                           //Fills half of the vector with 1s
	}

	for (int i = DIMEN / 2; i < DIMEN; i++) {
		dvector.push_back(-1);                          //Fills other hald of the vector with -1s
	}

	for (int j = 0; j < 27; j++) {
		random_shuffle(dvector.begin(), dvector.end()); //Shuffles the vector to rearrange the 1s and -1s

		random_shuffle(dvector.begin(), dvector.end(),  //Reshuffling the vector by feeding random index postions
			myrandom);                       //to make sure ITEM_MEMORY is unique

		ITEM_MEMORY[alph[j]] = dvector;                 //Fills the ITEM_MEMORY

														//vector<int> testvec;
														//testvec = ITEM_MEMORY.at(alph[j]);

														/*#ifdef PRINT_ITEM_MEMORY
														cout << "ITEM_MEMORY[" << alph[j] << "] = ";
														for (std::vector<int>::const_iterator i = testvec.begin(); i != testvec.end(); ++i)
														std::cout << *i << ' ';
														cout << '\n';
														#endif*/

	}


	/*std::cout << "ITEM_MEMORY contains:";
	for (std::unordered_map<char, vector<int>>::const_iterator it = ITEM_MEMORY.begin(); it != ITEM_MEMORY.end(); ++it)
	std::cout << " " << it->first << ":" << it->second;
	std::cout << std::endl;*/


}

vector<int> MAP(vector<vector<int> > tot_data) {
	int length = tot_data.size();
	vector<int> result = tot_data.back();
	int val = 1;
	for (std::vector< vector<int> >::const_iterator i = tot_data.begin(), j = --tot_data.end(); i != j; ++i, val++) {
		//	result *= circ_rotate(*i,length-val);
		std::transform(result.begin(), result.end(), circ_rotate(*i, length - val).begin(), result.begin(), std::multiplies<int>());

	}
	return result;
}

vector<int> result_words(vector<int> word, vector<int> result) {
	/*	if result is empty,
	then assign word_vector to result...
	else, add the word_vector to the existing result.

	....after this is done, assign it to the resut by returning it..
	*/
	vector<int> temp;

	if (result.empty())
	{
		temp = word;
	}
	else {
		//cout << "555";
		std::transform(result.begin(), result.end(), word.begin(), result.begin(), std::plus<int>());
		//cout << "343";
		temp = result;
	}
	return temp;
}

void add_to_assoc_mem(string category, vector<int> input) {
	size_t const inLenby2 = input.size()/2;
	if (ASSOCIATIVE_MEMORY.find(category) == ASSOCIATIVE_MEMORY.end()) {
		// Meaning that you have not found the data . or if this is the first occurence.
		vector<int> input1(input.begin(), input.begin() + inLenby2);
		vector<int> input2(input.begin() + inLenby2, input.end());

		ASSOCIATIVE_MEMORY.insert(std::pair<string, vector<int>>(category+"1", input1));
		ASSOCIATIVE_MEMORY.insert(std::pair<string, vector<int>>(category+"2", input2));
	}
	else {
		//Meaning that you have found the data andd we need to append the data to the existing data.
		vector<int> temp_assoc = ASSOCIATIVE_MEMORY.find(category)->second;
		std::transform(input.begin(), input.end(), temp_assoc.begin(), temp_assoc.begin(), std::plus<int>());
		ASSOCIATIVE_MEMORY.insert(std::pair<string, vector<int>>(category, temp_assoc));
	}

}


float testing_data(string file_name) {
	std::vector<string> path = ReadFile(file_name.c_str());
	std::vector<string>::const_iterator i;
	int successs_news = 0;
	int total_news = path.size();
	//float percentage;
	//cout << "Total test-size is: " << total_news<<"\n";
	for (i = path.begin(); i != path.end(); ++i) {
		//	std::cout << *i << "\n-----";
		string str = (string)*i;

		string strWords[2];
		short counter = 0;
		for (short i = 0; i < str.length(); i++) {
			if (str[i] == '\t')
				counter++;
			else
				strWords[counter] += str[i];
		}

		//cout << "Key is--" << strWords[0] << "\n";
		//cout << "Value --" << strWords[1] << "\n\n";
		vector<int> need_to_check = splitText_continuous(NGRAM, strWords[1]);
		//vector<int> need_to_check = splitText_space(strWords[1]);
		//get the corresponding data value based from the existing map data.
		// call the function which calculates the MAP value 
		//get the final vector value from split function....
		string resultant = final_comparison(need_to_check);
		if (strWords[0].find(resultant)!=string::npos) {

		//if (resultant.compare(strWords[0]) == 0) {
			++successs_news;
		}

	}
	cout << "Totally " << successs_news << " matches out of " << total_news << " entries\n";
	float percentage = ((float)((float)successs_news / (float)total_news) * 100);
	//cout << percentage;
	return percentage;
}

string final_comparison(vector<int> test) {
	float percent = 0.00;
	string final_category;
	typedef std::unordered_map< std::string, std::vector<int> > type_t;
	typedef type_t::iterator type_t_iter;

	int tempTest[DIMEN];
	int tempAssc[DIMEN];
	std::copy(test.begin(), test.end(), tempTest);


	for (type_t_iter i = ASSOCIATIVE_MEMORY.begin(); i != ASSOCIATIVE_MEMORY.end(); ++i) {


		std::copy(i->second.begin(), i->second.end(), tempAssc);
		float cosine_value = (cosAngle(tempAssc, tempTest, DIMEN));

		if (cosine_value > percent) {


			//if ((cosAngle(test, i->second))	> percent) {
			final_category = i->first;
			percent = cosine_value;
		}
	}

	//	cout << "Category: " << final_category << " with a percentage of " << percent << ".\n";
	return final_category;
}

double norm(int *vec, int n) {
	double result = 0;
	int i;
	for (i = 0; i<n; i++) {
		result += (int)vec[i] * vec[i];
	}
	result = sqrt(result);

	return result;
}

double cosAngle(int *u, int *v, int n) {
	long int dot = 0;
	int i;
	for (i = 0; i<n; i++) dot += u[i] * v[i];
	double temp = (norm(u, n)*norm(v, n));
	double result = (double)(dot / temp);
	//cout << "Cosine value is: " << result;
	return result;
}