#include<iostream>
#include <vector>
#include<fstream>
using namespace std;



void readvalue(vector<vector<int>>& tm, const char* filename)
{
	ifstream file;
	file.open(filename);

	int rows = 0;
	int cols = 0;
	file >> rows >> cols;
	int r;
	for (int i = 0; i < rows; i++)
	{
		vector<int> d;
		for (int j = 0; j < cols; j++)
		{
			file >> r;
			d.push_back(r);
		}
		tm.push_back(d);
	}

	/*for (int i = 0; i < rows; i++)
	{
		
		for (int j = 0; j < cols; j++)
		{
			cout << tm[i][j] << " ";
		}
		cout << endl;
		
	}*/
}


void alphabetState(vector<vector<int>> table,char ib,ifstream & read,string& l)
{
	//starting state is 7
	int state = table[7][0];
	int nextstate = table[7][0];
	//char ib;
	
	while (nextstate != -1)
	{
		read.get(ib);
		//cout << "this: " << ib << endl;
		if ((ib >= 97 && ib <= 122) || ib== '_')
		{
			state = table[7][0];
			l.push_back(ib);
		}
		else if (ib >= 48 && ib <= 57)
		{
			state = table[7][0];
			//l += ib;
			l.push_back(ib);
		}
		else
		{
			state = table[7][4];   // going to dead state 
		}

		nextstate = table[state][0];
	}
	
	
	
}



void readSourcefile(const char*filename, vector<vector<int>> table)
{
	ifstream read;
	read.open(filename);
	char ib;
	string d;
	vector<string> lexi;
	//00
	int pt;
	int state=0;
	int nextstate;
	int finalstate;
	while (!read.eof())
	{
		read.get(ib);
		d = "";
		//cout << ib << " ";
	    if ((ib >= 97 && ib <= 122) || ib == '_')
		{
			//alphabet state
			d.push_back(ib);
			alphabetState(table, ib, read, d);
			lexi.push_back(d);
			//cout << lexi[0] << endl;
		}
		else if(ib == ' ')
		{
			state = 8;
		}
		else
		{
			state = 8;
		}
		
		//nextstate = table[state][0];

		//if (table[nextstate][24] == -1)   //is final or not
		//{

		//}
	}

	cout << "Print lexi: " << endl;

	for (int i = 0; i < lexi.size(); i++)
	{
		cout << lexi[i] << endl;
	}
}

void lex(string str)
{
	char ib;
	for (int i = 0; i < str.size(); i++)
	{
		ib = str[i];
		if (ib >= 97 && ib <= 122)
		{
	      
		}
	}


}

void createLexemes(vector<string>word)
{
	for (int i = 0; i < word.size(); i++)
	{
		lex(word[i]);
	}
}

int main()
{
	vector<vector<int>> tablemap;
	vector<string> word;
    vector<string> lexeme;
	readvalue(tablemap, "mapping.txt");
	readSourcefile( "sourcefile.txt",tablemap);
	createLexemes(lexeme);

	return 0;
}