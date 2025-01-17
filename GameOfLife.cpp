#include "GameOfLife.h"
uint GameOfLife::THREADS_COUNT = 2;

GameOfLife::GameOfLife(size_t size, uint chance) {
	this->mainMatrix.reserve(size+2);
	for (auto& a : this->mainMatrix)
		a.reserve(size+2);
	this->chance = chance;
    this->size = size;
    this->init();
}
GameOfLife::GameOfLife(size_t size, uint *chance) {
	this->mainMatrix.reserve(size+2);
	for (auto& a : this->mainMatrix)
		a.reserve(size+2);
	this->chance = *chance;
    this->size = size;
    this->init();
}

void GameOfLife::initMultiThread(uint top, uint bot) {
    uint y = top;
    while (y <= bot) {
        vector<bool> row;
        row.reserve(mainMatrix.size());
        for (size_t x = 0; x < mainMatrix.size(); ++x) {
            if ((x == 0) || (x == mainMatrix.size() - 1) || (y == 0) || (y == mainMatrix.size() - 1)) {
                row.push_back(true);
            }
            else {
                row.push_back(this->randBool(this->chance));
            }
        }
        mainMatrix[y] = row;
        y++;
    }
}

void GameOfLife::init() {
    this->mainMatrix.resize(size);
    const uint THREADS_COUNT = GameOfLife::THREADS_COUNT;
    const uint CHUNK_SIZE = (size / THREADS_COUNT) + 1;
    vector<std::thread> THREADS;

    for (uint x = 0; x < mainMatrix.size(); x += CHUNK_SIZE) {
        uint left = x, right = left + CHUNK_SIZE;
        if (left + CHUNK_SIZE > mainMatrix.size() && left != mainMatrix.size()) {
            right = mainMatrix.size()-1;
        }
        THREADS.push_back(std::thread([left, right, thisPtr = this]() {
            thisPtr->initMultiThread(left, right);
        }));
    }

    for (auto& thread : THREADS) {
        thread.join();
    }
}

void GameOfLife::reInit(size_t size) {
    this->size = size;
    this->mainMatrix.resize(size);
    this->backupMatrix.clear();
    this->secondMatrix.clear();
    this->init();
}
void GameOfLife::reInit(size_t size, uint chanceOfSpawn) {
    this->size = size;
    this->mainMatrix.resize(size);
    this->chance = chanceOfSpawn;
    this->backupMatrix.clear();
    this->secondMatrix.clear();
    this->init();
}

void GameOfLife::setThreadCount(uint count) {
    if (count == 0) {
        GameOfLife::THREADS_COUNT = std::thread::hardware_concurrency();
    }
    else {
        GameOfLife::THREADS_COUNT = count;
    }
}

void GameOfLife::multiThreadLife(uint top, uint bot) {
    uint pos = top; // pos = y
    while (pos != bot) {
        vector<bool> row;
        row.reserve(mainMatrix.size());
        for (uint i = 0; i < mainMatrix.size(); i++) {      // i = x
            if (pos == 0 || pos == mainMatrix.size() - 1 || i == 0 || i == mainMatrix.size() - 1) {
                row.push_back(false);
                continue;
            }
            size_t neighbours = getNeighbourCount(pos, i);
                
                
            if (this->mainMatrix[pos][i]) {
                if (isNumberInArray(neighbours, ref(S))) {
                    row.push_back(true);
                }
                else {
                    row.push_back(false);
                }
            }
            else {
                if (isNumberInArray(neighbours, ref(B))) {
                    row.push_back(true);
                }
                else {
                    row.push_back(false);
                }
            }
        }
        secondMatrix[pos] = row;
        pos++;
    }
}

void GameOfLife::life() noexcept {
    backupMatrix.reserve(iterations + 1); // мб даже лишнее
    backupMatrix.push_back(mainMatrix);
    this->secondMatrix.clear();
    if (B.size() == 0) {
        printf("B rules is empty!");
        return;
    }
    if (S.size() == 0) {
        printf("S rules is empty!");
        return;
    }
    const uint THREADS_COUNT = GameOfLife::THREADS_COUNT;
    /* RYZEN 7 5700X3D 4.05Ghz / 3466 16-8-20-21 8*4
    500:
        DEBUG BUILD - работает ОЧЕНЬ медленно
        3.25с ST
        2.27с 2 THREAD
        2.37с 3 THREAD
        2.58с 4 THREAD
        2.76с 5 THREAD
        ...
        6.62с 16 THREAD
        При Release сборке работает моментально
    1000:
        Release BUILD
        0.03с ST
        0.016 2 THREAD
        0.012 3 THREAD
        0.011 4 THREAD
        ---------------
        Включение второго потока ускоряет вычисление вполовину
    */
    const uint CHUNK_SIZE = (mainMatrix.size() / THREADS_COUNT) + 1;
    vector<std::thread> THREADS;

    auto timeN = std::chrono::high_resolution_clock::now();

    for (uint x = 0; x < mainMatrix.size(); x += CHUNK_SIZE) {
        uint left = x, right = left + CHUNK_SIZE;
        if (left + CHUNK_SIZE > mainMatrix.size() && left != mainMatrix.size()) {
            right = mainMatrix.size();
        }
        this->secondMatrix.resize(mainMatrix.size());
        THREADS.push_back(std::thread([left, right, thisPtr = this]() {
            thisPtr->multiThreadLife(left, right);
        }));
    }

    for (auto& thread : THREADS) {
        thread.join();
    }

    this->mainMatrix = this->secondMatrix;
    deleteBorders();
    this->iterations++;

    auto timeP = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(timeP - timeN);
    printf("generation time: %f s\n", duration.count());
}
void GameOfLife::fill(bool znach, uint posX, uint posY, uint size) noexcept {
    if (posX + size > this->mainMatrix.size() || posY + size > this->mainMatrix.size()) {
        printf("pos + size > matrix.size!\n");
        return;
    }
    for (uint x = posX; x < posX + size; x++) {
        for (uint y = posY; y < posY + size; y++) {
            this->mainMatrix[x][y] = znach;
        }
    }
    this->iterations++;
}

void GameOfLife::stepBack() {
    if (this->iterations > 0 && this->backupMatrix.size() > 0) {
        this->mainMatrix = this->backupMatrix.back();
        this->backupMatrix.pop_back();
        this->iterations--;
        this->deleteBorders();
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

size_t GameOfLife::getNeighbourCount(uint i, uint j) noexcept {
    size_t count = 0;
    if (i >= mainMatrix.size()) {
        printf("i more then size!");
        return 0;
    }
    if (j >= mainMatrix.size()) {
        printf("j more then size!");
        return 0;
    }

    if (this->mainMatrix[i - 1][j - 1]) ++count;
    if (this->mainMatrix[i - 1][j    ]) ++count;
    if (this->mainMatrix[i - 1][j + 1]) ++count;

    if (this->mainMatrix[i    ][j - 1]) ++count;
    if (this->mainMatrix[i    ][j + 1]) ++count;

    if (this->mainMatrix[i + 1][j - 1]) ++count;
    if (this->mainMatrix[i + 1][j    ]) ++count;
    if (this->mainMatrix[i + 1][j + 1]) ++count;


    return count;
}

bool GameOfLife::randBool(uint chance) noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);
    uint res = dist(gen);
    if (res >= chance) {
        return false;
    }
    return true;
}
void GameOfLife::setBS(std::initializer_list<size_t> B, std::initializer_list<size_t> S) {
	this->B = B;
	this->S = S;
}

vector<vector<bool>>* GameOfLife::operator()() {
    return &(this->mainMatrix);
}

void GameOfLife::setB(std::initializer_list<size_t> arr) {
    this->B = arr;
}
void GameOfLife::setS(std::initializer_list<size_t> arr) {
    this->S = arr;
}

void GameOfLife::setB(size_t begin, size_t end) {
    if (this->B.size() > 0) this->B.clear();
    for (size_t x = begin; x <= end; x++) {
        this->B.push_back(x);
    }
}
void GameOfLife::setS(size_t begin, size_t end) {
    if (this->S.size() > 0) this->S.clear();
    for (size_t x = begin; x <= end; x++) {
        this->S.push_back(x);
    }
}

void GameOfLife::setChance(uint chance) {
    this->chance = chance;
}
void GameOfLife::setChance(uint *chance) {
    this->chance = *chance;
}

bool GameOfLife::isNumberInArray(size_t number, vector<size_t>& vec) noexcept {
    for (auto& a : vec) {
        if (a == number) {
            return true;
        }
    }
    return false;
}

bool GameOfLife::deserialization(const string& filename) {
    std::ofstream stream(filename);
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

string GameOfLife::getRules() {
    string res = "chance: ";
    res += std::to_string(int(this->chance));
    res += "% | B";
    for (auto& a: this->B){
        res += std::to_string(a);
    }
    res+="/S";
    for (auto& a: this->S){
        res += std::to_string(a);
    }
    res += " | map size: ";
    res += std::to_string(int(this->mainMatrix.size()));
    return res;
}

void GameOfLife::saveToBMP(const string& filename) {

    Bitmap bmp;

    PixelMatrix pixelMat;
    for (auto& a : this->mainMatrix) {
        static vector<Pixel> tempMatrix;
        for (auto b : a) {
            tempMatrix.push_back(Pixel(b));
        }
        pixelMat.push_back(tempMatrix);
        tempMatrix.clear();
    }


    bmp.fromPixelMatrix(pixelMat);
    bmp.save(filename);

    printf("File %s saved!\n", filename.c_str());
}