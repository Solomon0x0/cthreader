#pragma once
#include <vector>
#include <cstdint>

namespace Workloads {
    // Yardımcı: Test için rastgele veri üretir
    std::vector<double> GenerateRandomData(size_t size, int seed = 42);
    std::vector<int> GenerateRandomIntData(size_t size, int seed = 42);

    // 1. Saf CPU Yükü (Ağır Matematik)
    double HeavyMathTask(long long iterations);

    // 2. CPU + Bellek Yükü (Matris Çarpımı - NxN)
    std::vector<double> MatrixMultiplication(size_t N, const std::vector<double>& A, const std::vector<double>& B);

    // 3. Bellek Bant Genişliği Yükü (Görüntü İşleme Simülasyonu)
    void ImageProcessing(std::vector<double>& data, size_t start_index, size_t end_index);

    // 4. Bellek Gecikmesi Yükü (Rastgele Erişim - Cache Miss)
    long long RandomMemoryAccess(const std::vector<int>& large_data, size_t access_count, int seed);
}