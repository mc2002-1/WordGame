#include <iostream>
#include <vector>
#include <fstream>
#include <string>  
#include <cctype>
#include <cstdlib>  // For system() function
#include <limits>
#include <windows.h>
#include <filesystem>
#include <algorithm>  
#include <random> 


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

// ===================================== Custom variables =====================================

typedef struct
{
	char lin;
	char col;
	char dir;
} WordPosition;

typedef struct
{
	char lin;
	char col;
	char dir;
}CharsPosition;

typedef struct
{
	WordPosition pos;
	string word;
} WordsOnBoard;

typedef struct
{
	vector<CharsPosition> pos;
	vector<char> charWord;
} CharsOnBoard;

typedef struct {
	vector<vector<char>> boardCells;
	size_t numLins;
	size_t numCols;
	vector<WordsOnBoard> wordsOnBoard;
	vector<CharsOnBoard> charsOnBoard;
}BoardStruct;

typedef struct {
	char letter;
	CharsPosition pos;
} SingleChar;

typedef struct {
	vector<SingleChar> letters;
} BagOfLetters;


// ===================================== Functions =====================================

int read_nPlayers()
{
	// The user is asked the number of players that will play the game simultaneously

	int n_players;
	int min_n_players = 2; int max_n_players = 4; // Hardcoded range of players
	bool valid_n_players = false;

	do
	{
		cout << "\nNumber of players (2-4): ";
		cin >> n_players;

		if (n_players >= min_n_players && n_players <= max_n_players)
		{ valid_n_players = true; }
		else
		{ cout << "Error: Number of players is invalid!" << endl; }

	} while (!valid_n_players); // The loop will run until a valid number of players is inserted

	return n_players;
}

// ===================================== Classes =====================================

class Board {
private:
	pair<string, vector<string>> folder_file;
	string folder;
	vector<string> txtFiles;
	BoardStruct boardStruct;
	
	pair<string, vector<string>> getTxtFilesInFolder();
	BoardStruct readBoardFile(const string& folder, const vector<string>& txtFiles);

public:
	Board() 
	{
		// Initializing members in the constructor
		folder_file = getTxtFilesInFolder();
		folder = folder_file.first;
		txtFiles = folder_file.second;
		boardStruct = readBoardFile(folder, txtFiles);
	}

	BoardStruct& getBoardStruct();
	void showBoard(const BoardStruct& b);
};

class Bag {
private:
	BagOfLetters constructBag(BoardStruct& boardStruct);
	void shuffle(BagOfLetters& ActualBag); 
	BagOfLetters ActualBag;

public:
	Bag(BoardStruct& boardStruct) {
		ActualBag = constructBag(boardStruct);
		shuffle(ActualBag);
	}
	const BagOfLetters& getActualBag() const {
		return ActualBag;
	}
	bool isEmpty(const BagOfLetters& ActualBag);
	//void insert(BoardStruct& boardStruct, );
};

class Player
{
public:
	int id_player;
	string name;
	int id;
	pair<int, string> player_info;

	Player() // Construct
	{	
		player_info = create_player(id, name);
		id = player_info.first;
		name = player_info.second;
	}

	pair<int, string> create_player(int id, string name)
	{
		
	}
};

// ===================================== Board Class functions =====================================
// Lists the files on the folder with the saved games
pair <string, vector<string>> Board::getTxtFilesInFolder() {

	vector<string> txtFiles;
	string folder;

	while (1)
	{
		cout << "\nChoose a folder with saved board games: ";
		cin >> folder;

		if (fs::exists(folder))
		{
			cout << "\nFiles listed: " << endl;
			for (const auto& entry : fs::directory_iterator(folder)) {
				if (entry.is_regular_file() && entry.path().extension() == ".txt") {
					txtFiles.push_back(entry.path().string());
					cout << entry.path().filename().string() << endl;
				}
			}
			break;
		}
		else
		{
			cout << "\nThe folder does not exist. Make sure the right path is indicated!" << endl;
		}
	}
	return make_pair(folder, txtFiles);
}

// Reads the file chosen by the users and retrieves the saved game board information to reconstruct the board 
BoardStruct Board::readBoardFile(const string& folder, const vector<string>& txtFiles)
// Although the function is big and with a lot of identationa, it was meant this way in order to only loop through the tex
// file once to retrieve all the information regarding both the board and board characters, provinding an efficient code
{
	bool validFile = false;
	BoardStruct boardStruct;
	string boardFile;

	while (!validFile)
	{
		// Check if file to read is valid

		cout << "\nText file: ";
		cin >> boardFile;
		boardFile = folder + "\\" + boardFile; // Concatenating the folder to the string written by the user to form the path to the file

		const auto& it = find(txtFiles.begin(), txtFiles.end(), boardFile);

		if (it != txtFiles.end())
		{
			validFile = true;
		}
		else
		{
			cout << "\nThe file chosen is not valid." << endl;
		}

		// If the file is valid it will be read
		if (validFile)
		{
			vector<string> fileTxtArray;
			ifstream inputFile(boardFile); //Open file
			if (inputFile.is_open())
			{
				string line;
				while (getline(inputFile, line))
				{
					fileTxtArray.push_back(line); // Read each line
				}

				inputFile.close(); // Close file
			}
			else
			{
				cout << "Error reading file." << endl;
			}


			// Board size variables
			size_t numCol;
			size_t numLin;
			bool posStrFound = false;

			// Board words and corresponding position variables:
			WordsOnBoard wordsOnBoard;
			WordPosition wordPos;
			bool beginWordRead = false;

			// Loop through each line on the txt file
			for (string& fileLine : fileTxtArray)
			{
				// Retrive the Board size information:
				if (!posStrFound)
				{
					string posStr = fileLine.substr(fileLine.find_first_of(":") + 1, fileLine.size());
					size_t x_pos = posStr.find_first_of("x");

					if (x_pos != string::npos) /*string::npos is used to represent "no position". If x_pos is not equal to string::npos, it means that the substring "x" was found in the string.*/
					{
						numLin = stoi(posStr.substr(0, x_pos)); /*First argument of substr : start position | Second argument : Length of the string evaluated*/
						numCol = stoi(posStr.substr(x_pos + 1)); /*If no second argument is given, the substr will start from the positivion given on the argument to the end of the string*/
						boardStruct.numLins = numLin;
						boardStruct.numCols = numCol;
						boardStruct.boardCells = vector<vector<char>>(numLin, vector<char>(numCol, '.')); /*Initializing the board cells*/
						posStrFound = true;
					}
				}

				// Retrieve the words on the Board with their corresponding positions and populates its cells:
				if (beginWordRead)
				{

					// Words on the board:
					auto it = remove_if(fileLine.begin(), fileLine.end(), ::isspace);
					fileLine.erase(it, fileLine.end()); /*Removing the spaces between the word and its corresponding position*/

					wordsOnBoard.word = fileLine.substr(3);

					// Position Information:
					wordPos.lin = fileLine[0]; wordPos.col = fileLine[1]; wordPos.dir = fileLine[2];
					wordsOnBoard.pos = wordPos;

					// Populating the Board structure with the words on the file

					switch (wordPos.dir)
					{
					case 'H':
						for (int c = 0; c < wordsOnBoard.word.size(); c++)
						{
							boardStruct.boardCells[wordPos.lin - 'A'][wordPos.col - 'a' + c] = wordsOnBoard.word[c];
						}
						break;
					case 'V':
						for (int c = 0; c < wordsOnBoard.word.size(); c++)
						{
							boardStruct.boardCells[wordPos.lin - 'A' + c][wordPos.col - 'a'] = wordsOnBoard.word[c];
						}
						break;
					}

					boardStruct.wordsOnBoard.push_back(wordsOnBoard);
				}

				size_t begin = fileLine.find("-"); // Finds the '-' char for on the next iteration start to populate the cells and save the words on the structure
				if (begin != string::npos)
				{
					beginWordRead = true;
				}
			}
		}
	}
	return boardStruct;
}

BoardStruct& Board::getBoardStruct() {
	return boardStruct;
}

// Prints the Board
void Board::showBoard(const BoardStruct& b)
{
	cout << "\n ------ Board Game ------ " << endl;

	cout << endl;
	cout << BLUE << "  ";
	for (size_t j = 0; j < b.boardCells.at(0).size(); j++)
		cout << (char)('a' + j) << ' ';
	cout << endl;
	cout << NO_COLOR;
	for (size_t i = 0; i < b.boardCells.size(); i++)
	{
		cout << BLUE << (char)('A' + i) << ' ' << NO_COLOR;
		for (size_t j = 0; j < b.boardCells.at(i).size(); j++)
			cout << b.boardCells.at(i).at(j) << ' ';
		cout << endl;
	}
	cout << endl;
}

// ===================================== Bag Class functions =====================================

BagOfLetters Bag::constructBag(BoardStruct& boardStruct)
{


	for(const WordsOnBoard& words : boardStruct.wordsOnBoard)
	{
		// Word characters and corresponding position variables
		CharsOnBoard charsOnBoard;
		CharsPosition charsPosition;
		BagOfLetters ActualBag;

		size_t wordSize = words.word.size();

		charsOnBoard.charWord.reserve(wordSize); /* Reserve pre - allocates memory to the vector improving the efficiency of the code */
		charsOnBoard.pos.reserve(wordSize);
		ActualBag.letters.reserve(wordSize);

		// Decomposing the board words into each character and its position
		switch (words.pos.dir)
		{
		case 'H': // Horizontal direction
			for (int c = 0; c < wordSize; c++)
			{
				// Retrieve each character information from each word on the board
				charsOnBoard.charWord.push_back(words.word[c]); // Character
				charsPosition.lin = words.pos.lin; // Character line
				charsPosition.col = words.pos.col + c; // Character column

				// Checks whether the character is specific to a word or belongs to two words. If so, the + char references that in the char position
				if (words.pos.lin - 'A' > 0 && words.pos.lin - 'A' < boardStruct.numLins &&
					(boardStruct.boardCells[charsPosition.lin - 'A' - 1][charsPosition.col - 'a'] == '.' ||
						boardStruct.boardCells[charsPosition.lin - 'A' + 1][charsPosition.col - 'a'] == '.'))
				{
					charsPosition.dir = 'H';
				}
				else
				{
					charsPosition.dir = '+';
				}
				charsOnBoard.pos.push_back(charsPosition);
			}
			break;

		case 'V': // Vertical direction
			for (int c = 0; c < wordSize; c++)
			{
				// Reconstruct board cells for each word
				boardStruct.boardCells[words.pos.lin - 'A' + c][words.pos.col - 'a'] = words.word[c];

				// Retrieve each character information from each word on the board
				charsOnBoard.charWord.push_back(words.word[c]); // Character
				charsPosition.lin = words.pos.lin + c; // Character line
				charsPosition.col = words.pos.col; // Character column

				// Checks whether the character is specific to a word or belongs to two words. If so, the + char references that in the char position
				if (words.pos.col - 'a' > 0 && words.pos.col - 'a' < boardStruct.numCols &&
					(boardStruct.boardCells[charsPosition.lin - 'A'][charsPosition.col - 'a' - 1] == '.' ||
						boardStruct.boardCells[charsPosition.lin - 'A'][charsPosition.col - 'a' + 1] == '.'))
				{
					charsPosition.dir = 'V';
				}
				else
				{
					charsPosition.dir = '+';
				}
				charsOnBoard.pos.push_back(charsPosition);
			}
			break;
		}
		boardStruct.charsOnBoard.push_back(charsOnBoard);
		
	}
	for (const CharsOnBoard& word : boardStruct.charsOnBoard)
	{
		for (size_t i = 0; i < word.charWord.size(); ++i)
		{
			SingleChar singleChar;
			singleChar.letter = word.charWord[i];
			singleChar.pos = word.pos[i];

			ActualBag.letters.push_back(singleChar);
		}
	}
	return ActualBag;
}

void Bag::shuffle(BagOfLetters& ActualBag) {
	
	random_device rd;
	std::mt19937 g(rd());
	std::shuffle(ActualBag.letters.begin(), ActualBag.letters.end(), g);

}

bool Bag::isEmpty(const BagOfLetters& ActualBag) {
	
	bool empty = false;
	if (ActualBag.letters.empty() == true) {
		empty = true;
	}

	return empty;
}

int main()
{
	cout << "============== Multiplayer Board Game ==============" << endl;

	int nPlayers = read_nPlayers();

	Board board;

	/*
	Player player;
	Player game_players[4];
	
	string name;
	for (int i = 0; i < nPlayers; i++)
	{
		cout << "Player " << i+1 << " name: " << endl;
		cin >> name;
		game_players[i] = player.create_player(i, name);
	}
	 */

	BoardStruct& boardStruct = board.getBoardStruct();
	board.showBoard(boardStruct);

	Bag bag(boardStruct);
	const BagOfLetters& actualBag = bag.getActualBag();
	

	for (const CharsOnBoard& word : boardStruct.charsOnBoard)
	{
		cout << "-----------------" << endl;
		for (int i = 0; i < word.charWord.size(); i++)
		{		
			cout << word.charWord[i] << "  " << word.pos[i].lin << word.pos[i].col << word.pos[i].dir << endl;
		}	
	}
	
	return 0;

}
