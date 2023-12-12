// ParallelComputing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

std::once_flag flag;

void calculateSum(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

int main() {
    std::setlocale(LC_ALL, "RU");

    int hardwareThreads = std::thread::hardware_concurrency();
    std::cout << "Cores " << hardwareThreads << std::endl;

    const int numThreads[] = { 1, 2, 4, 8, 16 };
    const int vectorSizes[] = { 1000, 10000, 100000, 1000000 };

    const int columnWidth = 12;

    
    std::cout << std::setw(columnWidth) << "Size:";
    for (int size : vectorSizes) {
        std::cout << std::setw(columnWidth) << size;
    }
    std::cout << "\n";

    for (int threads : numThreads) {
        int usedThreads = std::min(threads, hardwareThreads);

        std::cout << std::setw(columnWidth) << "Threads:"  << usedThreads;

        for (int size : vectorSizes) {
            std::vector<int> a(size, 1);
            std::vector<int> b(size, 2);
            std::vector<int> result(size);

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<std::thread> threadPool;
            int chunkSize = size / usedThreads;
            for (int i = 0; i < usedThreads; ++i) {
                int startIdx = i * chunkSize;
                int endIdx = (i == usedThreads - 1) ? size : (i + 1) * chunkSize;

                threadPool.emplace_back(calculateSum, std::ref(a), std::ref(b), std::ref(result), startIdx, endIdx);
            }

            for (std::thread& t : threadPool) {
                t.join();
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            
            std::cout << std::setw(columnWidth) << std::fixed << std::setprecision(4) << duration.count();
        }

        std::cout << "\n";
    }

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
