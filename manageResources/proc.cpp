#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <unistd.h>
using namespace std;
void CommandInput(string, string);
int parsingData(string, string);
double TotalPorcentage(long, long, string);
int parsingData2(string, string, int);
int main()
{
	while (true)
	{
		long Memavailable = parsingData("/proc/meminfo", "MemAvailable");
		long MemTotal = parsingData("/proc/meminfo", "MemTotal");
		TotalPorcentage(Memavailable, MemTotal, "Memoria Total");
		int User = parsingData2("/proc/stat", "cpu", 0);
		int System = parsingData2("/proc/stat", "cpu", 2);
		int Idle = parsingData2("/proc/stat", "cpu", 3);
		int Nice = parsingData2("/proc/stat", "cpu", 1);
		int TotalTime = User + System + Idle + Nice;
		cout << "Total Time: " << TotalTime << endl;
		TotalPorcentage(User, TotalTime, "User");
		TotalPorcentage(System, TotalTime, "System");
		TotalPorcentage(Idle, TotalTime, "Idle");
		CommandInput("/proc/stat", "ctxt");
		CommandInput("/proc/stat", "processes");
		usleep(1000000);
	}
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
int parsingData(string Input, string Output)
{
	fstream file;
	int number;
	int numbers[6];
	file.open(Input, ios::in);
	string word;
	if (file.is_open())
	{
		while (getline(file, word, ':'))
		{
			if (word.find(Output) != string::npos)
			{
				string line;
				file >> number;
			}
		}
	}
	file.close();
	return number;
}

double TotalPorcentage(long Number, long Total, string Name)
{
	double result;
	result = (Number * 100.0) / Total;
	cout << Name << ": " << result << "%" << endl;
	return result;
}

int parsingData2(string Input, string Output, int num)
{
	ifstream file;
	int number;
	int numbers[6];
	file.open(Input, ios::in);
	if (!file.is_open())
	{
		return 1;
	}
	string line;
	while (getline(file, line, ' '))
	{
		if (line.find(Output) != string::npos)
		{
			for (int i = 0; i < 6; i++)
			{
				file >> numbers[i];
			}
		}
	}
	if (num == 0)
	{
		cout << "User mode: " << numbers[num] << endl;
	}
	else if (num == 2)
	{
		cout << "System mode: " << numbers[num] << endl;
	}
	else if (num == 3)
	{
		cout << "Idle mode: " << numbers[num] << endl;
	}
	file.close();
	return numbers[num];
}
