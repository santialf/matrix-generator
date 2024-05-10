#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>


const int array_size = 4096;
const int block_size = 16;
const int density_reset = 32;

int buildArray(std::vector<std::vector<int>>& array) {
    array.resize(array_size, std::vector<int>(array_size, 0));

    int nnz_count = 0;

    // Seed the random number generator
    std::srand(std::time(nullptr));

    int block_count = 0;
    for (int i = 0; i < array_size; i += block_size) {
        for (int j = 0; j < array_size; j += block_size) {
            // Determine the number of non-zero elements for this block
            int block_density = block_count % density_reset + 1;
            
            // Generate non-zero elements randomly within the block
            int l = 0;
            for (int k = 0; k < block_size; ++k) {
                for (int l2 = 0; l < block_density && l2 < block_size; ++l2) {
                    int row = i + k;
                    int col = j + l2;
                    
                    if (row < array_size && col < array_size) {
                        array[row][col] = 1;
                        nnz_count++;
                    }
                    l++;
                }
            }
            block_count++;
        }
    }
    return nnz_count;
}

std::vector<int> generateRandomVector(int n) {
    // Create a vector of size n with numbers 1 to n
    std::vector<int> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = i + 1;
    }

    // Shuffle the elements randomly
    std::random_shuffle(result.begin(), result.end());

    return result;
}

int buildRandomArray(std::vector<std::vector<int>>& array) {
    array.resize(array_size, std::vector<int>(array_size, 0));

    int nnz_count = 0;

    // Seed the random number generator
    std::srand(std::time(nullptr));

    int block_count = 0;
    for (int i = 0; i < array_size; i += block_size) {
        for (int j = 0; j < array_size; j += block_size) {
            // Determine the number of non-zero elements for this block
            int block_density = block_count % density_reset + 1;
            std::vector<int> ids(block_size*block_size);
            ids = generateRandomVector(block_size*block_size);

            // Generate non-zero elements randomly within the block
            int l2 = 0;
            for (int k = 0; k < block_size; ++k) {
                for (int l = 0; l2 < block_density && l < block_size; ++l) {
                    int id = k*block_size + l + 1;
                    int row = i + k;
                    int col = j + l;
                    
                    if ((row < array_size && col < array_size) && (std::find(ids.begin(), ids.begin() + block_density, id) != ids.begin() + block_density)){
                        array[row][col] = 1;
                        nnz_count++;
                        l2++;
                    }
                }
            }
            block_count++;
        }
    }
    return nnz_count;
}

void writeMTX(const std::vector<std::vector<int>>& array, const std::string& filename, int nnz_count) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write Matrix Market format header
    outFile << "%%MatrixMarket matrix coordinate pattern general" << std::endl;
    outFile << array_size << " " << array_size << " " << nnz_count << std::endl;;

    // Count non-zero elements and write coordinates of non-zero elements
    for (int i = 0; i < array_size; ++i) {
        for (int j = 0; j < array_size; ++j) {
            if (array[i][j] != 0) {
                //nnz_count++;
                outFile << i + 1 << " " << j + 1 << std::endl;
            }
        }
    }

    // Close the file
    outFile.close();

    std::cout << "Matrix coordinates have been written to " << filename << std::endl;
}

int main(int argc, char * argv[]) {
    std::vector<std::vector<int>> array;
    int nnz_count;

    //nnz_count = buildArray(array);
    nnz_count = buildRandomArray(array);

    std::string filename = "test.mtx"; // Output file name
    writeMTX(array, filename, nnz_count);

    return 0;
}
