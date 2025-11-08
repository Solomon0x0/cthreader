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

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

#include "CThreader/CThreader.hpp"

// ───────────────────────────────────────────────────────────────────────────────
// 1) Serbest (free) fonksiyon örnekleri
int Topla(int a, int b) { return a + b; }
double Hipotenus(double x, double y) { return std::sqrt(x * x + y * y); }
void YazMesaj(std::string s) { std::cout << "[Mesaj] " << s << '\n'; }

// 2) Statik üye fonksiyon
struct Math {
    static int Carp(int a, int b) { return a * b; }
    static double Bol(double a, double b) { return a / b; }
};

// 3) Functor (callable class)
struct KelimeSayaci {
    int operator()(const std::string& text) const {
        int count = 0;
        for (char c : text)
            if (c == ' ') count++;
        return count + 1;
    }
};

// 4) std::function değişkeni
std::function<std::string(std::string)> Selamla = [](std::string isim) {
    return "Merhaba, " + isim + "!";
};

// 5) Template fonksiyon
template<typename T>
T Kare(T x) { return x * x; }

// 6) Sınıf üyesi fonksiyon (member function pointer ile çağıracağız)
struct Hesaplayici {
    double carp(double x, double y) const { return x * y; }
};

// ───────────────────────────────────────────────────────────────────────────────
// Küçük yardımcı: std::any yazdırıcı
static void PrintAny(const std::any& a) {
    try {
        if (a.type() == typeid(int)) {
            std::cout << std::any_cast<int>(a);
        }
        else if (a.type() == typeid(double)) {
            std::cout << std::any_cast<double>(a);
        }
        else if (a.type() == typeid(std::string)) {
            std::cout << std::any_cast<std::string>(a);
        }
        else if (a.type() == typeid(const char*)) {
            std::cout << std::any_cast<const char*>(a);
        }
        else if (a.type() == typeid(bool)) {
            std::cout << (std::any_cast<bool>(a) ? "true" : "false");
        }
        if (a.type() == typeid(void) || !a.has_value()) {
            std::cout << "[void]";
        }
        else if (a.type() == typeid(long long)) {
            std::cout << std::any_cast<long long>(a);
        }
        else {
            std::cout << "[Bilinmeyen tip: " << a.type().name() << "]";
        }
    }
    catch (...) {
        std::cout << "[Tip hatası]";
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    using namespace CT;

    CThreader threader;
    threader.Initialize(); // Havuz thread’lerini ayarla
    threader.Start(); // Havuz thread’lerini başlat

    std::cout << "=== CThreader Öncelikli Kuyruk Örnekleri ===\n\n";

    // İpuçları:
    // - Enqueue(Task(...), TaskLevel::Low/Medium/High) şeklinde priority dışarıdan veriliyor.
    // - priority_queue; Compare'a göre en küçük level öne gelir (Low önce).
    // - Aynı seviyede FIFO garanti edilemiyor (stabil değil). FIFO istenirse Task'a "sequenceNo"
    //   ekleyip Compare’da (level, sequenceNo) tuple’ı ile kıyaslayabilirsin.

    // ID’leri saklayalım ki sonra sonuçları dolaşalım
    std::vector<std::pair<uint64_t, std::string>> jobs; // (id, açıklama)

    // ───────────────────────────────────────────────────────────────────────────
    // 1) Basit lambdalar (farklı öncelikler)
    // Low (0) → önce işlenecek
    jobs.emplace_back(threader.Enqueue(Task([](int a, int b) { return a + b; }, 3, 7), TaskLevel::Low),
        "Low: lambda int+int");

    // High (2) → daha geç
    jobs.emplace_back(threader.Enqueue(Task([](std::string s) { return s + " - işlendi"; }, std::string("CThreader")), TaskLevel::High),
        "High: lambda string işleme");

    // Medium (1)
    jobs.emplace_back(threader.Enqueue(Task([] { std::cout << "Medium void lambda çalıştı!\n"; }), TaskLevel::Medium),
        "Medium: void lambda");

    // ───────────────────────────────────────────────────────────────────────────
    // 2) Serbest fonksiyonlar (free functions)
    jobs.emplace_back(threader.Enqueue(Task(Topla, 5, 12), TaskLevel::Low),
        "Low: Topla(5,12)");
    jobs.emplace_back(threader.Enqueue(Task(Hipotenus, 3.0, 4.0), TaskLevel::High),
        "High: Hipotenus(3,4)");
    jobs.emplace_back(threader.Enqueue(Task(YazMesaj, std::string("Normal fonksiyon görev")), TaskLevel::Medium),
        "Medium: YazMesaj(...) -> void");

    // ───────────────────────────────────────────────────────────────────────────
    // 3) Statik üye fonksiyonlar
    jobs.emplace_back(threader.Enqueue(Task(Math::Carp, 4, 9), TaskLevel::Low),
        "Low: Math::Carp(4,9)");
    jobs.emplace_back(threader.Enqueue(Task(Math::Bol, 81.0, 9.0), TaskLevel::Medium),
        "Medium: Math::Bol(81,9)");

    // ───────────────────────────────────────────────────────────────────────────
    // 4) Functor (callable class)
    KelimeSayaci sayac;
    jobs.emplace_back(threader.Enqueue(Task(sayac, "CThreader thread pool test"), TaskLevel::Low),
        "Low: KelimeSayaci(\"CThreader thread pool test\")");

    // ───────────────────────────────────────────────────────────────────────────
    // 5) std::function
    jobs.emplace_back(threader.Enqueue(Task(Selamla, "Timur"), TaskLevel::Medium),
        "Medium: std::function Selamla(\"Timur\")");

    // ───────────────────────────────────────────────────────────────────────────
    // 6) Template fonksiyon
    jobs.emplace_back(threader.Enqueue(Task(Kare<int>, 12), TaskLevel::High),
        "High: Kare<int>(12)");
    jobs.emplace_back(threader.Enqueue(Task(Kare<double>, 5.5), TaskLevel::Low),
        "Low: Kare<double>(5.5)");

    // ───────────────────────────────────────────────────────────────────────────
    // 7) Sınıf üye fonksiyonu (member function pointer + nesne adresi)
    Hesaplayici h;
    jobs.emplace_back(threader.Enqueue(Task(&Hesaplayici::carp, &h, 2.5, 8.0), TaskLevel::Medium),
        "Medium: Hesaplayici::carp(2.5, 8.0)");

    // ───────────────────────────────────────────────────────────────────────────
    // 8) Yakalamalı / yakalamasız lambdalar
    // a) Yakalamayan lambda (parametreli)
    jobs.emplace_back(threader.Enqueue(Task([](int n) {
        int sum = 0;
        for (int i = 1; i <= n; ++i) sum += i;
        return sum;
        }, 100), TaskLevel::Low), "Low: 1..n toplami (n=100)");

    // b) Yakalama yapan lambda
    int sabit = 50;
    jobs.emplace_back(threader.Enqueue(Task([sabit](int x) { return sabit + x; }, 25), TaskLevel::High),
        "High: sabit+y (sabit=50, x=25)");

    // ───────────────────────────────────────────────────────────────────────────
    // 9) Ağır CPU işi (Pi yaklaşımı, intentionally küçük iterasyon sayısı)
    jobs.emplace_back(threader.Enqueue(Task([]() -> double {
        double pi = 0.0;
        for (int i = 0; i < 200000; ++i)
            pi += (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1.0);
        return pi * 4.0;
        }), TaskLevel::Medium), "Medium: Pi yaklaşımı (Leibniz) ~200k iter");

    // ───────────────────────────────────────────────────────────────────────────
    // 10) Nested görev (içeride ufak bekleme)
    jobs.emplace_back(threader.Enqueue(Task([] {
        std::cout << "[Nested] İç görev başlıyor...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return std::string("İç görev tamamlandı");
        }), TaskLevel::Low), "Low: nested kısa iş");

    // ───────────────────────────────────────────────────────────────────────────
    // 11) Boş parametreli lambda
    jobs.emplace_back(threader.Enqueue(Task([] { return 42; }), TaskLevel::High),
        "High: sabit 42");

    // ───────────────────────────────────────────────────────────────────────────
    // 12) Karmaşık veri döndürme (std::any ile taşınır)
    struct Data { int a; double b; std::string c; };
    // Not: std::any ile custom struct döndürüyorsan GetValue tarafında type check gerekecek.
    jobs.emplace_back(threader.Enqueue(Task([]() -> Data { return { 3, 4.5, "veri" }; }), TaskLevel::Medium),
        "Medium: custom struct Data");

    // ───────────────────────────────────────────────────────────────────────────
    // 13) Yüksek CPU yükü (büyük döngü)
    jobs.emplace_back(threader.Enqueue(Task([] {
        long long sum = 0;
        for (long long i = 0; i < 1'000'000; ++i)
            sum += i % 5;
        return sum;
        }), TaskLevel::Low), "Low: 1e6 mod toplami");

    // ───────────────────────────────────────────────────────────────────────────
    // 14) Hata/exception yönetimi örneği
    // Burada exception fırlatan bir işi, içeride yakalayıp string döndürüyoruz.
    // (ThreadPool tarafında try/catch varsa orası handle eder; yoksa bu şekilde güvence al)
    jobs.emplace_back(threader.Enqueue(Task([]() -> std::string {
        try {
            throw std::runtime_error("Beklenen test hatasi");
        }
        catch (const std::exception& e) {
            return std::string("[Handled Exception] ") + e.what();
        }
        }), TaskLevel::Medium), "Medium: try/catch ile sarmalanmış istisna");

    // ───────────────────────────────────────────────────────────────────────────
    // 15) IO-benzeri (simüle) iş: küçük beklemeler
    jobs.emplace_back(threader.Enqueue(Task([]() -> std::string {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return "IO benzeri is bitti (sim)";
        }), TaskLevel::High), "High: IO-sim işi");

    // Küçük bekleme: hem üretim hem tüketim çakışmasın (sadece demo amaçlı)
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // ───────────────────────────────────────────────────────────────────────────
    // Sonuçları yazdır
    std::cout << "\n=== Sonuçlar (id, açıklama, değer) ===\n";

    auto printResult = [&](uint64_t id, const std::string& desc) {
        // GetResult(id) -> std::optional<TaskResult* / TaskResultRef / benzeri> varsayıyorum.
        auto result = threader.GetResult(id);

        std::cout << " - [" << id << "] " << desc << " -> ";
        if (!result) {
            std::cout << "[Result yok / daha hazır değil]\n";
            return;
        }

        // Senin tipinde HasValue/GetValue olduğunu biliyoruz (std::any taşıyor).
        if (result->HasValue()) {
            const auto& v = result->GetValue();
            // Eğer custom struct (Data) döndüreceksen burada özel handling yapabilirsin:
            if (v.type() == typeid(Data)) {
                const auto& d = std::any_cast<const Data&>(v);
                std::cout << "Data{ a=" << d.a << ", b=" << d.b << ", c=\"" << d.c << "\" }";
            }
            else {
                PrintAny(v);
            }
        }
        else {
            std::cout << "[Değer yok]";
        }
        std::cout << "\n";
        };

    // Not: Burada basitçe kısa bir bekleme daha koyuyoruz; gerçek dünyada
    //  - ya sonuçlar hazır olana dek koşullu bekleme (condition_variable),
    //  - ya futures/promises/await benzeri,
    //  - ya da zaman-aşımı ile polling yaparsın.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (auto& [id, desc] : jobs) {
        printResult(id, desc);
    }

    std::cout << "\n=== Açıklamalar ===\n"
        << "* Öncelik: Low < Medium < High. Low her zaman önce işlenir.\n"
        << "* Aynı seviyede `std::priority_queue` stabil değil: FIFO garanti edilmez.\n"
        << "  FIFO istersen Task’a artan `sequenceNo` koy, Compare’de (level, sequenceNo) karşılaştır.\n"
        << "* `Task` içinde level yok; Enqueue sırasında ayrı parametre ile veriliyor.\n"
        << "* `std::any` ile farklı dönüş tiplerini tek tip altında taşıyoruz.\n"
        << "* Üye fonksiyon çağrısı: `Task(&Class::method, &instance, args...)` şeklinde.\n";

    std::cout << "\n=== Bitti ===\n";
    return 0;
}
