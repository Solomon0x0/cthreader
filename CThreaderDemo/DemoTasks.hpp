#ifndef DEMO_TASKS_HPP
#define DEMO_TASKS_HPP

#include <vector>
#include <cstddef>
#include <cstdint>
#include <complex>
#include <string>

namespace Workloads {

    // =========================================
    // YARDIMCI VERİ TİPLERİ
    // =========================================
    struct Particle {
        double x, y, z;
        double vx, vy, vz;
        double mass;
    };

    using Complex = std::complex<double>;

    // =========================================
    // YARDIMCI FONKSİYONLAR (Benchmark hazırlığı için)
    // =========================================
    std::vector<double> GenerateRandomDoubleData(size_t size, int seed = 42);
    std::vector<int> GenerateRandomIntData(size_t size, int seed = 42);
    std::vector<int> GenerateGrid(int width, int height, int seed = 42);
    std::vector<Complex> GenerateRandomComplexData(size_t size, int seed = 42);
    std::string GenerateRandomString(size_t length, int seed = 42);

    // =========================================
    // İŞ YÜKÜ SENARYOLARI (TASKS)
    // =========================================

    // SENARYO 1: Ağır Matematik (FPU Bound)
    double Task1_HeavyMath(long long iterations);

    // SENARYO 2: Matris Çarpımı (CPU & Cache Bound)
    std::vector<double> Task2_MatrixMultiplication(size_t N, const std::vector<double>& A, const std::vector<double>& B);

    // SENARYO 3: 2D Konvolüsyon (Memory Bandwidth & FPU Bound)
    void Task3_2DConvolution(size_t width, size_t height, std::vector<double>& data, const std::vector<double>& kernel, int k_size);

    // SENARYO 4: Rastgele Bellek Erişimi (Latency Bound)
    long long Task4_RandomMemoryAccess(const std::vector<int>& large_data, size_t access_count, int seed);

    // SENARYO 5: Kripto Madencilik Simülasyonu (Integer ALU Bound)
    uint64_t Task5_CryptoMiningSim(uint64_t initial_nonce, size_t difficulty_iterations);

    // SENARYO 6: Sıralama ve Arama (Branch Prediction Bound)
    long long Task6_SortAndSearch(std::vector<int> data);

    // SENARYO 7: Asal Sayı Bulma - Trial Division (CPU Bound - Integer Division)
    size_t Task7_PrimeCounter(int start, int end);

    // SENARYO 8: N-Body Simülasyonu Step (FPU O(N^2) Bound)
    void Task8_NBodySimStep(std::vector<Particle>& particles, double dt);

    // SENARYO 9: Monte Carlo Pi (Embarrassingly Parallel CPU Bound)
    long long Task9_MonteCarloPi(long long iterations, int seed);

    // SENARYO 10: Özyinelemeli Fibonacci (Stack Bound)
    long long Task10_RecursiveFibonacci(int n);

    // SENARYO 11: Mandelbrot Set (FPU & Branch Heavy)
    std::vector<int> Task11_Mandelbrot(int width, int height, int max_iter);

    // SENARYO 12: Naive DFT (Trigonometric O(N^2) Bound)
    std::vector<Complex> Task12_NaiveDFT(const std::vector<Complex>& input);

    // SENARYO 13: Pathfinding BFS (Branch & Memory Latency Intense)
    int Task13_PathfindingBFS(const std::vector<int>& grid, int width, int height, int start_node, int end_node);

    // SENARYO 14: Sıkıştırma Simülasyonu (Memory Alloc Intense)
    std::vector<int> Task14_CompressionSim(const std::string& input);

    // SENARYO 15: Sieve of Eratosthenes (Memory Write Intense)
    std::vector<bool> Task15_SieveOfEratosthenes(int up_to);

}

#endif