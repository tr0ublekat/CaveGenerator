#include <vector>
#include <random>
#include <iostream>
using std::vector;

template<typename Any>
std::ostream& operator<<(std::ostream& stream, vector<Any> vec) {
	for (auto& a : vec) {
		stream << a << " ";
	}
	return stream;
}

class GameOfLife {
private:
	vector<vector<bool>> mainMatrix;
	vector<vector<bool>> secondMatrix;
	int chance;
	size_t size;
	vector<size_t> B;
	vector<size_t> S;


	bool randBool(int chance = 50);
	size_t getNeighbourCount(int i, int j);
	void deleteBorders();
	bool isNumberInArray(size_t number,vector<size_t> &arr);
public:
	GameOfLife(size_t size, int chanceOfSpawn = 40);

	void init();
	void life();

	void setBS(std::initializer_list<size_t> B, std::initializer_list<size_t> S);
	vector<vector<bool>>& operator()(); // ¬озвращает матрицу по ссылке дл€ отрисовки
};