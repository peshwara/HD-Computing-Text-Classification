	

	// reading a text file
#include <iostream>
#include <fstream>
#include <string>
	using namespace std;
	const int portion = 50;
	int main() {
		string line;
		string outline;
		ifstream myfile("D:\\Users\\Hariharan\\Desktop\\testfiles\\split.txt");

		ofstream outfile;
		outfile.open("D:\\Users\\Hariharan\\Desktop\\testfiles\\out_50.txt");

		float linesize = portion / 100;
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{

				outline = line.substr(0, int(line.length()*portion/100));
				cout << line << '\n';
				cout << outline << '\n';
				outfile << outline << '\n';
			}
			myfile.close();
			outfile.close();
		}

		else cout << "Unable to open file";
		getchar();
		return 0;
	}
	