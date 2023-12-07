// WORD GAMES - BOARD MAKER
// INITIAL SKELETON OF THE PROGRAM + SOME FUNCTIONS
//================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <string>  
#include <cctype>
#include <cstdlib>  // For system() function
#include <limits>
#include <windows.h>
#include <filesystem>


//--------------------------------------------------------------------------------
using namespace std;
namespace fs = std::filesystem;

//================================================================================
// COLOR CODES / ANSI ESCAPE SEQUENCES
//================================================================================
// TEXT COLOR
#define NO_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BROWN "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define LIGHTGRAY "\033[0;37m"
#define DARKGRAY "\033[1;30m"
#define LIGHTRED "\033[1;31m"
#define LIGHTGREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LIGHTBLUE "\033[1;34m"
#define LIGHTMAGENTA "\033[1;35m"
#define LIGHTCYAN "\033[1;36m"
#define WHITE "\033[1;37m"
// BACKGROUND COLOR
#define BLACK_B "\033[0;40m"
#define RED_B "\033[0;41m"
#define GREEN_B "\033[0;42m"
#define YELLOW_B "\033[0;43m"
#define BLUE_B "\033[0;44m"
#define MAGENTA_B "\033[0;45m"
#define CYAN_B "\033[0;46m"
#define WHITE_B "\033[1;47m"
//================================================================================
// USER DEFINED TYPES
//================================================================================
typedef struct
{
	char lin;
	char col;
	char dir;
} WordPosition;
//--------------------------------------------------------------------------------
typedef struct
{
	WordPosition pos;
	string word;
} WordOnBoard;
//--------------------------------------------------------------------------------
typedef struct
{
	vector<vector<char>> boardCells;
	int numLins;
	int numCols;
	vector<WordOnBoard> wordsOnBoard;
} Board;
//--------------------------------------------------------------------------------
typedef vector<string> WordList;
//================================================================================

//================================================================================
// Converts all characters of ‘s’ to lowercase
void tolowerStr(string& s)
// Because a pointer is used, a return is unecessary since the changes are occuring to the original variable
{
	// The added condition enables the use of words with random uppercase and lowercase chars
	for (auto& c : s) if (!islower(c)) c = tolower(c);
}
//================================================================================
// Converts all characters of ‘s’ to uppercase
void toupperStr(string& s)
// Because a pointer is used, a return is unecessary since the changes are occuring to the original variable
{
	// The added condition enables the use of words with random uppercase and lowercase chars
	for (auto& c : s) if (!isupper(c)) c = toupper(c);
}
//================================================================================
// ... other HELP FUNCTIONS
//================================================================================
//================================================================================
// Initializes the board, all cells with ‘.’
void initBoard(Board& board, int numLins, int numCols)
{
	board.boardCells = vector<vector<char>>(numLins, vector<char>(numCols, '.'));
	board.numLins = numLins;
	board.numCols = numCols;
	board.wordsOnBoard.resize(0); // Clear the Board everytime it initializes
}
//================================================================================
// Shows the board ‘b’ on the screen
// The line and column identifying letters are colored in red

string showBoard(const Board& b, bool saving)
{
	string boardString;
	// This is the only way I could make it to work because using either +' '+ or + RED + would add the ASCII value to that character instead of concatenating the strings 
	const string spc = " ";
	string red = RED;
	string no_cl = NO_COLOR;

	if (saving == true)
	{
		red = "";
		no_cl = "";
	}

	boardString += "\n";
	boardString += spc + spc;

	for (size_t j = 0; j < b.boardCells.at(0).size(); j++)
		boardString += red + char('a' + j) + no_cl + spc;

	boardString += "\n";

	for (size_t i = 0; i < b.boardCells.size(); i++)
	{
		boardString += red + char('A' + i) + no_cl + spc;

		for (size_t j = 0; j < b.boardCells.at(i).size(); j++)
			boardString += b.boardCells.at(i).at(j) + spc;

		boardString += "\n";
	}

	boardString += "\n";

	return boardString;
}

//================================================================================

void insertWords(Board& board, const WordList& wordList)
{
	
	string pos;
	WordPosition wordPos;
	bool validPos = false;
	bool inserted = false;

	while (!inserted)
	{
		// Insert the position of the word on board:

		cout << "Position (LcD / STOP): ";
		cin >> pos;

		string aux = pos;
		toupperStr(aux);

		if (aux == "STOP") {
			break;
		}

		//  In the ASCII encoding, uppercase letters 'A' to 'Z' have consecutive values from 65 to 90,
		//  and lowercase letters 'a' to 'z' have values from 97 to 122.
		if (pos.size() == 3 && // Checks if the string inputed is valid
			isupper(pos[0]) && board.numLins >= (pos[0] - 'A' + 1) &&
			islower(pos[1]) && board.numCols >= (pos[1] - 'a' + 1) &&
			isupper(pos[2]) && (pos[2] == 'H' || pos[2] == 'V'))
		{
			wordPos.lin = pos[0]; wordPos.col = pos[1]; wordPos.dir = pos[2];
			validPos = true;
		}
		else
		{
			cout << "\n\nInvalid position! Try again!\n" << endl;
		}

		// Insert the word on the board:

		string inpWord;

		// Checks if the inputed word is on the word list file ----------------------------------------------------------------------------

		bool isOnWordList = false;

		if (validPos)
		{
			cout << "\n\nINSERTING WORDS ...\n\n";

			cout << "Word: ";
			cin >> inpWord;

			for (const string& str : wordList)
				// "Const" in this context means the loop variable is read-only and cannot be modified within the loop
				// & is the pointer/reference, avoiding copying elements to another variable and using the original one
			{
				tolowerStr(inpWord);
				if (inpWord == str)
				{
					isOnWordList = true;
					toupperStr(inpWord);
					break;
				}
			}

			if (!isOnWordList) // After the loop has ended, if no word matched, display the message 
			{
				cout << "\nWord is not on the list! Try again!\n" << endl;
			}
		}

		// Checks if the inputed word is already on the board  ----------------------------------------------------------------------------

		bool boardEmpty = false;
		bool isOnBoard = true;

		if (isOnWordList)
		{
			if (!board.wordsOnBoard.empty())
			{
				for (const WordOnBoard& wordOnBoard : board.wordsOnBoard)
				{
					if (wordOnBoard.word == inpWord)
					{
						cout << "\nThis word is already placed on the Board. Try again!\n" << endl;
						break;
					}
					else
					{
						isOnBoard = false;
					}
				}

			}
			else
			{
				boardEmpty = true;
			}
		}

		// Checks if the inputed word is valid in the context of the board  ---------------------------------------------------------------

		WordOnBoard wordInserted;

		wordInserted.pos = wordPos;
		wordInserted.word = inpWord;

		if (!isOnBoard || boardEmpty)
		{

			string auxString;
			string clone = inpWord;

			switch (wordPos.dir)
			{
			case 'H':
			{
				if (inpWord.size() <= board.numCols - (wordPos.col - 'a'))
				// If the size of the word relative to the position it is inserted does not surpass the size of the board 
				{
					if (board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == '.' ||
						board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == inpWord[0])
					// If the board cell is empty (contains a dot) or if the first letter of the word is equal to a letter already placed on that cell's board
					{	
						for (int c = 0; c < inpWord.size(); c++)
						{
							auxString += board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + c];
							if (auxString[c] != '.')
							{
								clone[c] = auxString[c];
							}
						}
						if (clone == inpWord)
						{
							for (int c = 0; c < inpWord.size(); c++)
							{
								if (c == 0)
								{
									board.wordsOnBoard.push_back(wordInserted);
								}
								board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + c] = inpWord[c];
								inserted = true;
							}
						}
						else
						{
							cout << "\nInvalid position for the word. It does not match the other words on the board!\n" << endl;
						}
					}
					else 
					{
						cout << "\nInvalid position for the word\n" << endl;
					}
				}
				else
				{
					cout << "Word reaches the limits of the board! Try again!" << endl;
				}
				break;
			}

			case 'V':
				if (inpWord.size() <= board.numLins - (wordPos.lin - 'A'))
				{
					if (board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == '.' ||
						board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == inpWord[0])
						// If the board cell is empty (contains a dot) or if the first letter of the word is equal to a letter already placed on that cell's board
					{
						if (board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == '.' ||
							board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'] == inpWord[0])
							// If the board cell is empty (contains a dot) or if the first letter of the word is equal to a letter already placed on that cell's board
						{
							for (int c = 0; c < inpWord.size(); c++)
							{
								auxString += board.boardCells[wordPos.lin - 'A' + c][wordPos.col - 'a'];
								if (auxString[c] != '.')
								{
									clone[c] = auxString[c];
								}
							}
							if (clone == inpWord)
							{
								for (int c = 0; c < inpWord.size(); c++)
								{
									if (c == 0)
									{
										board.wordsOnBoard.push_back(wordInserted);
									}
									board.boardCells[wordPos.lin - 'A' + c][wordPos.col - 'a'] = inpWord[c];
									inserted = true;
								}
							}
							else
							{
								cout << "\nInvalid position for the word. It does not match the other words on the board!\n" << endl;
							}
						}
						else
						{
							cout << "\nInvalid position for the word.\n" << endl;
						}
					}
				else
				{
					cout << "Word reaches the limits of the board! Try again!" << endl;
				}
				break;
				}
			default: 
				break;
			}

		}
	}
}

void removeWords(Board& board) {
	while (1) {
		string wordRem;
		WordPosition wordPos;
		bool match = false;

		cout << "Word to remove: ";
		cin >> wordRem;

		toupperStr(wordRem);

		auto it = remove_if(board.wordsOnBoard.begin(), board.wordsOnBoard.end(),
			[wordRem](const WordOnBoard& w) {
				return wordRem == w.word;
			});

		if (it != board.wordsOnBoard.end()) {
			match = true;
			wordPos.lin = it->pos.lin;
			wordPos.col = it->pos.col;
			wordPos.dir = it->pos.dir;

			// Erase the elements from the vector
			board.wordsOnBoard.erase(it, board.wordsOnBoard.end());

			for (int c = 0; c < wordRem.size(); c++) {
				switch (wordPos.dir) {
				case 'H':
					board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + c] = '.';
					break;
				case 'V':
					board.boardCells[wordPos.lin - 'A' + c][wordPos.col - 'a'] = '.';
					break;
				}
			}
		}

		if (match) {
			break;
		}
		else {
			cout << "\nThe word you choose is not currently on the board!\n" << endl;
			break;
		}
	}
}


bool folderExists(const string& folderPath) {
	DWORD attributes = GetFileAttributesA(folderPath.c_str());
	return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

// Creates folder for Saved Games

void createFolder(const string& folderPath) 
{
#ifdef _WIN32  
	// This is a preprocessor directive used for conditional compilation in C and C++ that checks whether the code is being compiled for a Windows-based operating system. 
	//The _WIN32 macro is typically defined by Windows compilers, and its presence indicates that the code is being compiled on a Windows platform.
	if (!folderExists(folderPath)) {
		string command = "mkdir " + folderPath;
		int result = system(command.c_str());
	}
#else
#error "Unsupported platform"
#endif
}

vector<string> getTxtFilesInFolder(const string& folderPath) {
	vector<string> txtFiles;

	try {
		for (const auto& entry : fs::directory_iterator(folderPath)) {
			if (entry.is_regular_file() && entry.path().extension() == ".txt") {
				txtFiles.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << std::endl;
	}

	return txtFiles;
}

//================================================================================
// Saves ‘board’ into a text file

void saveBoard(const Board& board)
{
		
	string folder = "SavedGames";
	createFolder(folder);

	vector<string> txtFiles = getTxtFilesInFolder(folder);

	int numFiles = txtFiles.size();
	string numStr = to_string(numFiles);


	string fileNum;

	switch (numStr.size())
	{
	case 1:
		fileNum = "00" + numStr;
		break;
	case 2:
		fileNum = "0" + numStr;
		break;
	case 3:
		fileNum = numStr;
		break;
	default:
		fileNum = "000";
	}

	ofstream file(folder + "/b" + fileNum + ".txt", ios::app); // Use ios::app to append to the file
	bool saving = true;
	string str = "";

	if (file.is_open())
	{
		string boardOut = showBoard(board, saving);

		file << boardOut << endl;
		file << "\n\n";

		file << "Position: " << "\tWord:" << endl;
		file << "---------------------" << endl;
		for (const WordOnBoard& wob : board.wordsOnBoard)
		{
			file << wob.pos.lin + str + wob.pos.col + str + wob.pos.dir << "\t\t" + wob.word << endl;
		}

		file.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
	}

}
//================================================================================

WordList readWordsList() {

	// The user chooses the file containing the list of words 
	string file;

	while (1) // If the user inputs a invalid txt file, the program will continue to ask for a valid file
	{
		cout << "Words filename: ";
		cin >> file;
		WordList wordList;

		// Open the file
		ifstream inputFile(file);

		// Check if the file is open

		if (inputFile.is_open())
		{
			// Read each line of the file
			string line;
			while (getline(inputFile, line)) {
				wordList.push_back(line);
			}

			// Close the file
			inputFile.close();

			return wordList;
		}
		else
		{
			cerr << "Error opening the file! Enter a valid 'txt' file." << endl;
		}
	}
}

//================================================================================

bool readBoardSize(int numLins, int numCols) { // Reads Board size and outputs if it is valid or not in a boolean
	bool validsize = false;
	int minrange = 7; int maxrange = 15;
	if ((numLins >= minrange && numLins <= maxrange) && (numCols >= minrange && numCols <= maxrange))
		validsize = true;
	else
	{
		cout << "\nInvalid board size! The range should be within " << minrange << " and " << maxrange << " for both dimensions \n\n";
	}
	return validsize;
}

//=================================================================================

char readOption() { // Reads user game option and returns the choosen one among 4 
	cout << "H - Help " << endl << "I - Insert words" << endl << "R - Remove words" << endl << "E - Exit" << endl << "\nOption: ";

	char inserted;
	cin >> inserted;

	if (!isupper(inserted))
	{
		inserted = toupper(inserted);
	}

	return inserted;
}

//================================================================================

//================================================================================
void showHelp() {
	cout << "This is a word game in which the player has to insert the maximum number of words taken from a text file"
		" into a board, which has its size specified by the player himself."
		"The player is not allowed to create non-existent words while positioning them next to each other " << endl;

}
//================================================================================

int main()
{
	// Header start
	cout << "WORD GAMES - BOARD MAKER\n\n";

	// Read word list
	WordList availableWords = readWordsList();

	// Let the user choose the Board size
	bool validBoardSize = false;
	unsigned int numLins; unsigned int numCols;
	do // Prevents the code to continue if the board size is not valid
	{
		cout << "Board size (Lines Columns): ";
		cin >> numLins >> numCols;
		validBoardSize = readBoardSize(numLins, numCols);
	} while (!validBoardSize);

	unsigned numWords = availableWords.size();
	if (numWords > 0)
	{
		bool endOfOperations = false;
		Board board;
		bool saving = false;
		initBoard(board, numLins, numCols);
		string boardOut = showBoard(board, saving);
		cout << boardOut;

		do
		{
			char option = readOption();
			switch (option)
			{
			case 'H':
				showHelp();
				break;
			case 'I':
				insertWords(board, availableWords);
				boardOut = showBoard(board, saving);
				cout << boardOut;
				break;
			case 'R':
				removeWords(board);
				boardOut = showBoard(board, saving);
				cout << boardOut;
				break;
			case 'E':
				endOfOperations = true;
				boardOut = showBoard(board, saving);
				cout << endl;
				cout << "Saving the Board..." << endl;
				break;
			default: 
				cout << "\nWrong option! Try again!\n\n"; 
				break; 
			}
		} while (!endOfOperations);
		saveBoard(board);

	}
	return 0;

}

/*
if (((wordPos.lin - 'A' > 0 && wordPos.col - 'a' > 0 && wordPos.lin - 'A' < board.numLins - 1) &&
	(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.') &&
	(board.boardCells[wordPos.lin - 'A' + 1][wordPos.col - 'a'] == '.') &&
	(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.')) ||

	((wordPos.lin - 'A' == 0 && wordPos.col - 'a' > 0) &&
		(board.boardCells[wordPos.lin - 'A' + 1][wordPos.col - 'a'] == '.') &&
		(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.')) ||

	((wordPos.lin - 'A' == 0 && wordPos.col - 'a' == 0)) ||

	((wordPos.lin - 'A' == board.numLins - 1 && wordPos.col - 'a' > 0) &&
		(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.') &&
		(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.')) ||

	((wordPos.lin - 'A' == board.numLins - 1 && wordPos.col - 'a' == 0)) ||

	((wordPos.col - 'a' == 0 && wordPos.lin - 'A' > 0 && wordPos.lin - 'A' < board.numLins) &&
		(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.') &&
		(board.boardCells[wordPos.lin - 'A' + 1][wordPos.col - 'a'] == '.')))
{

if (((wordPos.lin - 'A' > 0 && wordPos.col - 'a' > 0 && wordPos.col - 'a' < board.numCols - 1) &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.') &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + 1] == '.') &&
								(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.')) ||

								((wordPos.col - 'a' == 0 && wordPos.lin - 'A' > 0) &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a'+1] == '.') &&
								(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.')) ||

								((wordPos.lin - 'A' == 0 && wordPos.col - 'a' == 0)) ||

								((wordPos.col - 'a' == board.numCols - 1 && wordPos.lin - 'A' > 0) &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.') &&
								(board.boardCells[wordPos.lin - 'A' - 1][wordPos.col - 'a'] == '.')) ||

								((wordPos.col - 'a' == board.numCols - 1 && wordPos.lin - 'A' == 0)) ||

								((wordPos.lin - 'A' == 0 && wordPos.col - 'a' > 0 && wordPos.col - 'a' < board.numCols) &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' - 1] == '.') &&
								(board.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + 1] == '.')))
							{


*/