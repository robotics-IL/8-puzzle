#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <mutex>
#include <cstring>

const short MAX_PUZZLE_SIDE_SIZE = 50;

//Ian's Standard Tools
#define Benchmark() Timer timer
struct Timer
{
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	//std::chrono::duration<float> duration;
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
		//start = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());

	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		std::cout << "duration " << std::chrono::duration<double>(end - start).count() * 1000 << " ms\n";
		//end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());
		//std::cout << setprecision(10) << fixed << showpoint;
		//std::cout << "duration " << std::chrono::duration<double>(end - start).count() << " nano" << "\n";

	}

	float Duration()
	{
		end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<float>(end - start).count() * 1000;
	}
};
struct Index
{
	short int num = 0;
	std::string name = " ";
};

//Structures specific to this program
struct EightProblemBox
{
	//short num = 0;
	short movesAwayi = 0;
	short movesAwayj = 0;
	short desiredi = 0;
	short desiredj = 0;
};

//Functions specific to this program
short manualEnterBoxCheck(std::string lineOut, int eightProblem[][MAX_PUZZLE_SIDE_SIZE], short boxNum, const short& side, const int& problemSize);
void eightProblemMiniMax(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], const short& side, const int& problemSize, short& depth);
//short miniMaxThreads(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
//	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched);
//short miniMaxThread(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
//	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched);
short miniMax(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched);

//Ian's Standard Tools
void timePunch(Index timePunchMenuIndexAlphabetical[], std::string timePunchMenuIndexNumerical[], int& timePunchMenuSize);
time_t manuallyEnterNewTime();
void addToTimePunchArrays(time_t timePunch, std::string outOrInString, std::string timePunchString[], std::string timePunchString2[],
	unsigned long timePunchUnsignedLong[], unsigned long timePunchUnsignedLong2[], int& timePunchFileSize);
void addToTimePunchFile(const unsigned long& timePunchTotalTimeSec, std::string timePunchString2[],
	unsigned long timePunchUnsignedLong2[], const int& timePunchFileSize);
int getFileArraySize(std::string fileName);
void getAlphabeticalIndex(std::string fileName, Index index[]);
void getNumericalIndex(std::string filename, std::string index[]);
std::string* addToNumericalFileAndArray(std::string newAddition, std::string indexNumerical[], int& initialArraySize,
	std::string fileNumericalName);
Index* addToAlphabeticalFileAndArray(std::string newAddition, Index indexAlphabetical[], int& initialArraySize,
	std::string fileAlphabeticalName);
short int getAnswerFromMenu(std::string typeOfMenu, Index indexAlphabetical[], int initialArraySize);
short int inputCheckShortInt(std::string lineOut);
bool inputCheckTrueFalse(std::string);

int main()
{
	//Ian's Standard Tools
	int initialMenuSize = getFileArraySize("InitialMenuIndexNumerical.txt");
	int timePunchMenuSize = getFileArraySize("TimePunchMenuIndexNumerical.txt");
	
	std::string* initialMenuIndexNumerical = new std::string[initialMenuSize];
	Index* initialMenuIndexAlphabetical = new Index[initialMenuSize];
	std::string* timePunchMenuIndexNumerical = new std::string[timePunchMenuSize];
	Index* timePunchMenuIndexAlphabetical = new Index[timePunchMenuSize];

	getNumericalIndex("InitialMenuIndexNumerical.txt", initialMenuIndexNumerical);
	getAlphabeticalIndex("InitialMenuIndexAlphabetical.txt", initialMenuIndexAlphabetical);
	getNumericalIndex("TimePunchMenuIndexNumerical.txt", timePunchMenuIndexNumerical);
	getAlphabeticalIndex("TimePunchMenuIndexAlphabetical.txt", timePunchMenuIndexAlphabetical);

	//Ian's Standard Tools - Menu code
	int initialMenuAnswer = 0;
	do
	{
		std::cout << "\n\n";
		initialMenuAnswer = getAnswerFromMenu("Initial", initialMenuIndexAlphabetical, initialMenuSize);
		switch (initialMenuAnswer)
		{
		case 999:			//Add to Initial Menu
		{
			std::string newInitialMenuOption;
			std::cin.get();
			std::cout << "Please enter a NEW Initial Menu Option : ";
			getline(std::cin, newInitialMenuOption);
			initialMenuIndexNumerical = addToNumericalFileAndArray(newInitialMenuOption, initialMenuIndexNumerical,
				initialMenuSize, "InitialMenuIndexNumerical.txt");
			initialMenuIndexAlphabetical = addToAlphabeticalFileAndArray(newInitialMenuOption,
				initialMenuIndexAlphabetical, initialMenuSize, "InitialMenuIndexAlphabetical.txt");
			initialMenuSize++;
			break;
		}
		case 1://End
		{
			break;
		}
		case 2://Time Punch
		{
			timePunch(timePunchMenuIndexAlphabetical, timePunchMenuIndexNumerical, timePunchMenuSize);
			break;
		}
		case 3://MiniMax Algorithm for Eight Problem
		{	
			//gets size of one side of the problem
			short side = 0;
			bool valid = false;
			std::cout << "\n";
			do
			{
				side = inputCheckShortInt("\nPlease enter the size of one side of the problem (so 3 for a 3x3 8 problem) : ");
				if (side < 2 || side > MAX_PUZZLE_SIDE_SIZE)
					std::cout << "***** Error : The size must be between 2 and 100 *****";
				else valid = true;
			} while (!valid);
			int problemSize = side * side - 1;

			//gets depth of the miniMax function
			short depth = 0;
			valid = false;
			std::cout << "\n";
			do
			{
				depth = inputCheckShortInt("\nPlease enter the depth for the miniMax function : ");
				if (depth < 1 || depth > 10000)
					std::cout << "***** Error : The depth must be between 1 and 10000 *****";
				else valid = true;
			} while (!valid);

			//Initializes Menu 
			int eightProblemMenuSize = getFileArraySize("EightProblemMenuIndexNumerical.txt");

			std::string* eightProblemMenuIndexNumerical = new std::string[eightProblemMenuSize];
			Index* eightProblemMenuIndexAlphabetical = new Index[eightProblemMenuSize];

			getNumericalIndex("EightProblemMenuIndexNumerical.txt", eightProblemMenuIndexNumerical);
			getAlphabeticalIndex("EightProblemMenuIndexAlphabetical.txt", eightProblemMenuIndexAlphabetical);

			//Eight Problem Menu
			int eightProblemMenuAnswer = 0;
			std::cout << "\n\n";
			eightProblemMenuAnswer = getAnswerFromMenu("Eight Problem", eightProblemMenuIndexAlphabetical, eightProblemMenuSize);
			switch (eightProblemMenuAnswer)
			{
			case 999://Add to Eight Problem Menu
			{
				std::string newEightProblemMenuOption;
				std::cin.get();
				std::cout << "Please enter a NEW Eight Problem Menu Option : ";
				getline(std::cin, newEightProblemMenuOption);
				eightProblemMenuIndexNumerical = addToNumericalFileAndArray(newEightProblemMenuOption, eightProblemMenuIndexNumerical,
					eightProblemMenuSize, "EightProblemMenuIndexNumerical.txt");
				eightProblemMenuIndexAlphabetical = addToAlphabeticalFileAndArray(newEightProblemMenuOption,
					eightProblemMenuIndexAlphabetical, eightProblemMenuSize, "EightProblemMenuIndexAlphabetical.txt");
				eightProblemMenuSize++;
				break;
			}
			case 1://Enter Starting Eight Problem
			{				
				int eightProblem[MAX_PUZZLE_SIDE_SIZE][MAX_PUZZLE_SIDE_SIZE] = { 0 };
				EightProblemBox boxes[MAX_PUZZLE_SIDE_SIZE * MAX_PUZZLE_SIDE_SIZE];
				std::cout << "\nThe Problem Box's are like this : \n";
				for (int i = 0, j = 0, k = 1; i < side; i++)
				{
					for (j = 0; j < side; j++, k++)
					{
						std::cout << k << " ";
					}
					std::cout << "\n";
				}
				std::cout << "\n";
				std::cout << "Enter 0 for an empty box and 1-" << problemSize << " for the starting numbers in the boxes : \n";
				for (int i = 0, j = 0, k = 1; i < side; i++)
				{
					for (j = 0; j < side; j++, k++)
					{
						std::string lineOut = "Enter Box " + std::to_string(k) + std::string(" : ");
						eightProblem[i][j] = manualEnterBoxCheck(lineOut, eightProblem, k, side, problemSize);
					}
				}

				{
					Benchmark();
					eightProblemMiniMax(eightProblem, boxes, side, problemSize, depth);
				}
				break;
			}
			case 2://Random Starting Eight Problem
			{
				int eightProblem[MAX_PUZZLE_SIDE_SIZE][MAX_PUZZLE_SIDE_SIZE] = { 0 };
				EightProblemBox boxes[MAX_PUZZLE_SIDE_SIZE * MAX_PUZZLE_SIDE_SIZE];
				srand(time(NULL));
				//fill box num
				for (int i = 0, j = 0, k = 1; i < side; i++)
				{
					for (j = 0; j < side; j++, k++)
					{
						if (k != problemSize + 1)
							eightProblem[i][j] = k;
						else
							eightProblem[i][j] = 0;
					}
				}	
				//shuffles the box numbers
				short i = side - 1;
				short j = side - 1;
				for (int k = 0; k < problemSize * problemSize; k++)
				{
					short random = 0;
					random = rand() % 4;
					if (random == 0 && i != 0) //move up
					{
						short temp = eightProblem[i][j];
						eightProblem[i][j] = eightProblem[i - 1][j];
						eightProblem[i - 1][j] = temp;
						i--;
					}
					else if (random == 1 && j != side - 1) //move right
					{
						short temp = eightProblem[i][j];
						eightProblem[i][j] = eightProblem[i][j + 1];
						eightProblem[i][j + 1] = temp;
						j++;
					}
					else if (random == 2 && i != side - 1) //move down
					{
						short temp = eightProblem[i][j];
						eightProblem[i][j] = eightProblem[i + 1][j];
						eightProblem[i + 1][j] = temp;
						i++;
					}
					else if (random == 3 && j != 0) //move left
					{
						short temp = eightProblem[i][j];
						eightProblem[i][j] = eightProblem[i][j - 1];
						eightProblem[i][j - 1] = temp;
						j--;
					}
				}
				//int inversions = 0;
				//do
				//{
				//	inversions = 0;
				//	for (int i = side -1, j = side - 1, k = 0; i >= 0 && k < problemSize; i--)
				//	{
				//		for (j = side - 1; j >= 0 && k < problemSize; j--, k++)
				//		{
				//			random = rand() % (k + problemSize);
				//			for (int i2 = 0, j2 = 0, l = 0; i2 < side && l <= random; i2++)
				//			{
				//				for (j2 = 0; j2 < side && l <= random; j2++, l++)
				//				{
				//					if (l == random)
				//					{
				//						short temp = eightProblem[i][j].num;
				//						eightProblem[i][j].num = eightProblem[i2][j2].num;
				//						eightProblem[i2][j2].num = temp;
				//					}
				//				}
				//			}
				//		}
				//	}
				//	for (int i = 0, k = 0; i < side; i++) //find inversions
				//	{
				//		for (int j = 0; j < side; j++, k++)
				//		{
				//			for (int i2 = k / side; i2 < side; i2++)
				//			{
				//				if (i2 == k / side)
				//				{
				//					for (int j2 = k % side; j2 < side; j2++)
				//					{
				//						std::cout << eightProblem[i][j].num << " " << eightProblem[i2][j2].num;
				//						if (eightProblem[i][j].num > eightProblem[i2][j2].num)
				//						{
				//							inversions++;
				//							std::cout << "inversion";
				//						}
				//						std::cout << "\n";
				//					}
				//				}
				//				else
				//				{
				//					for (int j2 = 0; j2 < side; j2++)
				//					{
				//						std::cout << eightProblem[i][j].num << " " << eightProblem[i2][j2].num;
				//						if (eightProblem[i][j].num > eightProblem[i2][j2].num)
				//						{
				//							inversions++;
				//							std::cout << "inversion";
				//						}
				//						std::cout << "\n";
				//					}
				//				}
				//			}								
				//		}
				//	}
				//	std::cout << "inversions : " << inversions << "\n";
				//} while (inversions % 2);


				{
					Benchmark();
					eightProblemMiniMax(eightProblem, boxes, side, problemSize, depth);
				}
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	} while (initialMenuAnswer != 1);
}

//Functions specific to this program
short manualEnterBoxCheck(std::string lineOut, int eightProblem[][MAX_PUZZLE_SIDE_SIZE], short boxNum, const short& side, const int& problemSize)
{
	short input = -1;
	bool valid = true;
	do
	{
		input = inputCheckShortInt(lineOut);
		valid = true;
		if (input > problemSize || input < 0)
		{
			valid = false;
			std::cout << "***** Error : Number must be between 0 and " << problemSize << " *****\n";
		}
		for (int i = 0, j = 0, k = 0; i < side && k < boxNum - 1; i++)
		{
			for (j = 0; j < side && k < boxNum - 1; j++, k++)
			{
				if (input == eightProblem[i][j])
				{
					valid = false;
					std::cout << "***** Error : You cannot enter a duplicate Number *****\n";
				}
			}
		}
	} while (!valid);
	return input;
}

void eightProblemMiniMax(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], const short& side, const int& problemSize, short& depth)
{
	//displays the starting problem
	std::cout << "\nThe Starting Problem is : \n\n";
	for (int i = 0, j = 0, k = 0; i < side; i++)
	{
		for (j = 0; j < side; j++, k++)
		{
			std::cout << eightProblem[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	//searches for the box 0 (the empty box)
	//and determines movesLeft and the box's desiredi, desiredj, and movesAway variables
	short zeroi = 0;
	short zeroj = 0;
	short movesLeft = 0;
	{
		//Benchmark();
		for (int i = 0, j = 0; i < side; i++)
		{
			for (j = 0; j < side; j++)
			{
				if (eightProblem[i][j] == 0)
				{
					zeroi = i;
					zeroj = j;
				}
				else
				{
					boxes[eightProblem[i][j]].desiredi = (eightProblem[i][j] - 1) / (side);
					boxes[eightProblem[i][j]].desiredj = (eightProblem[i][j] - 1) % (side);
					boxes[eightProblem[i][j]].movesAwayi = abs(i - boxes[eightProblem[i][j]].desiredi);
					boxes[eightProblem[i][j]].movesAwayj = abs(j - boxes[eightProblem[i][j]].desiredj);
					movesLeft += boxes[eightProblem[i][j]].movesAwayi + boxes[eightProblem[i][j]].movesAwayj;
					//std::cout << "num " << eightProblem[i][j].num << " i " << i << " j " << j << " tempi " << tempi << " tempj " << tempj  << " Moves " << eightProblem[i][j].movesAway << "\n";
				}
			}
		}
	}

	int numMoves = 1;
	int totalNumSearched = 0;
	if (movesLeft)
	{
		short* moves = new short[depth * 2] { -1 };
		short* movesTemp = new short[depth * 2] { -1 };
		int maxTempMoves = depth * 2;
		int maxMoves = depth * 2;
		bool solved = false;
		short leastMovesLeft = movesLeft;
		do
		{
			int numSearched = 0;
			short tempMovesLeft = 0;
			short minDepth = 0;
			std::cout << "Moves Left : " << movesLeft << "\n";
			do
			{
				if (!solved)
				{
					//dynamically allocates more memory for the moves arrays
					//always keeps depth unused space in the array
					if (numMoves + depth == maxMoves)
					{
						short* temp = new short[maxMoves * 2]{ 0 };
						for (int i = 0; i < maxMoves; i++)
							temp[i] = moves[i];
						delete[] moves;
						moves = temp;
						maxMoves = maxMoves * 2;
					}	
					if (depth == maxTempMoves)
					{
						short* temp = new short[depth * 2] {0};
						delete[]movesTemp;
						movesTemp = temp;
						maxTempMoves = depth * 2;
					}
					tempMovesLeft = movesLeft;
					moves[numMoves] = miniMax(eightProblem, boxes, zeroi, zeroj, tempMovesLeft, minDepth, depth, depth, side, moves, movesTemp, numMoves, moves[numMoves - 1], solved, numSearched);
					if (solved)
						std::cout << "found Solution\n";
				}
				else
					break;
				if (tempMovesLeft >= leastMovesLeft && !solved)
				{
					depth++;
					std::cout << "depth increased : " << depth << "\n";
				}
				else //if (tempMovesLeft < leastMovesLeft || leastMovesLeft == 0)
				{
					leastMovesLeft = tempMovesLeft;
					break;
				}
			} while (true);
			if (moves[numMoves] == 0)
			{
				//moves box above
				short movesAwayiBefore = boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi;
				int temp = eightProblem[zeroi - 1][zeroj];
				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
				//changes movesLeft
				movesLeft  += boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
				zeroi -= 1;
			}
			else if (moves[numMoves] == 1)
			{
				//moves box right
				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj;
				int temp = eightProblem[zeroi][zeroj + 1];
				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
				//changes movesLeft
				movesLeft += boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
				zeroj += 1;
			}
			else if (moves[numMoves] == 2)
			{
				//moves box down
				short movesAwayiBefore = boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi;
				int temp = eightProblem[zeroi + 1][zeroj];
				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
				//changes movesLeft
				movesLeft += boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
				zeroi += 1;
			}
			else //if (moves[numMoves] == 3)
			{
				//moves box left
				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj;
				int temp = eightProblem[zeroi][zeroj - 1];
				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
				//changes movesLeft
				movesLeft += boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
				zeroj -= 1;
			}
			std::cout << moves[numMoves] << "->\n";
			for (int i = 0, j = 0, k = 0; i < side; i++)
			{
				for (j = 0; j < side; j++, k++)
				{
					std::cout << eightProblem[i][j] << " ";
				}
				std::cout << "\n";
			}
			std::cout << "searched : " << numSearched << "\n\n";
			totalNumSearched += numSearched;
			numMoves++;
		} while (movesLeft && numMoves < problemSize * problemSize * problemSize * problemSize);
		if (numMoves >= problemSize * problemSize * problemSize * problemSize)
			std::cout << "NOT solved\n";
	}
	if(!movesLeft)
		std::cout << "solved\n";
	std::cout << "Moves : " << numMoves - 1 << "\n";
	std::cout << "Total Searched : " << totalNumSearched << "\n\n";
}

//short miniMaxThreads(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
//	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched)
//{
//	//for (int i = 0, j = 0, k = 0; i < side; i++)
//	//{
//	//	for (j = 0; j < side; j++, k++)
//	//	{
//	//		std::cout << eightProblem[i][j].num << " ";
//	//	}
//	//	std::cout << "\n";
//	//}
//	//std::cout << "\n";
//	if (depth > minDepth && depth > maxDepth - 2)
//	{
//		short move = 0;
//		short possibleMoves[4] = { 0 };
//		short checkMove[4] = { 0, 1, 2, 3 };
//		for (int k = 3; k > 0; k--)
//		{
//			short random = rand() % (k);
//			short temp = checkMove[k];
//			checkMove[k] = checkMove[random];
//			checkMove[random] = temp;
//		}
//		short leastMovesLeft = movesLeft;
//		//short bestMovesLeft = movesLeft;
//		short numPossibleMoves = 0;
//		bool solvedTemp = false;
//		for (int m = 0; m < 4; m++)
//		{
//			//determines possible boxes that can move into the empty box
//			if (previousMove != 2 && zeroi != 0 && minDepth < depth && checkMove[m] == 0) //box is above and previous move wasn't below
//			{
//				possibleMoves[numPossibleMoves++] = 0;
//				movesTemp[depth - 1] = 0;
//				//movies the box that is above, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//				if (movesLeftTemp)
//					miniMaxThreads(eightProblem, boxes, zeroi - 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 0, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				//moves the box that is above, down into the empty box
//				temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 3 && zeroj != side - 1 && minDepth < depth && checkMove[m] == 1) //box is right and previous move wasn't left
//			{
//				possibleMoves[numPossibleMoves++] = 1;
//				movesTemp[depth - 1] = 1;
//				//moves the box that is right, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMaxThreads(eightProblem, boxes, zeroi, zeroj + 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 1, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 0 && zeroi != side - 1 && minDepth < depth && checkMove[m] == 2) //box is below and previous move wasn't above
//			{
//				possibleMoves[numPossibleMoves++] = 2;
//				movesTemp[depth - 1] = 2;
//				//movies the box that is below, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//				if (movesLeftTemp)
//					miniMaxThreads(eightProblem, boxes, zeroi + 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 2, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 1 && zeroj != 0 && minDepth < depth && checkMove[m] == 3) //box is left and previous move wasn't right
//			{
//				possibleMoves[numPossibleMoves++] = 3;
//				movesTemp[depth - 1] = 3;
//				//movies the box that is left, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMaxThreads(eightProblem, boxes, zeroi, zeroj - 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 3, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//		}
//		if (depth != maxDepth)
//			movesLeft = leastMovesLeft;
//		return possibleMoves[move];
//	}
//	else if (depth > minDepth && depth == maxDepth - 2) //make threads when 2 depth deep // aka up to 9 threads can be made
//	{
//		short move = 0;
//		short possibleMoves[4] = { 0 };
//		short checkMove[4] = { 0, 1, 2, 3 };
//		for (int k = 3; k > 0; k--)
//		{
//			short random = rand() % (k);
//			short temp = checkMove[k];
//			checkMove[k] = checkMove[random];
//			checkMove[random] = temp;
//		}
//		short leastMovesLeft = movesLeft;
//		//short bestMovesLeft = movesLeft;
//		short numPossibleMoves = 0;
//		bool solvedTemp = false;
//		
//		//make threads
//		if (previousMove == 0)
//		{
//			int eightProblem1[MAX_PUZZLE_SIDE_SIZE][MAX_PUZZLE_SIDE_SIZE] = { 0 };
//			int eightProblem2[MAX_PUZZLE_SIDE_SIZE][MAX_PUZZLE_SIDE_SIZE] = { 0 };
//			int eightProblem3[MAX_PUZZLE_SIDE_SIZE][MAX_PUZZLE_SIDE_SIZE] = { 0 };
//			EightProblemBox boxes1[MAX_PUZZLE_SIDE_SIZE * MAX_PUZZLE_SIDE_SIZE];
//			EightProblemBox boxes2[MAX_PUZZLE_SIDE_SIZE * MAX_PUZZLE_SIDE_SIZE];
//			EightProblemBox boxes3[MAX_PUZZLE_SIDE_SIZE * MAX_PUZZLE_SIDE_SIZE];
//			for (int i = 0, k = 0; i < side; i++)
//			{
//				for (int j = 0; j < side; j++, k++)
//				{
//					eightProblem1[i][j] = eightProblem[i][j];
//					eightProblem2[i][j] = eightProblem[i][j];
//					eightProblem3[i][j] = eightProblem[i][j];
//					boxes1[k] = boxes[k];
//					boxes2[k] = boxes[k];
//					boxes3[k] = boxes[k];
//				}
//			}
//			short movesLeftTemp1 = movesLeft;
//			short movesLeftTemp2 = movesLeft;
//			short movesLeftTemp3 = movesLeft;
//			short minDepth1 = minDepth;
//			short minDepth2 = minDepth;
//			short minDepth3 = minDepth;
//			//moves
//			bool solved1 = solved;
//			bool solved2 = solved;
//			bool solved3 = solved;
//			int numSearched1 = numSearched;
//			int numSearched2 = numSearched;
//			int numSearched3 = numSearched;
//
//			//std::thread a(miniMaxThread, eightProblem1, boxes1, zeroi - 1, zeroj, movesLeftTemp1, minDepth1, depth, maxDepth, side, moves, movesTemp, numMoves, previousMove, solved1, numSearched1);
//		}
//		else if (previousMove == 1)
//		{
//
//		}
//		else if (previousMove == 2)
//		{
//
//		}
//		else if (previousMove == 3)
//		{
//
//		}
//
//		
//		for (int m = 0; m < 4; m++)
//		{
//			//determines possible boxes that can move into the empty box
//			if (previousMove != 2 && zeroi != 0 && minDepth < depth && checkMove[m] == 0) //box is above and previous move wasn't below
//			{
//
//
//				possibleMoves[numPossibleMoves++] = 0;
//				movesTemp[depth - 1] = 0;
//				//movies the box that is above, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//
//				if (movesLeftTemp)
//					miniMaxThreads(eightProblem, boxes, zeroi - 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 0, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				//moves the box that is above, down into the empty box
//				temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 3 && zeroj != side - 1 && minDepth < depth && checkMove[m] == 1) //box is right and previous move wasn't left
//			{
//				possibleMoves[numPossibleMoves++] = 1;
//				movesTemp[depth - 1] = 1;
//				//moves the box that is right, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi, zeroj + 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 1, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 0 && zeroi != side - 1 && minDepth < depth && checkMove[m] == 2) //box is below and previous move wasn't above
//			{
//				possibleMoves[numPossibleMoves++] = 2;
//				movesTemp[depth - 1] = 2;
//				//movies the box that is below, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi + 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 2, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 1 && zeroj != 0 && minDepth < depth && checkMove[m] == 3) //box is left and previous move wasn't right
//			{
//				possibleMoves[numPossibleMoves++] = 3;
//				movesTemp[depth - 1] = 3;
//				//movies the box that is left, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi, zeroj - 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 3, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//		}
//		if (depth != maxDepth)
//		movesLeft = leastMovesLeft;
//		return possibleMoves[move];
//	}
//	else
//		return -1; //hit the max depth or a better solution was found
//}
//
//short miniMaxThread(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
//	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched)
// {
//	//for (int i = 0, j = 0, k = 0; i < side; i++)
//	//{
//	//	for (j = 0; j < side; j++, k++)
//	//	{
//	//		std::cout << eightProblem[i][j].num << " ";
//	//	}
//	//	std::cout << "\n";
//	//}
//	//std::cout << "\n";
//	if (depth > minDepth)
//	{
//		short move = 0;
//		short possibleMoves[4] = { 0 };
//		short checkMove[4] = { 0, 1, 2, 3 };
//		for (int k = 3; k > 0; k--)
//		{
//			short random = rand() % (k);
//			short temp = checkMove[k];
//			checkMove[k] = checkMove[random];
//			checkMove[random] = temp;	
//		}
//		short leastMovesLeft = movesLeft;
//		//short bestMovesLeft = movesLeft;
//		short numPossibleMoves = 0;
//		bool solvedTemp = false;
//		for (int m = 0; m < 4; m++)
//		{
//			//determines possible boxes that can move into the empty box
//			if (previousMove != 2 && zeroi != 0 && minDepth < depth && checkMove[m] == 0) //box is above and previous move wasn't below
//			{
//				possibleMoves[numPossibleMoves++] = 0;
//				movesTemp[depth - 1] = 0;
//				//movies the box that is above, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi - 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 0, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				//moves the box that is above, down into the empty box
//				temp = eightProblem[zeroi - 1][zeroj];
//				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 3 && zeroj != side - 1 && minDepth < depth && checkMove[m] == 1) //box is right and previous move wasn't left
//			{
//				possibleMoves[numPossibleMoves++] = 1;
//				movesTemp[depth - 1] = 1;
//				//moves the box that is right, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi, zeroj + 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 1, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj + 1];
//				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 0 && zeroi != side - 1 && minDepth < depth && checkMove[m] == 2) //box is below and previous move wasn't above
//			{
//				possibleMoves[numPossibleMoves++] = 2;
//				movesTemp[depth - 1] = 2;
//				//movies the box that is below, down into the empty box
//				short movesAwayiBefore = boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi;
//				int temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
//				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi + 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 2, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi + 1][zeroj];
//				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi = movesAwayiBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//			else if (previousMove != 1 && zeroj != 0 && minDepth < depth && checkMove[m] == 3) //box is left and previous move wasn't right
//			{
//				possibleMoves[numPossibleMoves++] = 3;
//				movesTemp[depth - 1] = 3;
//				//movies the box that is left, down into the empty box
//				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj;
//				int temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
//				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
//				//changes movesLeft
//				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
//				if (movesLeftTemp)
//					miniMax(eightProblem, boxes, zeroi, zeroj - 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 3, solved, numSearched);
//				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
//				{
//					if (minDepth < depth)
//						minDepth = depth;
//					solvedTemp = true;
//					solved = true;
//					for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
//					{
//						moves[k] = movesTemp[l];
//					}
//					//std::cout << "solved\n";
//				}
//				if (movesLeftTemp < leastMovesLeft || solvedTemp)
//				{
//					move = numPossibleMoves - 1;
//					leastMovesLeft = movesLeftTemp;
//				}
//				//moves the box back into origional position
//				temp = eightProblem[zeroi][zeroj - 1];
//				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
//				eightProblem[zeroi][zeroj] = temp;
//				boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj = movesAwayjBefore;
//				if (depth == 1 || solvedTemp)
//					numSearched++;
//			}
//		}
//		if(depth != maxDepth)
//			movesLeft = leastMovesLeft;
// 		return possibleMoves[move];
//	}
//	else
//		return -1; //hit the max depth or a better solution was found
//}

short miniMax(int eightProblem[][MAX_PUZZLE_SIDE_SIZE], EightProblemBox boxes[], short zeroi, short zeroj, short& movesLeft, short& minDepth, short depth, const short& maxDepth, const short& side,
	short moves[], short movesTemp[], const int& numMoves, short previousMove, bool& solved, int& numSearched)
{
	//for (int i = 0, j = 0, k = 0; i < side; i++)
	//{
	//	for (j = 0; j < side; j++, k++)
	//	{
	//		std::cout << eightProblem[i][j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n";
	if (minDepth < depth)
	{
		short move = 0;
		short possibleMoves[4] = { 0 };
		/*short checkMove[4] = { 0, 1, 2, 3 };
		for (int k = 3; k > 0; k--)
		{
			short random = rand() % (k);
			short temp = checkMove[k];
			checkMove[k] = checkMove[random];
			checkMove[random] = temp;
		}*/
		short leastMovesLeft = movesLeft;
		//short bestMovesLeft = movesLeft;
		short numPossibleMoves = 0;
		//for (int m = 0; m < 4; m++)
		//{
			//determines possible boxes that can move into the empty box
			if (previousMove != 2 && zeroi != 0 /*&& minDepth < depth && checkMove[m] == 0*/) //box is above and previous move wasn't below
			{
				possibleMoves[numPossibleMoves++] = 0;
				movesTemp[depth - 1] = 0;
				//movies the box that is above, down into the empty box
				short movesAwayiBefore = boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi;
				int temp = eightProblem[zeroi - 1][zeroj];
				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
				//changes movesLeft
				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
				bool solvedTemp = false;
				if (movesLeftTemp)
					miniMax(eightProblem, boxes, zeroi - 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 0, solvedTemp, numSearched);
				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
				{
					solved = true;
					if(!solvedTemp) //only do during the depth that the solution was found
					{
						minDepth = depth;
						solvedTemp = true;
						for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
						{
							moves[k] = movesTemp[l];
						}	
					}
					//std::cout << "solved\n";
				}
				if (movesLeftTemp < leastMovesLeft || solvedTemp)
				{
					move = numPossibleMoves - 1;
					leastMovesLeft = movesLeftTemp;
				}
				//else if (!leastMovesLeft && );
				//moves the box back into origional position
				//moves the box that is above, down into the empty box
				temp = eightProblem[zeroi - 1][zeroj];
				eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi - 1][zeroj]].movesAwayi = movesAwayiBefore;
				if (depth == 1 || solvedTemp)
					numSearched++;
			}
			/*else*/ if (previousMove != 3 && zeroj != side - 1 && minDepth < depth /*&& checkMove[m] == 1 */) //box is right and previous move wasn't left
			{
				possibleMoves[numPossibleMoves++] = 1;
				movesTemp[depth - 1] = 1;
				//moves the box that is right, down into the empty box
				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj;
				int temp = eightProblem[zeroi][zeroj + 1];
				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
				//changes movesLeft
				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
				bool solvedTemp = false;
				if (movesLeftTemp)
					miniMax(eightProblem, boxes, zeroi, zeroj + 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 1, solvedTemp, numSearched);
				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
				{
					solved = true;
					if (!solvedTemp) //only do during the depth that the solution was found
					{
						minDepth = depth;
						solvedTemp = true;
						for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
						{
							moves[k] = movesTemp[l];
						}
					}
					//std::cout << "solved\n";
				}
				if (movesLeftTemp < leastMovesLeft || solvedTemp)
				{
					move = numPossibleMoves - 1;
					leastMovesLeft = movesLeftTemp;
				}
				//moves the box back into origional position
				temp = eightProblem[zeroi][zeroj + 1];
				eightProblem[zeroi][zeroj + 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj + 1]].movesAwayj = movesAwayjBefore;
				if (depth == 1 || solvedTemp)
					numSearched++;
			}
			/*else*/ if (previousMove != 0 && zeroi != side - 1 && minDepth < depth /*&& checkMove[m] == 2*/) //box is below and previous move wasn't above
			{
				possibleMoves[numPossibleMoves++] = 2;
				movesTemp[depth - 1] = 2;
				//movies the box that is below, down into the empty box
				short movesAwayiBefore = boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi;
				int temp = eightProblem[zeroi + 1][zeroj];
				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayi = abs(zeroi - boxes[eightProblem[zeroi][zeroj]].desiredi);
				//short movesAwayiAfter = eightProblem[zeroi][zeroj].movesAwayi;
				//changes movesLeft
				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayi - movesAwayiBefore;
				bool solvedTemp = false;
				if (movesLeftTemp)
					miniMax(eightProblem, boxes, zeroi + 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 2, solvedTemp, numSearched);
				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
				{
					solved = true;
					if (!solvedTemp) //only do during the depth that the solution was found
					{
						minDepth = depth;
						solvedTemp = true;
						for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
						{
							moves[k] = movesTemp[l];
						}
					}
					//std::cout << "solved\n";
				}
				if (movesLeftTemp < leastMovesLeft || solvedTemp)
				{
					move = numPossibleMoves - 1;
					leastMovesLeft = movesLeftTemp;
				}
				//moves the box back into origional position
				temp = eightProblem[zeroi + 1][zeroj];
				eightProblem[zeroi + 1][zeroj] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi + 1][zeroj]].movesAwayi = movesAwayiBefore;
				if (depth == 1 || solvedTemp)
					numSearched++;
			}
			/*else*/ if (previousMove != 1 && zeroj != 0 && minDepth < depth /*&& checkMove[m] == 3*/) //box is left and previous move wasn't right
			{
				possibleMoves[numPossibleMoves++] = 3;
				movesTemp[depth - 1] = 3;
				//movies the box that is left, down into the empty box
				short movesAwayjBefore = boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj;
				int temp = eightProblem[zeroi][zeroj - 1];
				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj]].movesAwayj = abs(zeroj - boxes[eightProblem[zeroi][zeroj]].desiredj);
				//short movesAwayjAfter = eightProblem[zeroi][zeroj].movesAwayj;
				//changes movesLeft
				short movesLeftTemp = movesLeft + boxes[eightProblem[zeroi][zeroj]].movesAwayj - movesAwayjBefore;
				bool solvedTemp = false;
				if (movesLeftTemp)
					miniMax(eightProblem, boxes, zeroi, zeroj - 1, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 3, solvedTemp, numSearched);
				if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
				{
					solved = true;
					if (!solvedTemp) //only do during the depth that the solution was found
					{
						minDepth = depth;
						solvedTemp = true;
						for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)
						{
							moves[k] = movesTemp[l];
						}
					}
					//std::cout << "solved\n";
				}
				if (movesLeftTemp < leastMovesLeft || solvedTemp)
				{
					move = numPossibleMoves - 1;
					leastMovesLeft = movesLeftTemp;
				}
				//moves the box back into origional position
				temp = eightProblem[zeroi][zeroj - 1];
				eightProblem[zeroi][zeroj - 1] = eightProblem[zeroi][zeroj];
				eightProblem[zeroi][zeroj] = temp;
				boxes[eightProblem[zeroi][zeroj - 1]].movesAwayj = movesAwayjBefore;
				if (depth == 1 || solvedTemp)
					numSearched++;
			}
		//}
		//if (depth != maxDepth)
			movesLeft = leastMovesLeft;
		return possibleMoves[move];
	}
	else
		return -1; //hit the max depth or a better solution was found
}

/* Saving old code incase I need it
possibleMoves[numPossibleMoves++] = 0;
movesTemp[depth - 1] = 0;
//movies the box that is above, down into the empty box
//short movesAwayTemp = eightProblem[zeroi - 1][zeroj].movesAway;
//movesLeftTemp -= movesAwayTemp;
short tempiBefore = (eightProblem[zeroi][zeroj].num - 1) / (side);
EightProblemBox temp = eightProblem[zeroi - 1][zeroj];
eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
eightProblem[zeroi][zeroj] = temp;
//changes movesLeft
short tempiAfter = (eightProblem[zeroi][zeroj].num - 1) / (side);
//short tempj = (eightProblem[zeroi][zeroj].num - 1) % (side);
//eightProblem[zeroi][zeroj].movesAway = abs(zeroi - tempi);
//eightProblem[zeroi][zeroj].movesAway += abs(zeroj - tempj);
//eightProblem[zeroi][zeroj].movesAway -= tempiBefore - tempiAfter;
//movesLeftTemp += eightProblem[zeroi][zeroj].movesAway;
short movesLeftTemp = movesLeft - tempiBefore + tempiAfter;
if (movesLeftTemp)
miniMax(eightProblem, zeroi - 1, zeroj, movesLeftTemp, minDepth, depth - 1, maxDepth, side, moves, movesTemp, numMoves, 0, solved);
if (!movesLeftTemp) // movesLeftTemp == 0 and a solution has been found
{
	if (minDepth < depth)
		minDepth = depth;
	solvedTemp = true;
	solved = true;
	for (int k = numMoves, l = maxDepth - 1; l >= depth - 1; k++, l--)//k < numMoves + maxDepth - depth + 1
	{
		moves[k] = movesTemp[l];
	}
	//std::cout << "solved\n";
}
if (movesLeftTemp < leastMovesLeft || solvedTemp)
{
	move = numPossibleMoves - 1;
	leastMovesLeft = movesLeftTemp;
	//bestMovesLeft = mleft;
}
//moves the box back into origional position
//moves the box that is above, down into the empty box
temp = eightProblem[zeroi - 1][zeroj];
eightProblem[zeroi - 1][zeroj] = eightProblem[zeroi][zeroj];
eightProblem[zeroi][zeroj] = temp;
//changes movesAway
//eightProblem[zeroi - 1][zeroj].movesAway = movesAwayTemp;
//tempi = (eightProblem[zeroi - 1][zeroj].num - 1) / (side);
//tempj = (eightProblem[zeroi - 1][zeroj].num - 1) % (side);
//eightProblem[zeroi - 1][zeroj].movesAway = abs(zeroi - 1 - tempi);
//eightProblem[zeroi - 1][zeroj].movesAway += abs(zeroj - tempj);
*/

//Ian's Standard Tools
void timePunch(Index timePunchMenuIndexAlphabetical[], std::string timePunchMenuIndexNumerical[], int& timePunchMenuSize)
{
	int timePunchAnswerFromMenu = getAnswerFromMenu("Time Punch", timePunchMenuIndexAlphabetical,
		timePunchMenuSize);
	if (timePunchAnswerFromMenu == 999)
	{
		std::string newTimePunchMenuOption;
		std::cin.get();
		std::cout << "Please enter a NEW Time Punch Menu Option : ";
		getline(std::cin, newTimePunchMenuOption);
		timePunchMenuIndexNumerical = addToNumericalFileAndArray(newTimePunchMenuOption, timePunchMenuIndexNumerical,
			timePunchMenuSize, "TimePunchMenuIndexNumerical.txt");
		timePunchMenuIndexAlphabetical = addToAlphabeticalFileAndArray(newTimePunchMenuOption,
			timePunchMenuIndexAlphabetical, timePunchMenuSize, "TimePunchMenuIndexAlphabetical.txt");
		timePunchMenuSize++;
	}
	else if (timePunchAnswerFromMenu >= 1 && timePunchAnswerFromMenu <= 4)
	{
		int timePunchFileSize = 0, timePunchTotalTimeSec = 0;
		std::ifstream inTimePunchFile;
		inTimePunchFile.open("TimePunch.txt");
		if (inTimePunchFile)
		{
			inTimePunchFile >> timePunchFileSize;
			inTimePunchFile >> timePunchTotalTimeSec;
			std::string* timePunchString = new std::string[timePunchFileSize];
			//unsigned long temp = 0;
			unsigned long* timePunchUnsignedLong = new unsigned long[timePunchFileSize];
			for (int i = 0; i < timePunchFileSize; i++)
			{
				inTimePunchFile.get();
				std::getline(inTimePunchFile, timePunchString[i]);
				//std::cout << "String : " << timePunchString[i] << "\n";
				inTimePunchFile >> timePunchUnsignedLong[i];
				//std::cout << "long : " << timePunchUnsignedLong[i] << "\n";
				//if (i % 2 == 1) temp += timePunchUnsignedLong[i] - timePunchUnsignedLong[i - 1];
				//std::cout << temp << "\n";
			}
			inTimePunchFile.close();
			//timePunchTotalTimeSec = temp;
			if (timePunchAnswerFromMenu == 1)// user tries to clock IN
			{
				if (timePunchString[timePunchFileSize - 1][0] == 'o') //last time punch was to clock OUT, so clock IN happens automatically
				{
					std::string* timePunchString2 = new std::string[timePunchFileSize + 1];
					unsigned long* timePunchUnsignedLong2 = new unsigned long[timePunchFileSize + 1];
					time_t nowIN = std::time(0);
					addToTimePunchArrays(nowIN, "in at ", timePunchString, timePunchString2, timePunchUnsignedLong, timePunchUnsignedLong2, timePunchFileSize);
					//timePunchTotalTimeSec doesn't change
					addToTimePunchFile(timePunchTotalTimeSec, timePunchString2, timePunchUnsignedLong2, timePunchFileSize);
					delete[] timePunchString2;
					delete[] timePunchUnsignedLong2;
				}
				else //timePunchString[timePunchFileSize - 1][0] == 'i' //last time punch was to clock IN, so need to clock OUT first manually
				{
					std::cout << "Your last time punch was to punch IN\n";
					bool punchOUT = inputCheckTrueFalse("Would you like to enter a time OUT? (t or f): ");
					if (punchOUT)
					{
						std::string* timePunchString2 = new std::string[timePunchFileSize + 1];
						unsigned long* timePunchUnsignedLong2 = new unsigned long[timePunchFileSize + 1];
						time_t manualTimeOUT = manuallyEnterNewTime();
						addToTimePunchArrays(manualTimeOUT, "out at m ", timePunchString, timePunchString2, timePunchUnsignedLong, timePunchUnsignedLong2, timePunchFileSize);
						timePunchTotalTimeSec += timePunchUnsignedLong2[timePunchFileSize - 1] - timePunchUnsignedLong2[timePunchFileSize - 2];
						addToTimePunchFile(timePunchTotalTimeSec, timePunchString2, timePunchUnsignedLong2, timePunchFileSize);
						delete[] timePunchString2;
						delete[] timePunchUnsignedLong2;
					}
				}
			}
			else if (timePunchAnswerFromMenu == 2)//user tries to clock OUT
			{
				if (timePunchString[timePunchFileSize - 1][0] == 'i') //last time punch was to clock IN, so clock OUT happens automatically
				{
					std::string* timePunchString2 = new std::string[timePunchFileSize + 1];
					unsigned long* timePunchUnsignedLong2 = new unsigned long[timePunchFileSize + 1];
					time_t nowOUT = std::time(0);
					addToTimePunchArrays(nowOUT, "out at ", timePunchString, timePunchString2, timePunchUnsignedLong, timePunchUnsignedLong2, timePunchFileSize);
					timePunchTotalTimeSec += timePunchUnsignedLong2[timePunchFileSize - 1] - timePunchUnsignedLong2[timePunchFileSize - 2];
					addToTimePunchFile(timePunchTotalTimeSec, timePunchString2, timePunchUnsignedLong2, timePunchFileSize);
					delete[] timePunchString2;
					delete[] timePunchUnsignedLong2;
				}
				else //timePunchString[timePunchFileSize - 1][0] == 'o' //last time punch was to clock OUT, so need to clock IN first manually
				{
					std::cout << "Your last time punch was to punch OUT\n";
					bool punchIN = inputCheckTrueFalse("Would you like to enter a time IN? (t or f): ");
					if (punchIN)
					{
						std::string* timePunchString2 = new std::string[timePunchFileSize + 1];
						unsigned long* timePunchUnsignedLong2 = new unsigned long[timePunchFileSize + 1];
						time_t manualTimeIN = manuallyEnterNewTime();
						addToTimePunchArrays(manualTimeIN, "in at m ", timePunchString, timePunchString2, timePunchUnsignedLong, timePunchUnsignedLong2, timePunchFileSize);
						//timePunchTotalTimeSec doesn't change
						addToTimePunchFile(timePunchTotalTimeSec, timePunchString2, timePunchUnsignedLong2, timePunchFileSize);
						delete[] timePunchString2;
						delete[] timePunchUnsignedLong2;
					}
				}
			}
			else if (timePunchAnswerFromMenu == 3) //user wants to know their last punch
			{
				std::cout << "\nYour last time punch was : " << timePunchString[timePunchFileSize - 1];
			}
			else if (timePunchAnswerFromMenu == 4) //user wants to edit a previous punch
			{
				int countTo = timePunchFileSize / 10, timePunchToChange = -1, lowBound = -1;
				if (timePunchFileSize % 10 != 0)
					countTo++;
				std::cout << "\n\n     Time Punches Menu\n";
				std::cout << "9999 Exit Menu\n";
				std::cout << "8888 Show More Punches\n";
				for (int counter = 1; counter <= countTo; counter++)
				{
					int i = -1;
					for (i = timePunchFileSize - ((counter - 1) * 10) - 1; i > timePunchFileSize - (counter * 10) - 1 && i >= 0; i--)
					{
						std::cout << i << " " << timePunchString[i] << "\n";
					}
					do
					{
						timePunchAnswerFromMenu = inputCheckShortInt("Please enter the punch number you would like to change : ");
						lowBound = timePunchFileSize - (counter * 10);
						if (i == -1)
							lowBound = 0;
						if ((timePunchAnswerFromMenu < timePunchFileSize - (counter * 10) && timePunchAnswerFromMenu >= 0) ||
							(timePunchAnswerFromMenu >= timePunchFileSize && timePunchAnswerFromMenu != 9999 && timePunchAnswerFromMenu != 8888))
							std::cout << "***** Error : Must enter a number bettween " << lowBound << " & " << timePunchFileSize - 1 << " or equal to 8888 or 9999 ******\n";
						if (timePunchAnswerFromMenu == 8888 && i == -1)
						{
							std::cout << "***** Error : there are no more punches ******\n";
						}
					} while ((timePunchAnswerFromMenu < timePunchFileSize - (counter * 10) && timePunchAnswerFromMenu >= 0) ||
						(timePunchAnswerFromMenu >= timePunchFileSize && timePunchAnswerFromMenu != 9999 && timePunchAnswerFromMenu != 8888) || (timePunchAnswerFromMenu == 8888 && i == -1));
					if (timePunchAnswerFromMenu != 8888)
						break;
				}
				if (timePunchAnswerFromMenu != 9999)
				{
					//  72 in at Thu Aug 20 14:54 : 04 2020
					//	71 out at Thu Aug 20 14 : 53 : 03 2020
					timePunchToChange = timePunchAnswerFromMenu;
					time_t manualTime = 0;
					do
					{
						manualTime = manuallyEnterNewTime();
						if (timePunchUnsignedLong[timePunchToChange + 1] - timePunchUnsignedLong[timePunchToChange - 1] <= 60) //this will ignore the errors if the time bettween the previous time punch and the time punch ahead are within 60 seconds of eachother
							break;
						else if (manualTime - timePunchUnsignedLong[timePunchToChange - 1] < 0)
							std::cout << "\n***** Error : The time you entered was before time punch " << timePunchToChange - 1 << " *****\n";
						else if (timePunchUnsignedLong[timePunchToChange + 1] - manualTime < 0)
							std::cout << "\n***** Error : The time you entered was after time punch " << timePunchToChange + 1 << " *****\n";
					} while (manualTime - timePunchUnsignedLong[timePunchToChange - 1] < 0 || timePunchUnsignedLong[timePunchToChange + 1] - manualTime < 0);

					if (timePunchString[timePunchToChange][0] == 'o')
					{
						timePunchTotalTimeSec += manualTime - timePunchUnsignedLong[timePunchToChange];
					}
					char str[26];
					timePunchUnsignedLong[timePunchToChange] = manualTime;
					ctime_s(str, sizeof str, &manualTime);
					std::string outOrInString;
					if (timePunchString[timePunchToChange][0] == 'o')
					{
						outOrInString = "out at m ";
					}
					else//timePunchString[timePunchToChange][0] = 'i'
						outOrInString = "in at m ";
					timePunchString[timePunchToChange] = outOrInString + std::string(str);

					std::ofstream outTimePunchFile;
					outTimePunchFile.open("TimePunch.txt");
					outTimePunchFile << timePunchFileSize << "\n";
					outTimePunchFile << timePunchTotalTimeSec;
					for (int i = 0; i < timePunchToChange; i++)
					{
						//std::cout << "\n" << timePunchString2[i] << "\n";
						//std::cout << timePunchUnsignedLong2[i];
						outTimePunchFile << "\n" << timePunchString[i] << "\n";
						outTimePunchFile << timePunchUnsignedLong[i];
					}
					outTimePunchFile << "\n" << timePunchString[timePunchToChange]; // str has a "\n" at the end
					outTimePunchFile << timePunchUnsignedLong[timePunchToChange];
					for (int i = timePunchToChange + 1; i < timePunchFileSize; i++)
					{
						//std::cout << "\n" << timePunchString2[i] << "\n";
						//std::cout << timePunchUnsignedLong2[i];
						outTimePunchFile << "\n" << timePunchString[i] << "\n";
						outTimePunchFile << timePunchUnsignedLong[i];
					}
					outTimePunchFile.close();
					std::cout << "\nYou have punched " << timePunchToChange << " " << timePunchString[timePunchFileSize - 1] << "\n";
					std::cout << "The Total Time to date is " << timePunchTotalTimeSec / 3600.0 << "h\n";
				}

			}
		}
		else
			std::cout << "\n***** Error: The \"TimePunch.txt\" File didn't open *****\n";
	}
	else
		std::cout << "\n***** Error: You entered an answer that didn't go anywhere *****\n";
}
time_t manuallyEnterNewTime()
{
	tm time;
	short int mday = 0;
	std::cout << "\n";
	do
	{
		mday = inputCheckShortInt("Day of month? (1-31) : ");
		if (mday < 1 || mday > 31)
			std::cout << "***** Error : Must be between 1 and 31 *****\n";
	} while (mday < 1 || mday > 31);
	time.tm_mday = mday;
	short int mon = 0;
	do
	{
		mon = inputCheckShortInt("Month? (1-12) : ");
		if (mon < 1 || mon > 12)
			std::cout << "***** Error : Must be between 1 and 12 *****\n";
	} while (mon < 1 || mon > 12);
	time.tm_mon = mon - 1;
	short int year = 0;
	do
	{
		year = inputCheckShortInt("Year? (2020+) : ");
		if (year < 2020)
			std::cout << "***** Error : Must be 2020 or more *****\n";
	} while (year < 2020);
	time.tm_year = year - 1900;
	short int hour = -1;
	do
	{
		hour = inputCheckShortInt("Hour? (0 - 23) : ");
		if (hour < 0 || hour  > 23)
			std::cout << "***** Error : Must be between 0 and 23 *****\n";
	} while (hour < 0 || hour > 23);
	time.tm_hour = hour;
	short int minute = 0;
	do
	{
		minute = inputCheckShortInt("Minute? (0 - 59) : ");
		if (minute < 0 || minute  > 59)
			std::cout << "***** Error : Must be between 0 and 59 *****\n";
	} while (minute < 0 || minute  > 59);
	time.tm_min = minute;
	time.tm_sec = 0;
	return(mktime(&time));
}
void addToTimePunchArrays(time_t timePunch, std::string outOrInString, std::string timePunchString[], std::string timePunchString2[],
	unsigned long timePunchUnsignedLong[], unsigned long timePunchUnsignedLong2[], int& timePunchFileSize)
{
	for (int i = 0; i < timePunchFileSize; i++)
	{
		timePunchString2[i] = timePunchString[i];
	}
	delete[] timePunchString;
	for (int i = 0; i < timePunchFileSize; i++)
	{
		timePunchUnsignedLong2[i] = timePunchUnsignedLong[i];
	}
	delete[] timePunchUnsignedLong;
	char str[26];
	timePunchUnsignedLong2[timePunchFileSize] = timePunch;
	ctime_s(str, sizeof str, &timePunch);
	timePunchString2[timePunchFileSize] = outOrInString + std::string(str);
	timePunchFileSize++;
	//for (int i = 0; i < timePunchFileSize; i++)
	//{
	//	std::cout << "\n" << timePunchString2[i] << "\n";
	//	std::cout << timePunchUnsignedLong2[i];
	//}
}
void addToTimePunchFile(const unsigned long& timePunchTotalTimeSec, std::string timePunchString2[],
	unsigned long timePunchUnsignedLong2[], const int& timePunchFileSize)
{
	std::ofstream outTimePunchFile;
	outTimePunchFile.open("TimePunch.txt");
	outTimePunchFile << timePunchFileSize << "\n";
	outTimePunchFile << timePunchTotalTimeSec;
	for (int i = 0; i < timePunchFileSize - 1; i++)
	{
		//std::cout << "\n" << timePunchString2[i] << "\n";
		//std::cout << timePunchUnsignedLong2[i];
		outTimePunchFile << "\n" << timePunchString2[i] << "\n";
		outTimePunchFile << timePunchUnsignedLong2[i];
	}
	outTimePunchFile << "\n" << timePunchString2[timePunchFileSize - 1]; // str has a "\n" at the end
	outTimePunchFile << timePunchUnsignedLong2[timePunchFileSize - 1];
	outTimePunchFile.close();
	std::cout << "\nYou have punched " << timePunchString2[timePunchFileSize - 1] << "\n";
	std::cout << "The Total Time to date is " << timePunchTotalTimeSec / 3600.0 << "h\n";
}
int getFileArraySize(std::string fileName)  // gets the size of the file by reading the first thing in the file
{
	std::ifstream inIndexFile;
	inIndexFile.open(fileName); //opens file
	if (inIndexFile)			// checks if the file opened
	{
		int size = 0;
		inIndexFile >> size;
		inIndexFile.close();  //closes file
		//std::cout << "You got the " << fileName << " file size" << "\n";
		return size;
	}
	else
	{
		std::cout << "\n\n***** Error: The " << fileName << "File didn't open *****\n";
		return 0;
	}
};
void getAlphabeticalIndex(std::string fileName, Index index[]) //reads the file which contains alphabetically ordered strings and their index number and adds to an array
{
	std::ifstream inIndexFile;
	inIndexFile.open(fileName);
	if (inIndexFile)			//checks if file opened
	{
		int numCards = 0;
		while (!inIndexFile.eof())
		{
			inIndexFile >> index[numCards].num;			//this reads the next field in the file and stores it in an array
			inIndexFile.get();
			getline(inIndexFile, index[numCards].name);  //reads until end of line
			numCards++;
			//std::cout << numCards << " ";
		}
		//std::cout << index[0].name << " 0 name ";
		inIndexFile.close();
		//std::cout << "The " << fileName << " file was got and closed" << "\n";
	}
	else
		std::cout << "\n\n***** Error: The " << fileName << " File didn't open *****\n";
}
void getNumericalIndex(std::string fileName, std::string index[]) //reads the file which contains strings ordered by user input and adds to an array
{
	std::ifstream inIndexFile;
	inIndexFile.open(fileName);
	if (inIndexFile)			//checks if file opened
	{
		int numCards = 0;
		int size;
		inIndexFile >> size;		// reads the expected first int in the file
		inIndexFile.get();
		while (!inIndexFile.eof())
		{
			getline(inIndexFile, index[numCards]);  //reads strings from file and adds to an array
			numCards++;
			//std::cout << fileName << " numCards : " << numCards << " ";
		}
		//std::cout << index[0].name << " 0 name ";
		inIndexFile.close();
		//std::cout << "The Numerical Index "<< fileName << " has been got" << "\n";
	}
	else
		std::cout << "\n\n***** Error: The " << fileName << " File didn't open *****\n";
}
std::string* addToNumericalFileAndArray(std::string newAddition, std::string indexNumerical[], int& initialArraySize,
	std::string fileNumericalName)
{
	std::string* newNumericalArray = new std::string[initialArraySize + 1];

	for (int j = 0; j < initialArraySize; j++)
	{
		newNumericalArray[j] = indexNumerical[j];
	}
	newNumericalArray[initialArraySize] = newAddition;

	std::ofstream outIndexNumericalFile2;
	outIndexNumericalFile2.open(fileNumericalName);

	delete[] indexNumerical;

	outIndexNumericalFile2 << initialArraySize + 1;
	for (int j = 0; j < initialArraySize + 1; j++)
	{
		outIndexNumericalFile2 << "\n" << newNumericalArray[j];
	}
	outIndexNumericalFile2.close();
	return newNumericalArray;
}
Index* addToAlphabeticalFileAndArray(std::string newAddition, Index indexAlphabetical[], int& initialArraySize,
	std::string fileAlphabeticalName)
{
	Index* newAlphabeticalIndex = new Index[initialArraySize + 1];

	for (int j = 0; j < initialArraySize; j++)
	{
		newAlphabeticalIndex[j].name = indexAlphabetical[j].name;
		newAlphabeticalIndex[j].num = indexAlphabetical[j].num;
	}
	newAlphabeticalIndex[initialArraySize].name = newAddition;
	newAlphabeticalIndex[initialArraySize].num = short(initialArraySize);

	std::ofstream outIndexAlphabeticalFile;
	outIndexAlphabeticalFile.open(fileAlphabeticalName);

	//I want the 0 spot in the array to be 999 NO_Type/ActivationCondition/Ability so k needs to = 1
	//remember that the newIndex array is 1 bigger than the initialArraySize
	//i.e. for (int k = 1; k < initialArraySize + 1 - j - 1; k++)
	for (int j = 0; j < initialArraySize + 1; j++)
	{
		for (int k = 1; k < initialArraySize - j; k++)
		{
			if (newAlphabeticalIndex[k].name > newAlphabeticalIndex[k + 1].name)
			{
				std::string tempName = newAlphabeticalIndex[k].name;
				int tempInt = newAlphabeticalIndex[k].num;
				newAlphabeticalIndex[k].name = newAlphabeticalIndex[k + 1].name;
				newAlphabeticalIndex[k].num = newAlphabeticalIndex[k + 1].num;
				newAlphabeticalIndex[k + 1].name = tempName;
				newAlphabeticalIndex[k + 1].num = tempInt;
			}
		}
	}
	delete[] indexAlphabetical;

	outIndexAlphabeticalFile << newAlphabeticalIndex[0].num << "\n";
	outIndexAlphabeticalFile << newAlphabeticalIndex[0].name;

	for (int j = 1; j < initialArraySize + 1; j++)
	{
		outIndexAlphabeticalFile << "\n" << newAlphabeticalIndex[j].num;
		outIndexAlphabeticalFile << "\n" << newAlphabeticalIndex[j].name;
	}
	outIndexAlphabeticalFile.close();
	return newAlphabeticalIndex;
}
short int getAnswerFromMenu(std::string typeOfMenu, Index indexAlphabetical[], int initialArraySize)
{
	short int menuAnswer = -1;
	std::cout << "\n     " << typeOfMenu << " Menu\n";
	std::cout << 0 << " No " << typeOfMenu << "\n";					//every menu has a 0 and a No 
	std::cout << 999 << " Add a New " << typeOfMenu << "\n";			//every menu has a 999 which means that the user can add to the menu
	for (int j = 1; j < initialArraySize; j++)					//displays rest of menu
	{
		std::cout << indexAlphabetical[j].num << " " << indexAlphabetical[j].name << "\n";
	}
	std::cout << "\n";
	do					//prompts the user to enter the menu value and validates their input
	{
		menuAnswer = inputCheckShortInt("Menu - " + typeOfMenu + " (Enter a number from the menu above): ");
		//std::cout << "\n" << "Card - " << typeOfMenu << " (Enter a number from the menu above): ";
		//std::cin >> menuAnswer;
		if (menuAnswer < 0 || menuAnswer >= initialArraySize && menuAnswer != 999)
		{
			std::cout << "***** Error : Must enter a number from the menu *****\n";
			menuAnswer = -1;
		}
	} while (menuAnswer < 0 || menuAnswer >= initialArraySize && menuAnswer != 999);
	return menuAnswer;
}
short int inputCheckShortInt(std::string lineOut)
{
	short int shortInt = 0, n = 0;
	std::string input;
	bool trueFalse = false;
	do
	{
		std::cout << lineOut;
		std::cin >> input;
		//std::cout << "true\"" << trueFalse << "\"false";
		n = 0;
		while (input[n] != NULL)
		{
			if (
				input[n] == 48 ||
				input[n] == 49 ||
				input[n] == 50 ||
				input[n] == 51 ||
				input[n] == 52 ||
				input[n] == 53 ||
				input[n] == 54 ||
				input[n] == 55 ||
				input[n] == 56 ||
				input[n] == 57
				)
			{
				trueFalse = true;
			}
			else
			{
				trueFalse = false;
			}
			n++;
			if (!trueFalse)
				break;
		}
		if (!trueFalse)
			std::cout << "***** Error: Must enter an int *****\n";
	} while (!trueFalse);
	short int k = 0;
	do
	{
		switch (input[k])
		{
		case 48:
		{
			break;
		}
		case 49:
		{
			shortInt += 1 * int(pow(10, n - k - 1));
			break;
		}
		case 50:
		{
			shortInt += 2 * int(pow(10, n - k - 1));
			break;
		}
		case 51:
		{
			shortInt += 3 * int(pow(10, n - k - 1));
			break;
		}
		case 52:
		{
			shortInt += 4 * int(pow(10, n - k - 1));
			break;
		}
		case 53:
		{
			shortInt += 5 * int(pow(10, n - k - 1));
			break;
		}
		case 54:
		{
			shortInt += 6 * int(pow(10, n - k - 1));
			break;
		}
		case 55:
		{
			shortInt += 7 * int(pow(10, n - k - 1));
			break;
		}
		case 56:
		{
			shortInt += 8 * int(pow(10, n - k - 1));
			break;
		}
		case 57:
		{
			shortInt += 9 * int(pow(10, n - k - 1));
			break;
		}
		}
		k++;
	} while (input[k] != NULL);
	return shortInt;
}
bool inputCheckTrueFalse(std::string lineOut)
{
	std::string trueFalse;
	do
	{
		std::cout << lineOut;
		std::cin >> trueFalse;
		//std::cout << "true\"" << trueFalse << "\"false";
		if ((trueFalse[1] == NULL) && (trueFalse[0] == 'f' || trueFalse[0] == 't'))
		{
			if (trueFalse[0] == 't')
				return true;
			else if (trueFalse[0] == 'f')
				return false;
		}
		else
		{
			std::cout << "***** Error: Please enter only \"t\" or \"f\" *****\n";
			//string hold = " ";
			//std::cin.get();
			//std::cout << "Press Enter (unless a line of text is below this line): " << "\n";
			//getline(std::cin, hold);
		}
	} while (true);
}