#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int array_size = 16384;
const int block_size = 16;
const int density_reset = 8;

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

int main() {
    std::vector<std::vector<int>> array;
    int nnz_count = buildArray(array);

    std::string filename = "gradual_16k_8.mtx"; // Output file name
    writeMTX(array, filename, nnz_count);

    return 0;
}
