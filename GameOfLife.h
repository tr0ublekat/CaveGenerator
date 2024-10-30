#include <vector>

using std::vector;

class GameOfLife {
private:
	vector<vector<bool>> matrix;
public:
	vector<vector<bool>>& operator()(); // ¬озвращает матрицу по ссылке дл€ отрисовки
};