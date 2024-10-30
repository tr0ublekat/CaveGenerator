#include "GameOfLife.h"

GameOfLife::GameOfLife(size_t size, int chance) {
	this->mainMatrix.reserve(size+2);
	for (auto& a : this->mainMatrix)
		a.reserve(size+2);
	this->chance = chance;
	this->B = { 4 };
	this->S = { 5 };
    this->size = size + 2;
    this->init();
}

void GameOfLife::init() {
    this->mainMatrix.clear();
    for (size_t i = 0; i < size; ++i) {

        vector<bool> row;
        row.clear();
        row.reserve(size);

        for (size_t j = 0; j < size; ++j) {
            if ((i == 0) || (j == 0) || (i == size - 1) || (j == size - 1)) {
                bool value = true;
                row.push_back(value);
            }
            else {
                bool value = (randBool());
                row.push_back(value);
            }
        }
        this->mainMatrix.push_back(row);
    }
}

void GameOfLife::life() {
    this->secondMatrix.clear();
    for (size_t i = 0; i < size; i++) {

        vector<bool> row;
        row.clear();
        row.reserve(size);

        for (size_t j = 0; j < size; j++) {

            if ((i == 0) || (j == 0) || (i == size - 1) || (j == size - 1)) {
                row.push_back(false);
                continue;
            }

            int neighbours = getNeighbourCount(i, j);

            if (this->mainMatrix[i][j]) {
                if (neighbours >= 4 && neighbours <= 8) {
                    // Ничего
                    row.push_back(true);
                }
                else {
                    // map[i][j] = false;
                    row.push_back(false);
                }
            }
            else {
                if (neighbours >= 5 && neighbours <= 8) {
                    // map[i][j] = true;
                    row.push_back(true);
                }
                else {
                    // Ничего
                    row.push_back(false);
                }
            }
        }
        this->secondMatrix.push_back(row);
    }

    this->mainMatrix.clear();
    this->mainMatrix = this->secondMatrix;
    deleteBorders();
}

void GameOfLife::deleteBorders() {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if ((i == 0) || (j == 0) || (i == size - 1) || (j == size - 1)) {
                this->mainMatrix[i][j] = true;
            }
        }
    }
}

int GameOfLife::getNeighbourCount(int i, int j) {
    size_t count = 0;
    if (this->mainMatrix[i - 1][j - 1]) ++count;
    if (this->mainMatrix[i][j - 1]) ++count;
    if (this->mainMatrix[i + 1][j - 1]) ++count;
    if (this->mainMatrix[i + 1][j]) ++count;
    if (this->mainMatrix[i + 1][j + 1]) ++count;
    if (this->mainMatrix[i][j + 1]) ++count;
    if (this->mainMatrix[i - 1][j + 1]) ++count;
    if (this->mainMatrix[i - 1][j]) ++count;

    return count;
}

bool GameOfLife::randBool(int chance) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);
    int res = dist(gen);
    if (res >= chance) {
        return true;
    }
    return false;
}
void GameOfLife::setBS(std::initializer_list<size_t>& B, std::initializer_list<size_t>& S) {
	this->B = B;
	this->S = S;
}

vector<vector<bool>>& GameOfLife::operator()() {
	return this->mainMatrix;
}