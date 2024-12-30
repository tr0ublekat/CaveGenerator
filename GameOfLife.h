#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include <thread>
#include <array>
#include <iterator>
#include <utility>

// #include "BitmapPlusPlus.hpp"
#include "bitmap.h"

using uint = unsigned int;

using std::vector, std::string, std::ifstream, std::ref,
std::array, std::pair, std::thread;

template<typename Any>
std::ostream& operator<<(std::ostream& stream, const std::vector<Any>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		stream << vec[i];
		if (i < vec.size() - 1) {
			stream << " ";
		}
	}
	return stream;
}

template<typename Any>
std::ostream& operator<<(std::ostream& stream, const std::vector<std::vector<Any>>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		stream << vec[i];
		if (i < vec.size() - 1) {
			stream << std::endl;
		}
	}
	return stream;
}


class GameOfLife {
private:
	vector<vector<bool>> mainMatrix;
	vector<vector<bool>> secondMatrix;
	vector<vector<vector<bool>>> backupMatrix;
	uint chance;
	size_t size;
	vector<size_t> B;
	vector<size_t> S;


	static bool randBool(uint chance = 50) noexcept;
	size_t getNeighbourCount(uint i, uint j) noexcept;
	void deleteBorders();
	static bool isNumberInArray(size_t number, vector<size_t> &arr) noexcept;

	void init();

	uint iterations = 0;

	void initMultiThread(uint top, uint bot);
	void multiThreadLife(uint top, uint bot);
	static uint THREADS_COUNT;
public:
	GameOfLife(size_t size, uint chanceOfSpawn = 40);
	GameOfLife(size_t size, uint *chanceOfSpawn); // инициализация по указателю на шанс появления

	void reInit(size_t size);
	void reInit(size_t size, uint chanceOfSpawn);
	void life() noexcept;

	void fill(bool znach, uint posX, uint posY, uint size) noexcept;
	static void setThreadCount(uint count);

	string getRules();
	void stepBack();


	void setChance(uint chance);
	void setChance(uint *chance);
	void setBS(std::initializer_list<size_t> B, std::initializer_list<size_t> S);
	void setB(std::initializer_list<size_t> B);
	void setB(size_t begin, size_t end);
	void setS(std::initializer_list<size_t> S);
	void setS(size_t begin, size_t end); // ������������� B �� begin �� end. �������� (1,5) ����� 1,2,3,4,5
	vector<vector<bool>>& operator()(); // ���������� ������� �� ������ ��� ���������

	bool deserialization(const string& filename = "cave.txt");
	void saveToBMP(const string& filename = "cave.bmp");
};