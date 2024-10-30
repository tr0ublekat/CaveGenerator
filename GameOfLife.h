#include <vector>
#include <random>

using std::vector;

class GameOfLife {
private:
	vector<vector<bool>> mainMatrix;
	vector<vector<bool>> secondMatrix;
	int chance;
	size_t size;
	vector<size_t> B;
	vector<size_t> S;


	bool randBool(int chance = 50);
	int getNeighbourCount(int i, int j);
	void deleteBorders();
public:
	GameOfLife(size_t size, int chanceOfSpawn = 40);

	void init();
	void life();

	void setBS(std::initializer_list<size_t>& B, std::initializer_list<size_t>& S);
	vector<vector<bool>>& operator()(); // ¬озвращает матрицу по ссылке дл€ отрисовки
};