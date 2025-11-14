// main.cpp
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include <thread>
#include <chrono>
#include <typeinfo>
#include <any>
#include <array>
#include <utility>
#include <future>

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

#include "CThreader/CThreader.hpp"

#include "DemoTasks.hpp"

std::string AnyToString(const std::any& _value) 
{
    if (_value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(_value);
    }
    else if (_value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(_value));
    }
    else if (_value.type() == typeid(std::vector<double>)) {
        const auto value = std::any_cast<std::vector<double>>(_value);
        return std::to_string(value.size());
    }
    else if (_value.type() == typeid(long long)) {
        return std::to_string(std::any_cast<long long>(_value));
    }
    else if (_value.type() == typeid(uint64_t)) {
        return std::to_string(std::any_cast<uint64_t>(_value));
    }
    else if (_value.type() == typeid(size_t)) {
        return std::to_string(std::any_cast<size_t>(_value));
    }
    else if (_value.type() == typeid(std::vector<int>)) {
        const auto value = std::any_cast<std::vector<int>>(_value);
        return std::to_string(value.size());
    }
    else if (_value.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(_value));
    }
    else if (_value.type() == typeid(std::vector<bool>)) {
        const auto value = std::any_cast<std::vector<bool>>(_value);
        return std::to_string(value.size());
    }
	else if (_value.type() == typeid(std::vector<Workloads::Complex>)) {
		const auto value = std::any_cast<std::vector<Workloads::Complex>>(_value);
		return std::to_string(value.size());
	}
	else if (_value.type() == typeid(std::vector<Workloads::Particle>)) {
		const auto value = std::any_cast<std::vector<Workloads::Particle>>(_value);
		return std::to_string(value.size());
	}
    else {
		return "Bilinmeyen Tip.";
	}
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    using namespace CT;
    using namespace Workloads;

    CThreader threader;
    threader.Initialize(); // Havuz thread’lerini ayarla
    threader.Start(); // Havuz thread’lerini başlat

	const std::array<std::pair<std::string, std::function<std::any()>>, 15> tests = { {
		{"task1 ", [iterations = 1'000'000LL]() -> std::any {
			return Task1_HeavyMath(iterations);
		}},
		{"task2 ",
			[N = 64,
			 A = Workloads::GenerateRandomDoubleData(64 * 64, 42),
			 B = Workloads::GenerateRandomDoubleData(64 * 64, 1337)
			]() -> std::any
		{
			return Task2_MatrixMultiplication(N, A, B);
		}},
		{"task3 ",
			[width = 32,
		 height = 23,
		 kernel = Workloads::GenerateRandomDoubleData(9, 123), // 3×3 kernel örneği
		 k_size = 3
		]() -> std::any
	{
		std::vector<double> data = Workloads::GenerateRandomDoubleData(width * height, 42);
		Task3_2DConvolution(width, height, data, kernel, k_size);
		return data;
		}},
		{"task4 ",
			[large_data = Workloads::GenerateRandomIntData(1'000'000, 42),
			 access_count = 5'000'000,
			 seed = 12345]() -> std::any
		{
			return Task4_RandomMemoryAccess(large_data, access_count, seed);
		}},
		{"task5 ",
			[initial_nonce = 1000ULL,
			 difficulty_iterations = 500'000ULL]() -> std::any
		{
			return Task5_CryptoMiningSim(initial_nonce, difficulty_iterations);
		}},
		{"task6 ",
			[data = Workloads::GenerateRandomIntData(200'000, 777)]() -> std::any
		{
			return Task6_SortAndSearch(data);
		}},
		{"task7 ",
			[start = 2,
			 end = 200'000]() -> std::any
		{
			return Task7_PrimeCounter(start, end);
		}},
		{"task8",
		[]() -> std::any
		{
		std::vector<Particle> particles(200);
		for (size_t i = 0; i < particles.size(); i++) {
			particles[i] = { double(i), double(i + 1), double(i + 2),
							 0.01, -0.02, 0.03, 1.0 };
		}
		double dt = 0.01;
		Task8_NBodySimStep(particles, dt);
		return particles;
		}},
		{"task9 ",
			[iterations = 5'000'000LL,
			 seed = 9876]() -> std::any
		{
			return Task9_MonteCarloPi(iterations, seed);
		}},
		{"task10 ",
			[n = 35]() -> std::any
		{
			return Task10_RecursiveFibonacci(n);
		}},
		{"task11 ",
			[width = 500,
			 height = 500,
			 max_iter = 1000]() -> std::any
		{
			return Task11_Mandelbrot(width, height, max_iter);
		}},
		{"task12",
		[input = Workloads::GenerateRandomComplexData(512, 42)]() -> std::any
		{
		return Task12_NaiveDFT(input);
		}},
		{"task13 ",
			[width = 200,
			 height = 200,
			 grid = Workloads::GenerateGrid(200, 200, 42),
			 start_node = 0,
			 end_node = 39'999]() -> std::any
		{
			return Task13_PathfindingBFS(grid, width, height, start_node, end_node);
		}},
		{"task14 ",
			[input = Workloads::GenerateRandomString(2000, 123)]() -> std::any
		{
			return Task14_CompressionSim(input);
		}},
		{"task15 ",
			[up_to = 2'000'000]() -> std::any
		{
			return Task15_SieveOfEratosthenes(up_to);
		}}
	} };

	const auto startTime = std::chrono::steady_clock::now();
    for (const auto& test : tests)
    {
        const auto result = test.second();
        std::cout << test.first << ":" << AnyToString(result) << std::endl;
		Sleep(2000);
    }
	const auto elapsedTime = std::chrono::steady_clock::now() - startTime;
	const auto elapsedTimeMs = std::chrono::duration<double>(elapsedTime);

	std::cout << "Tüm görevler tamamlanması bitme süresi: " << elapsedTimeMs << std::endl;

	std::unordered_map<std::string, std::future<std::any>> futureValues;
	for (const auto& test : tests) {
		futureValues.insert({ test.first, std::async(std::launch::async, test.second) });
	}
	while (!futureValues.empty()){

		for (auto it = futureValues.begin(); it != futureValues.end(); )
		{
			auto& name = it->first;
			auto& futureValue = it->second;

			if (futureValue.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
			{
				auto result = futureValue.get();
				std::cout << name << ":" << AnyToString(result) << std::endl;

				it = futureValues.erase(it); 
			}
			else {
				++it;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::unordered_map<std::string, uint64_t> results;
	for	(const auto& test : tests) {
		const auto resultId = threader.Enqueue(CT::Task(test.second));
		results.insert({ test.first, resultId });
	}
	while (!results.empty())
	{
		for (auto it = results.begin(); it != results.end(); ) {
			auto& name = it->first;
			auto& resultId = it->second;
			if (const auto result = threader.GetResult(resultId); result) {
				if (result.value()) {
					std::cout << name << ": " << AnyToString(result.value().GetValue()) << std::endl;
					results.erase(it);
				}
			}
			else
			{
				std::cout << name << ": Error ID: " << static_cast<int>(result.error()) << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}