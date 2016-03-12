/*
	You have all permissions to modified this code and improve its capablilities.
	SparkTeam - Computer Team
	SparkTB v1.2
*/

#include<vector>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include<string>
#include<string.h>
#include<fstream>
#include <assert.h>  
#ifndef __linux__
#include<windows.h>
#endif
using namespace std;
typedef unsigned int uint32;

#define NO_OUTPUT "xu87@#&01*"
#define RUNTIME_ERROR "$&)9(hNa*"
#define MULTI_LINES "!@#$*()_+/*"

vector<string> g_TestCase;
vector<string> g_ExpectedResult;
vector<int> g_RuntimeError;
vector<unsigned int> g_RuntimeErrorStats;

/*check if file exist in the current directory*/
bool isExist(const std::string& name);
/*read file data into a vector*/
void readFile(string loc, vector<string>& vect);
/*Write to file*/
void WriteToFile(string fileName, string data);
/*get file size*/
ifstream::pos_type filesize(const char* filename);
/*Compare between two vectors data*/
void compareResults(vector<string>& Actual, vector<string>& Expected);
/*OS dependence adjustment for executable file*/
void adjustExecutableString(string &exe_str);
/*Options to SparkTB program*/
void setOption(char* opt);
/*delete spaces in the last of the string*/
void deleteLastSpaces(string& str);
/*return number of lines in a file*/
long n_lines(const char* filename);

int main(int argc,char*argv[])
{
	string sparkTest,sparkExpected,executableProgram;

	//checking user options to enter its own test
	if(argc > 1)
	{
		if(argc == 2)
		{
			if(argv[1][0] == '-') setOption(argv[1]);
			else
			{
				executableProgram=argv[1];
				sparkTest = "SparkTestCases";
				sparkExpected = "SparkExpectedOutput";
			}
		}
		else if(argc == 3)
		{
			cerr<<"Enter another argument"<<endl;
			cout<<"SparkTB .exe/.cpp file TestCasesFile ExpectedResultsFile"<<endl;
			executableProgram=argv[1];
			if (!isExist(executableProgram))
				{
					cerr << "Missing your exexutable program?\n";
					exit(1);
				}

			exit(2);
		}
		else if(argc >4)
		{
			cerr<<"Too many arguments,Three is enough"<<endl;
			cout<<"SparkTB .exe/.cpp file TestCasesFile ExpectedResultsFile"<<endl;
			exit(3);
		}
		else
		{
			executableProgram=argv[1];
			sparkTest = argv[2];
			sparkExpected = argv[3];
		}
	}
	else
	{
		sparkTest = "SparkTestCases";
		sparkExpected = "SparkExpectedOutput";
		executableProgram = "A1";
	}

	//adjust format of executable program name according to the OS
	adjustExecutableString(executableProgram);

	//checking if the files already exist or not
	if (!(isExist(sparkTest) && isExist(sparkExpected)))
	{
		cerr << "Missing Spark Files\n";
		exit(1);
	}
	else cout << "Found Spark Files" << endl;

#ifdef __linux__
	if (!isExist(executableProgram))
	{
		cerr << "Missing your exexutable program?\n";
		exit(1);
	}
	else cout << "Found "<<executableProgram<< endl;
#else
	if (!isExist(executableProgram))
	{
		cerr << "Missing your exexutable program?\n";
		exit(1);
	}
	else cout << "Found "<<executableProgram<< endl;
#endif

	// load SparkTestCases and Results into arrays
	readFile(sparkTest, g_TestCase);
	readFile(sparkExpected, g_ExpectedResult);

	//remove actual results file if already exist before.
	if (isExist("ActualResults")) remove("ActualResults");

	cout << "Starting Tests..." << endl;
	string executeCommand;
	uint32 tests_Size = g_TestCase.size();

	//check if each testCase has its equivalent in the expected results
	if(tests_Size != g_ExpectedResult.size())
	{
		cerr<<"#TestCases are NOT equivalent To #Results"<<endl;
		cerr<<"Please Consider Making Them equivalent in length!"<<endl;
		cout<<"Stopped!"<<endl;
		cout << '\a';//produce a beep
		exit(3);
	}

	unsigned int errorFlag;
	vector<string>actualResult;

#ifndef __linux__
	SetErrorMode(0 | SEM_NOGPFAULTERRORBOX);
#endif

	//execute test cases and output result into ActualResults file
	for (uint32 i = 0; i < tests_Size; i++)
	{
#ifndef __linux__
			executeCommand = executableProgram + " " + g_TestCase.at(i)+ " 1>ActualResults";
#else
			executeCommand = "./"+executableProgram + " " + g_TestCase.at(i) + " 1>ActualResults";
#endif
			cout<<"Hit: "<<g_TestCase.at(i)<<endl;
			errorFlag = system(executeCommand.c_str());
					if( errorFlag != 0)
					{
						g_RuntimeError.push_back(i);
						g_RuntimeErrorStats.push_back(errorFlag);
						WriteToFile("ActualResults",RUNTIME_ERROR);
					}
					
			//if the program doesn't output the result of the test case,don't crash and tell him it is not implemented
				if(filesize("ActualResults") == 0)  
					WriteToFile("ActualResults",NO_OUTPUT);
				if(n_lines("ActualResults") > 1)
					WriteToFile("ActualResults",MULTI_LINES);

			readFile("ActualResults", actualResult);
	}

#ifdef __linux__
	system("clear");
#else
	system("cls");
#endif
	//compare between both Actual Results and expected Results.
	compareResults(actualResult,g_ExpectedResult);
	remove("ActualResults");
	cout<<endl<<"End Tests"<<endl;
	cout<<"Spark TeamWork - EECE 2017"<<endl;

	return 0;
}

void WriteToFile(string fileName, string data)
{
	ofstream myfile(fileName.c_str(),ios::out);
	if (myfile.fail())
	{
		perror("error creating the file or writting to it !\n");
	}
	else
	{
		myfile << data << endl;;
	}
	myfile.close();
}

bool isExist(const string& name) {

	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

void readFile(string loc, vector<string>& vect)
{
	string line;
	ifstream myfile(loc.c_str(), ios::in);

	if (myfile.fail())
		perror("Error Reading The Input File");
	else
		while (getline(myfile, line)){
			if(line != "")
				vect.push_back(line);
		} 

	myfile.close();
}

long n_lines(const char* filename)
{
	long i_line=0;
	string line;
	ifstream myfile(filename, ios::in);
	if (myfile.fail())
		perror("Error Reading The Input File");
	else
		while (getline(myfile, line)) ++i_line;

	myfile.close();

	return i_line;
}

void compareResults(vector<string>& Actual, vector<string>& Expected)
{
	uint32 nonEqualFlag =false;
	uint32 oldExpectedSize = Expected.size();
	if(Expected.size() != Actual.size())
	{
		Expected.resize(Actual.size());
		nonEqualFlag =true;
		//assert(iterations_size == Actual.size());
	}

	uint32 iterations_size = Expected.size();
	uint32 FailCounter=0;
	uint32 noOutputCounter =0;
	uint32 MULTI_LINESCounter =0;

	for (uint32 i = 0; i < iterations_size; i++)
	{
		deleteLastSpaces(Actual.at(i));
		if (Expected.at(i) == Actual.at(i)) cout << "Test#" << i + 1 << ": OK!" << endl;
		else
		{
#ifdef __linux__
			system("tput setaf 1");
#endif
			cout << "----------------------" << endl;
			if (Actual.at(i) == NO_OUTPUT)
			{
				cout << "Test#" << i + 1 << ": " << g_TestCase.at(i) << endl;
				cout << "<NO_OUTPUT>" << endl;
				noOutputCounter++;
			}
			else if(Actual.at(i) == RUNTIME_ERROR)
			{
				cout << "Test#" << i + 1 << ": " << g_TestCase.at(i) << endl;
				cout << "<RUNTIME ERROR>" << endl;
			}
			else if(Actual.at(i) == MULTI_LINES)
			{
				cout << "Test#" << i + 1 << ": " << g_TestCase.at(i) << endl;
				cout << "<MULTI_LINES>" << endl;
				MULTI_LINESCounter++;
			}
			else
			{
				if(FailCounter < 3)
					cout << '\a';//produce a beep
				cout << "Test#" << i + 1 << ": Failed!" << endl;
				cout<<"Command: "<<g_TestCase.at(i)<<endl;
				cout << "Expected Output: '" << Expected.at(i) << "', Length: " << Expected.at(i).size() << endl;
				cout << "Actual Output:   '" << Actual.at(i) << "', Length: " << Actual.at(i).size() << endl;
				FailCounter++;
			}
			cout << "----------------------" << endl;
		

#ifdef __linux__
			system("tput sgr0");
#endif
		}
	}


	if(g_RuntimeError.size() > 0){
		cout<<"****************"<<endl;
		cout<<"[RUNTIME ERRORS]"<<endl;
		cout<<"****************"<<endl;
		for(int i = 0; i < g_RuntimeError.size(); i++){
			int testCaseIndex = g_RuntimeError.at(i);
			cout<<"Run time Error at Test Case "<< testCaseIndex+1 <<": "<<g_TestCase.at(testCaseIndex)<<endl;
			cout<<"exit with exception code: 0x"<<hex<<g_RuntimeErrorStats.at(i)<<dec<<endl;
			cout<<"---------------"<<endl;
		}
	}	
	cout<<endl;
	cout<<"Total Tests: "<<iterations_size<<endl;
	cout<<"Passed Tests: "<<(iterations_size-FailCounter-noOutputCounter-g_RuntimeError.size()-MULTI_LINESCounter) <<endl;
	cout<<"Failed Tests: "<<FailCounter <<endl;
	cout<<"No Output Tests: "<<noOutputCounter<<endl;
	cout<<"Run Time Error Tests: "<<g_RuntimeError.size()<<endl;
	cout<<"Multi Lines Outputs from the target:"<<MULTI_LINESCounter<<endl;

	if(nonEqualFlag == true)
	{
		cout<<endl;
		cout<<"Expected iterations size: "<<oldExpectedSize<<endl;
		cout<<"Actual size: "<<Actual.size()<<endl<<endl;
		cout<<"If you see this"<<endl;
		cout<<"that's meaning your program is doing something wrong"<<endl;
		cout<<"and test tool and cannot detect it"<<endl;
		cout<<Actual.size()<<" Tests performed according to the behaviour of your program"<<endl;
	}


}
void adjustExecutableString(string &exe_str)
{

#ifndef __linux__


	if (exe_str.find(".exe") != string::npos)
		return;
	else if(exe_str.find(".cpp") != string::npos){
		cout<<"Compile against ISO C++98"<<endl;
		string Compilecommand = "g++ "+ exe_str + " -std=c++98 -pedantic" + " -o " + "SparkRun";
		system(Compilecommand.c_str());
		exe_str = "SparkRun.exe";
	}
	else
		exe_str += ".exe";
#else
			if (exe_str.find(".cpp") != string::npos)
			{
				cout<<"Compile against ISO C++98"<<endl;
				string Compilecommand = "g++ "+ exe_str + " -std=c++98 -pedantic" + " -o " + "SparkRun";
				system(Compilecommand.c_str());
				exe_str = "SparkRun";
			}
			else
				return;
#endif

}

void setOption(char* option)
{
	string opt = option;
	if(opt == "-help")
	{
		cout<<"run like this"<<endl;
		cout<<"SparkTB .exe/.cpp file TestCasesFile ExpectedResultsFile"<<endl;
		cout<<"You can also create your own custom Testcases & ExpectedResultsFile"<<endl;
		cout<<"and pass them To the program"<<endl;;

	}
	else if(opt == "-version") cout<<"Developed by SparkTeam,Test Tool v1.1"<<endl;
	else if(opt  == "-clear")
	{
		remove("ActualResults");
		remove("SparkRun.exe");
		remove("SparkRun");
		cout<<"Done"<<endl;
	}
	else {
		cout<<"Undefined options"<<endl;
	}
	exit(0);
}


void deleteLastSpaces(string& str)
{
		size_t len = str.size();
		if(str.at(len - 1) != ' ') return;
		else
		{
			str.erase(str.end() - 1);
			deleteLastSpaces(str);
		}
}