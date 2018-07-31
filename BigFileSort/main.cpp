#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool MergeFiles(string file1,string file2,string fileOutput)
{
	ifstream f1in(file1);
	ifstream f2in(file2);
	ofstream fout(fileOutput);
	if (!fout.is_open() || !f1in.is_open() || !f2in.is_open())
		return false;
	string strF1, strF2;
	
	bool b1 = !f1in.eof(),b2 = !f2in.eof();
	
	getline(f1in, strF1);
	getline(f2in, strF2);

	if (!b1 || !b2) 
		return true;

	while (b1&&b2)
	{
		if (strF1 < strF2)
		{
			fout << strF1 <<endl;
			if (f1in.eof())
				b1 = false;
			getline(f1in, strF1);
		} else {
			fout << strF2 << endl;
			if (f2in.eof())
				b2 = false;
			getline(f2in, strF2);
		}
	}

	if (!b1) 
	{
		while (true)
		{
			fout << strF2;
			if (f2in.eof())
				break;
			fout << endl;
			getline(f2in, strF2);
		}
	} else {
		while (true)
		{
			fout << strF1;
			if (f1in.eof())
				break;
			fout << endl;
			getline(f1in, strF1);
		}
	}
	f1in.close();
	f2in.close();
	fout.close();
	return true;
}

bool MergeArrFile(vector<string> &sArr, string filein, string fileOutput)
{
	ifstream fin(filein);
	ofstream fout(fileOutput);
	if (!fout.is_open() || !fin.is_open())
		return false;
	string str;
	int i = 0;
	bool b1 = !fin.eof(), b2 = i < sArr.size();

	if (!b1 || !b2)
		return true;

	getline(fin, str);
	
	while (b1&&b2)
	{
		if (str < sArr[i])
		{
			fout << str << endl;
			if (fin.eof())
				b1 = false;
			getline(fin, str);
		}
		else {
			fout << sArr[i++] << endl;
			if (i>=sArr.size())
				b2 = false;
		}
	}

	if (!b1)
	{
		while (i<sArr.size())
		{
			fout << sArr[i++] << endl;
		}
	}
	else {
		while (true)
		{
			fout << str << endl;
			if (fin.eof())
				break;
			getline(fin, str);
		}
	}
	fin.close();
	fout.close();
	return true;
}

bool ArrOut(vector<string> &sArr, string fileName)
{
	ofstream fout(fileName);
	if (!fout.is_open()) 
		return false;
	if(sArr.size()>0)
		fout << sArr[0];
	for (int i = 1; i < sArr.size(); i++)
	{
		fout << endl << sArr[i];
	}
	fout.close();
	return true;
}

bool runingError(int argc, char** argv,int& buff) 
{
	if (argc <= 1)
	{
		cout << "syntax error\n";
		return true;
	}
	if (string(argv[1]) == "/\?")
	{
		setlocale(0, "RUS");
		cout << "Sort [путь][имя_файла] [количество строк]" << endl;
		cout << "Пример\n";
		cout << "Sort input.txt 5000" << endl;
		return true;
	}

	try
	{
		if (argc > 2) {
			buff = atoi(argv[2]);
			if (buff == 0)
				throw exception();
		}
	}
	catch (const std::exception&)
	{
		setlocale(0, "RUS");
		cout << "Синтаксическая ошибка\n/\? - Help";
		return true;
	}
	return false;
}

int main(int argc,char** argv)
{
	int buff = 100;
	if (runingError(argc,argv,buff))
		return 1;

	ifstream fin(argv[1]);
	if (!fin.is_open())
	{
		setlocale(0, "RUS");
		cout << "Файл "<< argv[1] <<" не найден\n/\? - Help";
		return 1;
	}
	system("md tmp");
	vector<string> arr(buff);
	int j = 0;
	while (!fin.eof())
	{
		int i;
		string str;
		for (i = 0; i < arr.size() && !fin.eof(); i++)
		{
			getline(fin, arr[i]);
		}
		sort(arr.begin(), arr.begin()+i);
		if (i >= arr.size()) {
			ArrOut(arr, "tmp\\first" + to_string(j++) + ".txt");
		} else { 
			arr.erase(arr.begin() + i, arr.end());
			ArrOut(arr, "tmp\\first" + to_string(j++) + ".txt"); 
		}
	}

	bool b = true;
	while (j > 1)
	{
		if (b) {
			for (int i = 0; i < j; i += 2)
			{
				if (!MergeFiles("tmp\\first" + to_string(i) + ".txt", "tmp\\first" + to_string(i + 1) + ".txt", "tmp\\second" + to_string((int)(i / 2)) + ".txt"))
				{
					cout << "error marge first" << i << ".txt And first" << i + 1 << ".txt\n";
					string str1 = "ren tmp\\first";
					str1 += to_string(i) + ".txt ";
					string str2 = " second";
					str2 += to_string((int)(i / 2)) + ".txt";
					system(("del /s /q /a " + str2).c_str());
					system((str1+str2).c_str());
					j++;
				}
			}
			system("del /s /q /a first*.txt");
		} else {
			for (int i = 0; i < j; i += 2)
			{
				if (!MergeFiles("tmp\\second" + to_string(i) + ".txt", "tmp\\second" + to_string(i + 1) + ".txt", "tmp\\first" + to_string((int)(i / 2)) + ".txt"))
				{
					cout << "error marge second" << i << ".txt And second" << i + 1 << ".txt\n";
					string str1 = "ren tmp\\second";
					str1 += to_string(i) + ".txt ";
					string str2 = " first";
					str2 += to_string((int)(i / 2)) + ".txt";
					system(("del /s /q /a " + str2).c_str());
					system((str1 + str2).c_str());
					j++;
				}
			}
			system("del /s /q /a second*.txt");	
		}
		b = !b;
		j /= 2;
	}
	return 0;
}