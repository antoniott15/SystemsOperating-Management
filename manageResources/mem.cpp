#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <unistd.h>
using namespace std;
void CommandInput(string, string);
void CommandInput2(string);
int main()
{
	CommandInput("/proc/meminfo", "MemTotal");
	CommandInput("/proc/cpuinfo", "model name");
	CommandInput("/proc/version", "version");
	CommandInput2("/proc/uptime");
	return 0;
}

void CommandInput(string Input, string Output)
{
	fstream file;
	file.open(Input, ios::in);
	string word;
	if (file.is_open())
	{
		while (getline(file, word))
		{
			if (word.find(Output) != string::npos)
				cout << word << endl;
		}
	}
	else
	{
		cout << "error" << endl;
	}
	file.close();
}

void CommandInput2(string Input)
{
	fstream file;
	file.open(Input, ios::in);
	string time2;
	if (file.is_open())
	{
		while (getline(file, time2))
		{
			cout << "La ultima vez que se prendio fue hace: " << time2 << "s" << endl;
		}
	}
	else
	{
		cout << "error" << endl;
	}
	file.close();
}
