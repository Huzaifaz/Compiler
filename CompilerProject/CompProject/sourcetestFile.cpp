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

	/*for (int i = 0; i < rows; i++)
	{
		
		for (int j = 0; j < cols; j++)
		{
			cout << tm[i][j] << " ";
		}
		cout << endl;
		
	}*/
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
	

	//	cout << "map size: " << chmap.size() << endl;
	//	cout << "size: " << size << endl;
	/*for (int i = 0; i < size; i++)
     {
		cout << chmap[i] << " ";     
     }*/
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
	//cout << "error" << endl;
	char ib = read.peek();

	//if ((ib == 'E' || ib == 'e')&& isnum)
		//     isnum = false;             //preventing double ee or EE 
	//cout << "ib: " << ib << endl;
	if (read.eof())
	{
		return -1;
	}
	if ((ib != ' ' && state != -1))
	{
			//cout << "error main if" << endl;
		int finalstate = 24;
		int chstate = getchmap(ib, chmap, isnum);
		int nextstate = table[state][chstate];
		//cout << "nextstate before if " << nextstate << endl;
		if (nextstate == -1)
		{
		//	cout << "returning state only " << endl;
			return state;
		}
		if (table[nextstate][finalstate] == 1)
		{
			//read.get();
		//	cout << "nextstate advanced: " << nextstate << endl;
			return nextstate;
		}
		//cout << "state advanced: " << state << endl;
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
	read.open(filename);
	//reading files
	vector<string> keywords = readfile("keywords.txt");
	vector<string> operators = readfile("operators.txt");
	vector<string> punctuations = readfile("punctuations.txt");

	
	vector<string> symbolTable;
	vector<string> tokens;
	vector<string> errors;
	//for reading lexi in input buffer
	vector<string> lexi;
	string lex="";
	//00
	char ib;
	int state=0;
	int chstate = 0;
	int nextstate=0;
	int finalstate=24;
	bool firstisnum = true;
	//cout << "Final state: " << table[5][finalstate] << endl;
	bool isnum= false;
	char d;
	//ib = '_';
	//chstate = getchmap(ib,chmap,isnum);
	//cout << "mystate: " << chstate << endl;
	while (!read.eof())
	{
		read.get(ib);
		//cout << "char: " << ib << endl;
		//if (ib == ' ')
		//	cout << "char: " << ib << "space" << endl;
		chstate = getchmap(ib, chmap, isnum);
		//cout << "chstate: " << chstate << endl;
	    

		if (firstisnum && chstate==1)
		{
			isnum = true;
		}
		firstisnum = false;

		state = table[nextstate][chstate];
		//cout << "state: " << state << endl;
		if (ib == ' ' || ib == '\n')
			d = ib;                   //dummy chracter
		else 
			lex += ib;
			

		state=checkAdvancedState(read, state, table, chmap, isnum);
		//cout << "after advanced state: " << endl;
		if (state == -1 || table[state][finalstate]==1)
		{
			//cout << "hello this is the end" << endl;
			if (lex != "")
			{
				string v = createTokens(lex, symbolTable, keywords, operators, punctuations, errors);
				if(v!="")
				tokens.push_back(v);
				lexi.push_back(lex);
			}
			
			
			//resetting the values after final state
			state = 0;
			chstate = finalstate; //putting the final state because starting state is not final
			isnum = false;
			firstisnum = true;
			lex = ""; //emptying the string after lexeme is complete
			
		}
		//cout << "state and chstate: " << state << " s " << chstate << endl;
		nextstate = table[state][chstate];
		//cout << "Next State: " << nextstate << endl;	
		//if (nextstate == -1)
		//	nextstate = 0;
	
	}

	//cout << "Print lexi: " << endl;

	//print(lexi);
	//cout << "Symbol table: " << endl;
	//print(symbolTable);
	//cout << "Tokens: " << endl;
	//print(tokens);
	//cout << "Errors: " << endl;
	//print(errors);

	createfiles("SymbolTable.txt", symbolTable);
	createfiles("Tokens.txt", tokens);
	createfiles("Errors.txt", errors);
}


//need to cater 3EE
// neet to cater 3+5 it is 3 lexemes
//int a224_ a_b = 0;
//3EEE+3 
/*void createLexemes(vector<string>word)
{
	for (int i = 0; i < word.size(); i++)
	{
		lex(word[i]);
	}
}*/

int main()
{
	vector<vector<int>> tablemap;
	vector<char> chmap;
	vector<string> word;
    vector<string> lexeme;
	readcharacter(chmap, "charactermap.txt");
	readmap(tablemap, "mapping.txt");
	lexicalAnalyzer( "sourcefile.txt",tablemap,chmap);
	//createLexemes(lexeme);

	return 0;
}


//if ((ib >= 97 && ib <= 122) || ib == '_')
//{
//	//alphabet state
//	d.push_back(ib);
//	alphabetState(table, ib, read, d);
//	lexi.push_back(d);
//	//cout << lexi[0] << endl;
//}
//else if (ib == ' ')
//{
//	state = 8;
//}
//else
//{
//	state = 8;
//}


/*void alphabetState(vector<vector<int>> table, char ib, ifstream& read, string& l)
{
	//starting state is 7
	int state = table[7][0];
	int nextstate = table[7][0];
	//char ib;

	while (nextstate != -1)
	{
		read.get(ib);
		//cout << "this: " << ib << endl;
		if ((ib >= 97 && ib <= 122) || ib == '_')
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



}*/


/*
    cout << "error" << endl;
	char ib = read.peek();
	//cout << "ib: " << ib << endl;
	if (read.eof())
	{
		return -1;
	}
	if ((ib != ' ' &&  state != -1))
	{
	//	cout << "error main if" << endl;
		int finalstate = 24;
		int chstate = getchmap(ib, chmap, isnum);
		int nextstate = table[state][chstate];
		//cout << "nextstate before if" << nextstate << endl;
		if (nextstate == -1)
		{
			return state;
		}
		if (table[nextstate][finalstate] == 1)
		{
			cout << "nextstate advanced: " << nextstate << endl;
			return nextstate;
		}
		cout << "state advanced: " << state << endl;
	}
	return state;
*/