#include "GameOfLife.h"

GameOfLife::GameOfLife(size_t size, unsigned int chance) {
	this->mainMatrix.reserve(size+2);
	for (auto& a : this->mainMatrix)
		a.reserve(size+2);
	this->chance = chance;
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
                bool value = (randBool(this->chance));
                row.push_back(value);
            }
        }
        this->mainMatrix.push_back(row);
    }
}

void GameOfLife::life() {
    this->secondMatrix.clear();
    if (B.size() == 0)
        throw std::runtime_error("B rules is empty!");
    if (S.size() == 0)
        throw std::runtime_error("S rules is empty!");
    for (size_t i = 0; i < size; i++) {

        vector<bool> row;
        row.clear();
        row.reserve(size);

        for (size_t j = 0; j < size; j++) {

            if ((i == 0) || (j == 0) || (i == size - 1) || (j == size - 1)) {
                row.push_back(false);
                continue;
            }

            size_t neighbours = getNeighbourCount(i, j);

            if (this->mainMatrix[i][j]) {
                if (isNumberInArray(neighbours, ref(S))) {
                    // Ничего
                    row.push_back(true);
                }
                else {
                    // map[i][j] = false;
                    row.push_back(false);
                }
            }
            else {
                if (isNumberInArray(neighbours, ref(B))) {
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

void GameOfLife::fill(bool znach, unsigned int posX, unsigned int posY, unsigned int size) {
    if (posX + size > this->mainMatrix.size() || posY + size > this->mainMatrix.size()) {
        throw std::runtime_error("pos + size > matrix.size!\n");
    }
    for (unsigned int x = posX; x < posX + size; x++) {
        for (unsigned int y = posY; y < posY + size; y++) {
            this->mainMatrix[x][y] = znach;
        }
    }
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

size_t GameOfLife::getNeighbourCount(unsigned int i, unsigned int j) {
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

bool GameOfLife::randBool(unsigned int chance) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);
    unsigned int res = dist(gen);
    if (res >= chance) {
        return false;
    }
    return true;
}
void GameOfLife::setBS(std::initializer_list<size_t> B, std::initializer_list<size_t> S) {
	this->B = B;
	this->S = S;
}

vector<vector<bool>>& GameOfLife::operator()() {
	return this->mainMatrix;
}

void GameOfLife::setB(std::initializer_list<size_t> arr) {
    this->B = arr;
}
void GameOfLife::setS(std::initializer_list<size_t> arr) {
    this->S = arr;
}

void GameOfLife::setB(size_t begin, size_t end) {
    for (size_t x = begin; x <= end; x++) {
        this->B.push_back(x);
    }
}
void GameOfLife::setS(size_t begin, size_t end) {
    for (size_t x = begin; x <= end; x++) {
        this->S.push_back(x);
    }
}

size_t GameOfLife::getSize() {
    return this->mainMatrix.size();
}

void GameOfLife::setChance(unsigned int chance) {
    this->chance = chance;
}

bool GameOfLife::isNumberInArray(size_t number, vector<size_t>& vec) {
    for (auto& a : vec) {
        if (a == number) {
            return true;
        }
    }
    return false;
}

bool GameOfLife::deserialization(const string& filename) {
    std::ofstream stream(filename+".txt");
    if (!stream.is_open()) {
        printf("Unable to write into a file %s!\n", filename.c_str());
        return false;
    }

    for (auto& x : this->mainMatrix) {
        for (auto y : x) {
            stream << y;
        } stream << "\n";
    }

    stream.close();
    return true;
}