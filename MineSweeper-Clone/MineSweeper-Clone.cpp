#include <bits/stdc++.h>
using namespace std;

const int BEGINNER = 0;
const int INTERMEDIATE = 1;
const int ADVANCED = 2;
const int MAXSIZE = 30;

int ROWS;
int COLUMNS;
int MINES;

class MinesWeeperGame
{
public:
	void chooseDifficultyLevel()
	{
		int level;
		cout << "Enter the Difficulty Level" << endl;
		cout << "Press 0 for BEGINNER     (9  * 9  cells and 10 mines)" << endl;
		cout << "Press 1 for INTERMEDIATE (16 * 16 cells and 40 mines)" << endl;
		cout << "Press 2 for ADVANCED     (16 * 30 cells and 99 mines)" << endl;

		cin >> level;

		if (level == BEGINNER)
		{
			ROWS = 9;
			COLUMNS = 9;
			MINES = 10;
		}

		if (level == INTERMEDIATE)
		{
			ROWS = 16;
			COLUMNS = 16;
			MINES = 40;
		}

		if (level == ADVANCED)
		{
			ROWS = 16;
			COLUMNS = 30;
			MINES = 99;
		}
	}

	void clearBoards(char mineBoard[][MAXSIZE], char gameBoard[][MAXSIZE])
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				gameBoard[i][j] = mineBoard[i][j] = '.';
			}
		}
	}

	void placeMines(char mineBoard[][MAXSIZE], int mines)
	{
		int placed = 0;
		while (placed < mines)
		{
			int random = rand() % (ROWS * COLUMNS);
			int row = random / COLUMNS;
			int col = random % ROWS;
			if (mineBoard[row][col] == '#')
			{
				continue; // already a mine
			}
			mineBoard[row][col] = '#';
			placed++;
		}
	}

	// replace the mine in (row, col) and put it to a vacant space
	void replaceMine(int row, int col, char mineBoard[][MAXSIZE])
	{
		placeMines(mineBoard, 1);  // add a new mine
		mineBoard[row][col] = '.'; // remove the old one
		return;
	}

	char indexToChar(int index)
	{
		if (index < 10)
			return index + '0';
		else
			return 'a' + (index - 10);
	}

	int charToIndex(char ch)
	{
		if (ch <= '9')
			return ch - '0';
		else
			return (ch - 'a') + 10;
	}

	void displayBoard(char gameBoard[][MAXSIZE])
	{
		// top line
		cout << "    ";
		for (int i = 0; i < COLUMNS; i++)
		{
			cout << indexToChar(i) << ' ';
		}
		cout << endl << endl;

		// rows
		for (int i = 0; i < ROWS; i++)
		{
			cout << indexToChar(i) << "   ";
			for (int j = 0; j < COLUMNS; j++)
			{
				cout << gameBoard[i][j] << " ";
			}
			cout << "  " << indexToChar(i);
			cout << endl;
		}

		// bottom line
		cout << endl << "    ";
		for (int i = 0; i < COLUMNS; i++)
		{
			cout << indexToChar(i) << ' ';
		}
		cout << endl;
	}

	bool isValid(int row, int col)
	{
		return (row >= 0) && (row < ROWS) && (col >= 0) && (col < COLUMNS);
	}

	bool isMine(int row, int col, char board[][MAXSIZE])
	{
		return (board[row][col] == '#');
	}

	// return vector of all neighbours of row, col
	vector < pair <int, int> > getNeighbours(int row, int col)
	{
		vector < pair <int, int> > neighbours;

		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				if (dx != 0 || dy != 0)
				{
					if (isValid(row + dx, col + dy))
					{
						neighbours.push_back(make_pair(row + dx, col + dy));
					}
				}
			}
		}
		return neighbours;
	}

	// count the number of mines in the adjacent cells
	int countAdjacentMines(int row, int col, char mineBoard[][MAXSIZE])
	{
		vector < pair <int, int> > neighbours = getNeighbours(row, col);

		int count = 0;
		for (int i = 0; i < neighbours.size(); i++)
		{
			if (isMine(neighbours[i].first, neighbours[i].second, mineBoard))
			{
				count++;
			}
		}
		return count;
	}

	void uncoverBoard(char gameBoard[][MAXSIZE], char mineBoard[][MAXSIZE], int row, int col, int* nMoves)
	{
		(*nMoves)++;
		int count = countAdjacentMines(row, col, mineBoard);
		gameBoard[row][col] = count + '0';

		// if cell number == 0, then uncover all the neighboring cells. (only uncover if neighbour == '.')
		if (count == 0)
		{
			vector < pair <int, int> > neighbours = getNeighbours(row, col);
			for (int i = 0; i < neighbours.size(); i++)
			{
				if (gameBoard[neighbours[i].first][neighbours[i].second] == '.')
				{
					uncoverBoard(gameBoard, mineBoard, neighbours[i].first, neighbours[i].second, nMoves);
				}
			}
		}
	}

	void markMines(char gameBoard[][MAXSIZE], char mineBoard[][MAXSIZE], bool won)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				if (gameBoard[i][j] == '.' && mineBoard[i][j] == '#')
				{
					if (won)
					{
						gameBoard[i][j] = 'F';
					}
					else
					{
						gameBoard[i][j] = '#';
					}
				}
			}
		}
	}

	void playMinesweeper()
	{
		// initialization
		char mineBoard[MAXSIZE][MAXSIZE], gameBoard[MAXSIZE][MAXSIZE];
		int nMovesTotal = ROWS * COLUMNS - MINES;
		int flags = MINES;
		clearBoards(mineBoard, gameBoard);
		placeMines(mineBoard, MINES);

		// lets play!
		int nMoves = 0;
		bool gameOver = false;

		while (!gameOver)
		{
			displayBoard(gameBoard);
			cout << flags << " flags left" << endl << endl;

			// take input
			char x, y, z;
			cout << "Enter your move, (row, column, safe(s)/flag(f)) -> ";
			cin >> x >> y >> z;
			cout << endl;

			int row = charToIndex(x);
			int col = charToIndex(y);

			if (nMoves == 0)
			{
				if (isMine(row, col, mineBoard))
				{
					replaceMine(row, col, mineBoard);
				}
			}

			if (z == 's')
			{
				if (gameBoard[row][col] == '.' && mineBoard[row][col] == '.')
				{
					uncoverBoard(gameBoard, mineBoard, row, col, &nMoves);
					if (nMoves == nMovesTotal)
					{
						markMines(gameBoard, mineBoard, true);
						displayBoard(gameBoard);
						cout << endl << "You won!!! :)" << endl;
						gameOver = true;
					}
				}
				else if (gameBoard[row][col] == '.' && mineBoard[row][col] == '#')
				{
					// game over
					gameBoard[row][col] = '#';
					markMines(gameBoard, mineBoard, false);
					displayBoard(gameBoard);
					cout << endl << "You lost! :(" << endl;
					gameOver = true;
				}
				else
				{
					// illegal move
					cout << "Illegal move. ";
					if (gameBoard[row][col] == 'F')
						cout << "Cell is a flag. Use f to toggle flag off. ";
					else
						cout << "Cell is already a number. ";
					cout << endl;
				}
			}

			if (z == 'f')
			{
				if (gameBoard[row][col] == '.')
				{
					if (flags != 0)
					{
						gameBoard[row][col] = 'F';
						flags--;
					}
					else
					{
						cout << "Illegal move. Too many flags!" << endl;
					}
				}
				else if (gameBoard[row][col] == 'F')
				{
					// undo a flag
					gameBoard[row][col] = '.';
					flags++;
				}
				else
				{
					// illegal move
					cout << "Illegal move. Cell is a number. " << endl;
				}
			}
		}
	}
};

int main()
{
	MinesWeeperGame* mw = new MinesWeeperGame();
	srand(time(NULL));
	mw->chooseDifficultyLevel();
	mw->playMinesweeper();
}