#include "DemoTasks.hpp"
#include <cmath>
#include <random>
#include <numeric>
#include <algorithm>

namespace Workloads {

    std::vector<double> GenerateRandomData(size_t size, int seed) {
        std::vector<double> data(size);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 100.0);
        for (auto& val : data) {
            val = dis(gen);
        }
        return data;
    }

    std::vector<int> GenerateRandomIntData(size_t size, int seed) {
        std::vector<int> data(size);
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 10000);
        for (auto& val : data) {
            val = dis(gen);
        }
        return data;
    }

    // GÖREV 1: Aðýr Matematik (CPU Bound)
    double HeavyMathTask(long long iterations) {
        double result = 0.0;
        for (long long i = 0; i < iterations; ++i) {
            // Kasýtlý olarak karmaþýklaþtýrýlmýþ anlamsýz bir iþlem
            result += std::sin(static_cast<double>(i)) * std::cos(static_cast<double>(i)) + std::sqrt(static_cast<double>(i) + 1.0);
        }
        return result;
    }

    // GÖREV 2: Matris Çarpýmý (CPU & Memory Bound)
    std::vector<double> MatrixMultiplication(size_t N, const std::vector<double>& A, const std::vector<double>& B) {
        if (A.size() < N * N || B.size() < N * N) return {};

        std::vector<double> C(N * N, 0.0);
        for (size_t i = 0; i < N; ++i) {
            for (size_t k = 0; k < N; ++k) {
                double temp = A[i * N + k];
                for (size_t j = 0; j < N; ++j) {
                    C[i * N + j] += temp * B[k * N + j];
                }
            }
        }
        return C;
    }

    // GÖREV 3: Görüntü Ýþleme Simülasyonu (Memory Bandwidth)
    void ImageProcessing(std::vector<double>& data, size_t start_index, size_t end_index) {
        if (start_index >= data.size() || end_index > data.size()) return;

        for (size_t i = start_index; i < end_index; ++i) {
            // Piksel baþýna aðýr bir filtre simülasyonu
            double val = data[i];
            for (int j = 0; j < 20; ++j) {
                val = std::sqrt(std::abs(val * std::sin(val) * 3.14159));
            }
            data[i] = val;
        }
    }

    // GÖREV 4: Rastgele Bellek Eriþimi (Latency Bound)
    long long RandomMemoryAccess(const std::vector<int>& large_data, size_t access_count, int seed) {
        long long sum = 0;
        std::mt19937 gen(seed);
        // Dizi sýnýrlarý içinde rastgele indeks üret
        std::uniform_int_distribution<size_t> dis(0, large_data.size() - 1);

        for (size_t i = 0; i < access_count; ++i) {
            // Rastgele indekse eriþim
            sum += large_data[dis(gen)];
        }
        return sum;
    }

}