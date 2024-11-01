#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
//#include <thread>
#include <functional>

using std::vector, std::string, std::ifstream, std::ref;

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
	unsigned int chance;
	size_t size;
	vector<size_t> B;
	vector<size_t> S;


	bool randBool(unsigned int chance = 50);
	size_t getNeighbourCount(unsigned int i, unsigned int j);
	void deleteBorders();
	bool isNumberInArray(size_t number,vector<size_t> &arr);
public:
	GameOfLife(size_t size, unsigned int chanceOfSpawn = 40);

	void init();
	void life();

	void fill(bool znach, unsigned int posX, unsigned int posY, unsigned int size);
	//void fill(bool znach, unsigned int posX, unsigned int posY, unsigned int posXkon, unsigned int posYkon);

	void setChance(unsigned int chance);
	void setBS(std::initializer_list<size_t> B, std::initializer_list<size_t> S);
	void setB(std::initializer_list<size_t> B);
	void setB(size_t begin, size_t end);
	void setS(std::initializer_list<size_t> S);
	void setS(size_t begin, size_t end); // устанавливает B от begin до end. Например (1,5) будет 1,2,3,4,5
	vector<vector<bool>>& operator()(); // Возвращает матрицу по ссылке для отрисовки

	bool deserialization(const string& filename = "../cave.txt");
};

// камера
// замкнутые пещеры
// импорт/экспорт