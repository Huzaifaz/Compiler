#include<iostream>
#include <vector>
#include<fstream>
#include<string>
using namespace std;



void readmap(vector<vector<int>>& tm, const char* filename)
{
	ifstream file;
	file.open(filename);

	int rows = 0;
	int cols = 0;
	file >> rows >> cols;
	cout << "Rows and Col: " << rows << " " << cols << endl;
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

}


void readcharacter(vector<char>& chmap, const char* filename)
{
	ifstream file;
	file.open(filename);

	int size = 0;

	file >> size;
	int r;
	char ch;
		
		for (int i = 0; i < size; i++)
		{
			file >> ch;
			chmap.push_back(ch);
		}
	
}


int getchmap(char ib,vector<char>chmap,bool& isnum)
{

	if ((ib >= 65 && ib <= 90 && !isnum) || (ib >= 97 && ib <= 122 && !isnum))
		ib = 'L';
	else if (ib >= 48 && ib <= 57)
		ib = 'N';
	else if ((ib == 'E' || ib == 'e') && isnum)
	{
		ib = 'E';
		isnum = false; //it will read the E two time after number
	}
	
	for (int i = 0; i < chmap.size(); i++)
	{
		if (ib == chmap[i])
		{
			return i;
		}
	}
	return 23;  //other state
}


int checkAdvancedState(ifstream& read,int state, vector<vector<int>> table, vector<char>chmap,bool isnum)
{
	char ib = read.peek();

	if (read.eof())
	{
		return -1;
	}
	if ((ib != ' ' && state != -1))
	{	
		int finalstate = 24;
		int chstate = getchmap(ib, chmap, isnum);
		int nextstate = table[state][chstate];

		if (nextstate == -1)
		{
			return state;
		}
		if (table[nextstate][finalstate] == 1)
		{
			return nextstate;
		}
	}
	return state;
	
	
}

vector<string> readfile(const char* filename)
{
	vector<string> rk;
	string s;
	ifstream read;
	read.open(filename);

	while (!read.eof())
	{
		read >> s;
		if (read.eof())           //this is to avoid reading last character multiple times
			break;
		rk.push_back(s);
	}
	
	return rk;
}

void print(vector<string> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		cout << s[i] << endl;
	}
	cout << endl;
}

bool checktoken(string lex, vector<string> str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (lex.compare(str[i])==0)
		{
			return true;
		}
	}
	return false;
}

string createTokens(string lex,vector<string>&symbolTable, vector<string> keywords, vector<string>operators, vector<string> punctuations,vector<string>& error)
{
	//creating tokens here
	int i = 0;
	for (i = 0; i < symbolTable.size(); i++)
	{
		if (lex.compare(symbolTable[i])==0)
		{
		
			if (lex.find('_') != string::npos)
			{
				return "< " + to_string(i + 1) + ", identifier>";
			}
			else
				return "< " + to_string(i + 1) + ", keyword>";
		}
	}

	if (lex.find('_') != string::npos)
	{
		symbolTable.push_back(lex);
		return "< " + to_string(i + 1) + ", identifier>";
	}
	else if (lex[0] >= 48 && lex[0] <= 57)
	{
		return "< " + lex + ", number>";
	}
	else if (checktoken(lex, keywords))
	{
		symbolTable.push_back(lex);
		return "< " + to_string(i + 1) + ", keyword>";
	}
	else if (checktoken(lex, operators))
	{
		return "< " + lex + ", operator>";
	}
	else if (checktoken(lex, punctuations))
	{
		return "< " + lex + ", punctuation>";
	}
	else
		error.push_back(lex);

	return "";
}


void createfiles(const char* filename, vector<string>writer)
{
	ofstream file;
	file.open(filename);
	for (int i = 0; i < writer.size(); i++)
	{
		file << writer[i] << endl;
	}
}

void lexicalAnalyzer(const char*filename, vector<vector<int>> table,vector<char>chmap)
{
	ifstream read;
	//opening sourcefile.txt
	read.open(filename);
	//reading files
	vector<string> keywords = readfile("keywords.txt");
	vector<string> operators = readfile("operators.txt");
	vector<string> punctuations = readfile("punctuations.txt");

	//this is for storing things in lexical anaylyzer
	vector<string> symbolTable;
	vector<string> tokens;
	vector<string> errors;
	//for reading lexi in input buffer
	
	vector<string> lexi;
	string lex="";
	
	//variables for input buffer
	char ib;
	int state=0;
	int chstate = 0;
	int nextstate=0;
	int finalstate=24;
	bool firstisnum = true;
	bool isnum= false;

	while (!read.eof())
	{
		read.get(ib);
		chstate = getchmap(ib, chmap, isnum); //match the character and return its index value just like map
	
		if (firstisnum && chstate==1)  //this is for exponent numbers
		{
			isnum = true;
		}
		
		firstisnum = false;
		state = table[nextstate][chstate];
		
		if (ib != ' ' && ib != '\n')
			lex += ib;       //avoiding space and nextline in lexemes

		//checking further by using file.peek() method and return if it is dead or finished
		state=checkAdvancedState(read, state, table, chmap, isnum);

		if (state == -1 || table[state][finalstate]==1)
		{
	
			if (lex != "")
			{
				string v = createTokens(lex, symbolTable, keywords, operators, punctuations, errors);
				if(v!="")
				tokens.push_back(v);  //inserting valid tokens inside
				lexi.push_back(lex);  //inserting each lexemes
			}		
			
			//resetting the values after final state
			state = 0;
			//putting the final state of starting state because starting state is not final
			chstate = finalstate; 
			//resetting the exponent number check
			isnum = false;  
			firstisnum = true;
			//resetting lexemes
			lex = ""; //emptying the string after lexeme is complete

		}
		nextstate = table[state][chstate];		
	}

	createfiles("SymbolTable.txt", symbolTable);
	createfiles("Tokens.txt", tokens);
	createfiles("Errors.txt", errors);
}


int main()
{
	vector<vector<int>> tablemap;
	vector<char> chmap;

	readcharacter(chmap, "charactermap.txt");        // reading character files
	readmap(tablemap, "mapping.txt");               //reading map values 
	lexicalAnalyzer( "sourcefile.txt",tablemap,chmap);
	
	return 0;
}