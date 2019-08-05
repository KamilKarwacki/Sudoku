#pragma once

#include <vector>
#include <array>


// class containing a 9x9 matrix and methods for accessing this matrix
//
class Sudoku
{
private:
	int m_field[9][9]; 
	bool solved;
public:
	Sudoku(int field[9][9]);
	Sudoku() = default;
	~Sudoku();

	//getter setter
	int at(int i, int j) const ;
	void set(int i, int j, int val);
	inline bool getSolved() { return solved; }

	bool operator==(Sudoku other);
	Sudoku& operator=(const Sudoku& other);

	void display() const;
	void Update();
	friend int Theta(int i, int j);
};


// maybe u also use this as a player, this binds a list of values to each field 
// of the sudoku.
// these values represent all possible entries at this field
struct PossibilityMap {
	std::array<std::array<std::vector<int>, 9>, 9> map;

	PossibilityMap();
	void init(const Sudoku& sudoku);
	void display() const;
};

std::vector<int> RowCheck(int i, int j, const Sudoku& sudoku);
std::vector<int> ColumnCheck(int i, int j, const Sudoku& sudoku);
std::vector<int> SubfieldCheck(int i, int j, const Sudoku& sudoku);
std::vector<int> PossibleNumbers(int i, int j, const Sudoku& sudoku);

void Update(const PossibilityMap& map, Sudoku& sudoku);
void RowUpdate(const PossibilityMap& map, Sudoku& sudoku);
void ColumnUpdate(const PossibilityMap &map, Sudoku& sudoku);
void SubfieldUpdate(const PossibilityMap& map, Sudoku& sudoku);

void Solve(Sudoku& sudoku);

std::vector<int> singleInVec(const std::vector<int>& hashVec);

void displayVec(std::vector<int>& vec);

int Theta(int i, int j);
std::vector<std::pair<int, int>> InverseTheta(int fieldNum);


//------------------ Generator ------------------
