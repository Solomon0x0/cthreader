#include "DemoTasks.hpp"
#include <cmath>
#include <random>
#include <numeric>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Workloads {

    // =========================================
    // YARDIMCI FONKSİYON IMPLEMENTASYONLARI
    // =========================================
    std::vector<double> GenerateRandomDoubleData(size_t size, int seed) {
        std::vector<double> data(size);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1000.0);
        std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
        return data;
    }

    std::vector<int> GenerateRandomIntData(size_t size, int seed) {
        std::vector<int> data(size);
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 100000);
        std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
        return data;
    }

    std::vector<int> GenerateGrid(int width, int height, int seed) {
        std::vector<int> grid(width * height, 0);
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 100);
        for (auto& cell : grid) {
            cell = (dis(gen) < 20) ? 1 : 0;
        }
        grid[0] = 0; grid[width * height - 1] = 0; 
        return grid;
    }

    std::vector<Complex> GenerateRandomComplexData(size_t size, int seed) {
        std::vector<Complex> data(size);
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        for (auto& val : data) {
            val = Complex(dis(gen), dis(gen));
        }
        return data;
    }

    std::string GenerateRandomString(size_t length, int seed) {
        std::string s(length, ' ');
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis((int)'a', (int)'z');
        for (auto& c : s) c = static_cast<char>(dis(gen));
        return s;
    }

    // =========================================
    // İŞ YÜKÜ SENARYOLARI (TASKS)
    // =========================================

    double Task1_HeavyMath(long long iterations) {
        double result = 0.0;
        for (long long i = 0; i < iterations; ++i) {
            double d = static_cast<double>(i);
            result += std::sin(d) * std::cos(d) + std::sqrt(std::abs(std::tan(d) + 1.0));
        }
        return result;
    }

    std::vector<double> Task2_MatrixMultiplication(size_t N, const std::vector<double>& A, const std::vector<double>& B) {
        if (A.size() != N * N || B.size() != N * N) return {};
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

    void Task3_2DConvolution(size_t width, size_t height, std::vector<double>& data, const std::vector<double>& kernel, int k_size) {
        std::vector<double> output = data;
        int k_half = k_size / 2;
        for (size_t y = k_half; y < height - k_half; ++y) {
            for (size_t x = k_half; x < width - k_half; ++x) {
                double sum = 0.0;
                for (int ky = -k_half; ky <= k_half; ++ky) {
                    for (int kx = -k_half; kx <= k_half; ++kx) {
                        sum += data[(y + ky) * width + (x + kx)] * kernel[(ky + k_half) * k_size + (kx + k_half)];
                    }
                }
                output[y * width + x] = sum;
            }
        }
        data = output;
    }

    long long Task4_RandomMemoryAccess(const std::vector<int>& large_data, size_t access_count, int seed) {
        long long sum = 0;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<size_t> dis(0, large_data.size() - 1);
        for (size_t i = 0; i < access_count; ++i) {
            sum += large_data[dis(gen)];
        }
        return sum;
    }

    uint64_t Task5_CryptoMiningSim(uint64_t initial_nonce, size_t difficulty_iterations) {
        uint64_t hash = initial_nonce;
        for (size_t i = 0; i < difficulty_iterations; ++i) {
            hash ^= (hash << 13) | (hash >> 51);
            hash += 0x5bd1e995;
            hash ^= (hash >> 15);
            hash *= 0x5bd1e9955bd1e995ULL;
        }
        return hash;
    }

    long long Task6_SortAndSearch(std::vector<int> data) {
        std::sort(data.begin(), data.end());
        long long found_count = 0;
        for (size_t i = 0; i < data.size() / 10; ++i) {
            if (std::binary_search(data.begin(), data.end(), static_cast<int>(i * 7))) {
                found_count++;
            }
        }
        return found_count;
    }

    size_t Task7_PrimeCounter(int start, int end) {
        size_t prime_count = 0;
        for (int num = start; num <= end; ++num) {
            if (num < 2) continue;
            bool is_prime = true;
            for (int i = 2; i * i <= num; ++i) {
                if (num % i == 0) {
                    is_prime = false; break;
                }
            }
            if (is_prime) prime_count++;
        }
        return prime_count;
    }

    void Task8_NBodySimStep(std::vector<Particle>& particles, double dt) {
        size_t n = particles.size();
        for (size_t i = 0; i < n; ++i) {
            double fx = 0, fy = 0, fz = 0;
            for (size_t j = 0; j < n; ++j) {
                if (i == j) continue;
                double dx = particles[j].x - particles[i].x;
                double dy = particles[j].y - particles[i].y;
                double dz = particles[j].z - particles[i].z;
                double dist_sq = dx * dx + dy * dy + dz * dz + 1e-9;
                double inv_dist3 = std::pow(dist_sq, -1.5);
                double force = particles[j].mass * particles[i].mass * inv_dist3;
                fx += force * dx; fy += force * dy; fz += force * dz;
            }
            particles[i].vx += fx * dt / particles[i].mass;
            particles[i].vy += fy * dt / particles[i].mass;
            particles[i].vz += fz * dt / particles[i].mass;
        }
    }

    long long Task9_MonteCarloPi(long long iterations, int seed) {
        long long inside_circle = 0;
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (long long i = 0; i < iterations; ++i) {
            double x = dis(gen); double y = dis(gen);
            if (x * x + y * y <= 1.0) inside_circle++;
        }
        return inside_circle;
    }

    long long Task10_RecursiveFibonacci(int n) {
        if (n <= 1) return n;
        return Task10_RecursiveFibonacci(n - 1) + Task10_RecursiveFibonacci(n - 2);
    }

    std::vector<int> Task11_Mandelbrot(int width, int height, int max_iter) {
        std::vector<int> output(width * height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                double r_c = (x - width / 2.0) * 4.0 / width;
                double i_c = (y - height / 2.0) * 4.0 / height;
                std::complex<double> c(r_c, i_c), z(0, 0);
                int iter = 0;
                while (std::abs(z) <= 2.0 && iter < max_iter) {
                    z = z * z + c; iter++;
                }
                output[y * width + x] = iter;
            }
        }
        return output;
    }

    std::vector<Complex> Task12_NaiveDFT(const std::vector<Complex>& input) {
        size_t N = input.size();
        std::vector<Complex> output(N);
        for (size_t k = 0; k < N; ++k) {
            Complex sum(0.0, 0.0);
            for (size_t n = 0; n < N; ++n) {
                double angle = -2.0 * M_PI * k * n / N;
                sum += input[n] * Complex(std::cos(angle), std::sin(angle));
            }
            output[k] = sum;
        }
        return output;
    }

    int Task13_PathfindingBFS(const std::vector<int>& grid, int width, int height, int start_node, int end_node) {
        if (grid[start_node] == 1 || grid[end_node] == 1) return -1;
        std::queue<std::pair<int, int>> q;
        q.push({ start_node, 0 });
        std::vector<bool> visited(width * height, false);
        visited[start_node] = true;
        int directions[4] = { -1, 1, -width, width };

        while (!q.empty()) {
            auto current = q.front(); q.pop();
            int curr = current.first; int dist = current.second;
            if (curr == end_node) return dist;
            for (int dir : directions) {
                int next = curr + dir;
                if (next >= 0 && next < width * height) {
                    if ((dir == -1 && curr % width == 0) || (dir == 1 && (curr + 1) % width == 0)) continue;
                    if (!visited[next] && grid[next] == 0) {
                        visited[next] = true; q.push({ next, dist + 1 });
                    }
                }
            }
        }
        return -1;
    }

    std::vector<int> Task14_CompressionSim(const std::string& input) {
        std::unordered_map<std::string, int> dict;
        for (int i = 0; i < 256; i++) dict[std::string(1, static_cast<char>(i))] = i;
        std::string w;
        std::vector<int> compressed;
        int next_code = 257;
        for (char c : input) {
            std::string wc = w + c;
            if (dict.count(wc)) { w = wc; }
            else {
                compressed.push_back(dict[w]);
                dict[wc] = next_code++;
                w = std::string(1, c);
            }
        }
        if (!w.empty()) compressed.push_back(dict[w]);
        return compressed;
    }

    std::vector<bool> Task15_SieveOfEratosthenes(int up_to) {
        std::vector<bool> is_prime(up_to + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int p = 2; p * p <= up_to; p++) {
            if (is_prime[p]) {
                for (int i = p * p; i <= up_to; i += p) is_prime[i] = false;
            }
        }
        return is_prime;
    }

}