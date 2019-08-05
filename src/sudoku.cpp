#include "../include/sudoku.h"
#include <iostream>
#include<cmath>

Sudoku::Sudoku(int field[9][9])
{
	//@brief initialize by using array
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			m_field[i][j] = field[i][j];
		}
	}
}

// class contains no pointers
Sudoku::~Sudoku(){}


void Sudoku::display() const
{	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cout << m_field[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int Sudoku::at(int i, int j) const
{
	if (i > 8 || i < 0 || j > 8 || j < 0) {
		std::cerr << "out of bounds" << std::endl;
	}
	// replace with 2 at calls
	return m_field[i][j];
}

void Sudoku::set(int i, int j, int val) 
{
	if (i > 8 || i < 0 || j > 8 || j < 0) {
		std::cerr << "out of bounds" << std::endl;
	}
	// replace with 2 at calls
	m_field[i][j] = val;
}

bool Sudoku::operator==(Sudoku other)
{
	// 2 sudokus are assumed to be equal
	// if they differ at some point j and i they become unequal
	bool equal = true;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (this->at(i, j) != other.at(i, j))
				equal = false;
		}
	}
	return equal;
}

Sudoku& Sudoku::operator=(const Sudoku& other) 
{	
	
	if(&other == this)
		return *this;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			this->m_field[i][j] = other.m_field[i][j];
		}
	}
	return *this;
}

void Sudoku::Update() 
{
	// checks if sudoku got solved
	solved = true;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (m_field[i][j] == 0)
				solved = false;
		}
	}
}

//-------------------------------------- PossibilityMap -----------------------------

PossibilityMap::PossibilityMap()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
		//	map.at(i).at(j).push_back(-1);
		}
	}
}

void PossibilityMap::init(const Sudoku& sudoku) 
{
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (sudoku.at(i, j) != 0) {
				map.at(i).at(j).clear();
				map.at(i).at(j).push_back(-1);
			}
			else {
				map.at(i).at(j) = PossibleNumbers(i,j,sudoku);
			}
		}
	}
}

void PossibilityMap::display() const
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cout << "(" << i << "," << j << ")  ";
			for (auto& k : map.at(i).at(j)) {
				std::cout << k << " ";
			}
			std::cout << std::endl;
		}
	}
}

//--------------------------------------- Functions --------------------------------

std::vector<int> RowCheck( int row, const Sudoku& sudoku)
{
	//@brief returns a vector of all numbers already in row
	std::vector<int> inRow ;
	for (int col = 0; col < 9; col++) {
		if (sudoku.at(row, col) != 0) {
			inRow.push_back(sudoku.at(row, col));
		}
	}
	return inRow;
}

std::vector<int> ColumnCheck(int col, const Sudoku& sudoku)
{
	//@brief returns a vector of all numbers already in Column
	std::vector<int> inColumn;
	for (int row = 0; row < 9; row++) {
		if (sudoku.at(row, col) != 0) {
			inColumn.push_back(sudoku.at(row, col));
		}
	}
	return inColumn;
}

std::vector<int> SubfieldCheck(int i, int j, const Sudoku& sudoku) 
{
	//@brief returns a vector of all numbers already in subfield
	std::vector<std::pair<int, int>> indices = InverseTheta(Theta(i,j));
	std::vector<int> result;
	for (auto &i : indices) {
		if (sudoku.at(i.first, i.second) != 0)
			result.push_back(sudoku.at(i.first, i.second));
	}
	return result;
}

std::vector<int> PossibleNumbers(int row, int col, const Sudoku& sudoku)
{
	//@brief writes all possible numbers in one vector
	std::vector<int> notPossible1 = ColumnCheck(col, sudoku);
	std::vector<int> notPossible2 = RowCheck(row, sudoku);
	std::vector<int> notPossible3 = SubfieldCheck(row, col, sudoku);

	std::vector<int> Possible = { 1,2,3,4,5,6,7,8,9 };
	for (size_t k = 0; k < notPossible1.size(); k++) {
		Possible[notPossible1[k] - 1] = 0;
	}
	for (size_t k = 0; k < notPossible2.size(); k++) {
		Possible[notPossible2[k] - 1] = 0;
	}
	for (size_t k = 0; k < notPossible3.size(); k++) {
		Possible[notPossible3[k] - 1] = 0;
	}

	std::vector<int> result;
	for (int& k : Possible) {
		if (k != 0)
			result.push_back(k);
	}
	return result;
}

void Update(const PossibilityMap& Posmap, Sudoku& sudoku)
{
	//@brief Checks if there is only one possible number in a field

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (Posmap.map.at(i).at(j).size() == 1 && Posmap.map.at(i).at(j).at(0) != -1) {
				std::cout << "Updating with first method" << "(" << i <<"," << j << ")" << "=" << Posmap.map.at(i).at(j).at(0) << std::endl;
				sudoku.set(i, j, Posmap.map.at(i).at(j).at(0));
			}
		}
	}
}

void RowUpdate(const PossibilityMap& Posmap, Sudoku& sudoku)
{
	//@brief Checks if a number appears only once in a Row
	std::vector<int> hashVector = {0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (auto &w : Posmap.map.at(i).at(j)) {
				if (w != -1) {
					hashVector.at(w - 1) += 1;
				}
			}
		}
		

		std::vector<int> singles = singleInVec(hashVector);
		for (auto &k : singles) {
			for (int j = 0; j < 9; j++) {
				for (auto &w : Posmap.map.at(i).at(j)) {
					if (k == w) {
						sudoku.set(i, j, k);
						std::cout << "Updating with RowUpdate: " << "(" << i << "," << j << ")" << "=" << k << std::endl;
					}
				}
			}
		}
		hashVector = { 0,0,0,0,0,0,0,0,0 };
	}
}

void ColumnUpdate(const PossibilityMap& Posmap, Sudoku& sudoku)
{
	//@brief Checks if a number appears only once in a Column
	std::vector<int> hashVector = { 0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (auto &w : Posmap.map.at(j).at(i)) {
				if (w != -1) {
					hashVector.at(w - 1) += 1;
				}
			}
		}

		std::vector<int> singles = singleInVec(hashVector);
		for (auto &k : singles) {
			for (int j = 0; j < 9; j++) {
				for (auto &w : Posmap.map.at(j).at(i)) {
					if (k == w) {
						sudoku.set(j, i, k);
						std::cout << "Updating with ColumnUpdate: " << "(" << j << "," << i << ")" << "=" << k << std::endl;
					}
				}
			}
		}
		hashVector = { 0,0,0,0,0,0,0,0,0 };
	}
}

void SubfieldUpdate(const PossibilityMap& Posmap, Sudoku& sudoku) 
{
	//@brief checks if a number appears only once in a Subfield
	std::vector<int> hashVector = { 0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 9; i++) { // sum over subfields
		std::vector<std::pair<int, int>> indices = InverseTheta(i);
		for (auto &k : indices) {
			for (auto &w : Posmap.map.at(k.first).at(k.second)) {
				if (w != -1)
					hashVector.at(w - 1) += 1;
			}
		}
		

		std::vector<int> singles = singleInVec(hashVector);
		for (auto &k : singles) {
			for (auto &x : indices) {
				for (auto &w : Posmap.map.at(x.first).at(x.second)) {
					if (k == w) {
						sudoku.set(x.first, x.second, w);
						std::cout << "Updating with SubfieldUpdate: " << "(" << x.first << "," << x.second << ")" << "=" << w << std::endl;
					}
				}
			}
		}
		hashVector = { 0,0,0,0,0,0,0,0,0 };
	}
}

void Solve(Sudoku& sudoku)
{
	int iterations = 1;
	PossibilityMap map;
	map.init(sudoku);
	
	while (true) {
		Sudoku last = sudoku;
		std::cout << "---iteration: " << iterations << std::endl;

		SubfieldUpdate(map, sudoku);
		map.init(sudoku);

		Update(map, sudoku);
		map.init(sudoku);

		ColumnUpdate(map, sudoku);
		map.init(sudoku);

		RowUpdate(map, sudoku);
		map.init(sudoku);

		if (last == sudoku) {
			if (sudoku.getSolved()) {
				std::cout << "Sudoku solved in " << iterations << " iterations" << std::endl;
				break;
			}
			else {
				std::cout << "ERROR, Sudoku is unsolvable with current methods" << std::endl;
				break;
			}
		}
		iterations++;
	}
}

std::vector<int> singleInVec(const std::vector<int>& hashVec) {
	std::vector<int> result; 
	for (size_t i = 0; i < hashVec.size(); i++) {
		if (hashVec.at(i) == 1)
			result.push_back(i+1);
	}
	return result;
}

int Theta(int i, int j) 
{
	// function mapping from i and j to subfield number
	if (i < 3)
		return (int)std::floor(j / 3);
	if (i >= 3 && i < 6)
		return (int)std::floor(j / 3 + 3);
	else
		return (int)std::floor(j / 3 + 6);
}

std::vector<std::pair<int,int>> InverseTheta(int fieldNum)
{
	// function returning all i and j pairs that correspond to subfield number
	std::vector <std::pair<int, int>> result;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (Theta(i, j) == fieldNum) {
				result.push_back({ i,j });
			}
		}
	}
	return result;
}

void displayVec(std::vector<int>& vec) {
	for (auto &i : vec) {
		std::cout << i << std::endl;
	}
}


//------------------------ Generator -------------------------------
