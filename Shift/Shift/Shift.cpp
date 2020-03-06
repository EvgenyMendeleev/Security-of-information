#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <string>
#include <vector>

using namespace std;

struct Point
{
	unsigned short i;
	unsigned short j;
};

void ToLower(string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = static_cast<char>(tolower(str[i]));
	}
}

void deleteChars(string& str, char ch)
{
	long pos = 0;
	while ((pos = str.find(ch, pos)) != string::npos)
	{
		str.erase(pos, 1);
	}
}

Point* FindPointOfChar(vector<char>& vec, unsigned short ROWS, unsigned short COLUMNS, char ch)
{
	for (unsigned short i = 0; i < ROWS; i++)
	{
		for (unsigned short j = 0; j < COLUMNS; j++)
		{
			if (vec[i * COLUMNS + j] == ch)
			{
				return new Point{ i, j };
			}
		}
	}

	return nullptr;
}

void LoadingOfCodeTable(ifstream& keyIn, vector<char>& codeTable, unsigned short ROWS, unsigned COLUMNS)
{
	const unsigned short COUNT_OF_CHAR = 32;
	bool usedChar[COUNT_OF_CHAR] = {};

	string codeData;
	while (getline(keyIn, codeData))
	{
		ToLower(codeData);

		for (size_t i = 0; i < codeData.length(); i++)
		{
			unsigned short pos = abs('à' - codeData[i]);

			if (!usedChar[pos])
			{
				usedChar[pos] = true;
				codeTable.push_back(codeData[i]);
			}
		}
	}

	for (unsigned short i = 0; i < COUNT_OF_CHAR; i++)
	{
		if (!usedChar[i])
		{
			usedChar[i] = true;
			codeTable.push_back('à' + i);
		}
	}
}

void CreatePairs(string& strOfText, vector<string>& vec)
{
	for (size_t i = 0; i < strOfText.length(); i += 2)
	{
		string tmp = "";
		tmp.push_back(strOfText[i]);

		if (i == strOfText.length() - 1)
		{
			tmp.push_back('ú');
			vec.push_back(tmp);
		}
		else
		{
			if (strOfText[i] == strOfText[i + 1])
			{
				tmp.push_back('ú');
				vec.push_back(tmp);

				--i;
			}
			else
			{
				tmp.push_back(strOfText[i + 1]);
				vec.push_back(tmp);
			}
		}
	}
}

void Shifr(ifstream& textIn, ofstream& fout, vector<char>& codeTable, unsigned short ROWS, unsigned short COLUMNS)
{
	string strOfText;
	while (getline(textIn, strOfText))
	{
		ToLower(strOfText);
		deleteChars(strOfText, ' ');

		vector<string> pairs;
		CreatePairs(strOfText, pairs);

		for (size_t i = 0; i < pairs.size(); i++)
		{
			Point* firstAlpha = FindPointOfChar(codeTable, ROWS, COLUMNS, pairs[i][0]);
			Point* secondAlpha = FindPointOfChar(codeTable, ROWS, COLUMNS, pairs[i][1]);

			if (firstAlpha->i == secondAlpha->i)
			{
				unsigned short column = firstAlpha->j == COLUMNS - 1 ? 0 : firstAlpha->j + 1;
				pairs[i][0] = codeTable[firstAlpha->i * COLUMNS + column];

				column = secondAlpha->j == COLUMNS - 1 ? 0 : secondAlpha->j + 1;
				pairs[i][1] = codeTable[secondAlpha->i * COLUMNS + column];
			}
			else if (firstAlpha->j == secondAlpha->j)
			{
				unsigned short row = firstAlpha->i == ROWS - 1 ? 0 : firstAlpha->i + 1;
				pairs[i][0] = codeTable[row * COLUMNS + firstAlpha->j];

				row = secondAlpha->i == ROWS - 1 ? 0 : secondAlpha->i + 1;
				pairs[i][1] = codeTable[row * COLUMNS + secondAlpha->j];
			}
			else
			{
				pairs[i][0] = codeTable[firstAlpha->i * COLUMNS + secondAlpha->j];
				pairs[i][1] = codeTable[secondAlpha->i * COLUMNS + firstAlpha->j];
			}

			delete firstAlpha;
			delete secondAlpha;
		}

		for (int i = 0; i < pairs.size(); i++)
		{
			fout << pairs[i];
		}
	}
}

void Deshifr(ifstream& textIn, ofstream& fout, vector<char>& codeTable, unsigned short ROWS, unsigned short COLUMNS)
{
	string strOfText;
	while (getline(textIn, strOfText))
	{
		ToLower(strOfText);
		deleteChars(strOfText, ' ');

		vector<string> pairs;
		CreatePairs(strOfText, pairs);

		for (size_t i = 0; i < pairs.size(); i++)
		{
			Point* firstAlpha = FindPointOfChar(codeTable, ROWS, COLUMNS, pairs[i][0]);
			Point* secondAlpha = FindPointOfChar(codeTable, ROWS, COLUMNS, pairs[i][1]);

			if (firstAlpha->i == secondAlpha->i)
			{
				unsigned short column = firstAlpha->j == 0 ? COLUMNS - 1 : firstAlpha->j - 1;
				pairs[i][0] = codeTable[firstAlpha->i * COLUMNS + column];

				column = secondAlpha->j == 0 ? COLUMNS - 1 : secondAlpha->j - 1;
				pairs[i][1] = codeTable[secondAlpha->i * COLUMNS + column];
			}
			else if (firstAlpha->j == secondAlpha->j)
			{
				unsigned short row = firstAlpha->i == 0 ? ROWS - 1 : firstAlpha->i - 1;
				pairs[i][0] = codeTable[row * COLUMNS + firstAlpha->j];

				row = secondAlpha->i == 0 ? ROWS - 1 : secondAlpha->i - 1;
				pairs[i][1] = codeTable[row * COLUMNS + secondAlpha->j];
			}
			else
			{
				pairs[i][0] = codeTable[firstAlpha->i * COLUMNS + secondAlpha->j];
				pairs[i][1] = codeTable[secondAlpha->i * COLUMNS + firstAlpha->j];
			}

			delete firstAlpha;
			delete secondAlpha;
		}

		for (int i = 0; i < pairs.size(); i++)
		{
			fout << pairs[i];
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	ifstream textIn("Input Files/Input.txt");
	ifstream keyIn("Input Files/Key.txt");
	ofstream fout("output.txt");

	const unsigned short ROWS = 8;
	const unsigned short COLUMNS = 4;
	vector<char> codeTable;

	LoadingOfCodeTable(keyIn, codeTable, ROWS, COLUMNS);

	unsigned short choice;
	cout << "ØÈÔÐ ÏËÅÉÔÅÐÀ" << endl;
	cout << "********************" << endl;
	cout << "1. Çàøèôðîâàòü" << endl;
	cout << "2. Ðàñøèôðîâàòü" << endl;
	cin >> choice;

	switch (choice)
	{
	case 1:
		Shifr(textIn, fout, codeTable, ROWS, COLUMNS);
		break;
	case 2:
		Deshifr(textIn, fout, codeTable, ROWS, COLUMNS);
		break;
	}

	return 0;
}