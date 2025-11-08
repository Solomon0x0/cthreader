Benchmark Test Senaryoları (Workload Scenarios)

Bu doküman, CThreader kütüphanesinin performansını farklı donanım darboğazları altında test etmek için hazırlanmış 15 adet sentetik iş yükü senaryosunu açıklar. 
Her senaryo, CPU'nun belirli bir alt birimini (ALU, FPU, Branch Predictor) veya bellek alt sistemini (L1/L2/L3 Cache, RAM Bandwidth/Latency) hedef alacak şekilde tasarlanmıştır.

Senaryo Listesi ve Detaylı Açıklamalar


1. Ağır Matematik (FPU Bound)

    Hedef: Kayan nokta birimi (Floating Point Unit).

    Açıklama: Döngü içerisinde yoğun trigonometrik (sin, cos, tan) ve kök alma (sqrt) işlemleri gerçekleştirir. Bellek erişimi minimumdur, tamamen işlemci hızına bağımlıdır.

    Kullanım: Saf işlemci gücünü test etmek için idealdir.


2. Matris Çarpımı (CPU & Cache Bound)

    Hedef: CPU, L1/L2 önbellek ve bellek bant genişliği.

    Açıklama: Klasik O(N^3) matris çarpımı algoritması. Büyük matrislerde önbellek "miss" oranları artar ve işlemci veriyi beklemek zorunda kalır.

    Kullanım: İşlemcinin veriyi ne kadar hızlı işleyebildiğini ve bellekten ne kadar hızlı veri çekebildiğini test eder.


3. 2D Konvolüsyon (Memory Bandwidth & FPU Bound)

    Hedef: Bellek bant genişliği ve FPU.

    Açıklama: Bir görüntü üzerinde filtre gezdirme (blur, edge detection vb.) simülasyonu. Matris çarpımına göre daha düzenli ama yoğun bellek erişimi içerir.

    Kullanım: Gerçek dünyadaki görüntü işleme yüklerini simüle eder.


4. Rastgele Bellek Erişimi (Latency Bound)

    Hedef: RAM gecikmesi (Latency).

    Açıklama: Büyük bir veri dizisinin rastgele indekslerine erişir. Bu durum, işlemcinin önbellek tahmincisini (prefetcher) devre dışı bırakır ve her erişimde ana belleğe gidilmesine neden olur.

    Kullanım: İşlemcinin bellek gecikmelerine karşı toleransını ölçer.


5. Kripto Madencilik Simülasyonu (Integer ALU Bound)

    Hedef: Tamsayı Aritmetik Mantık Birimi (ALU).

    Açıklama: Bitcoin madenciliğine benzer şekilde yoğun bit manipülasyonu (XOR, shift, rotate) işlemleri yapar. Kayan nokta işlemi içermez.

    Kullanım: Tamsayı işlem performansını saf bir şekilde test eder.


6. Sıralama ve Arama (Branch Prediction Bound)

    Hedef: Dallanma tahmincisi (Branch Predictor).

    Açıklama: Büyük bir diziyi sıralar ve ardından üzerinde çok sayıda ikili arama (binary search) yapar. Karar mekanizmaları (if-else) işlemci boru hattını (pipeline) zorlar.

    Kullanım: İşlemcinin karmaşık karar ağaçlarını ne kadar iyi yönetebildiğini gösterir.


7. Asal Sayı Bulma (CPU Bound - Integer Division)

    Hedef: ALU (özellikle bölme işlemi ünitesi).

    Açıklama: Belirli bir aralıktaki sayıların asallığını "deneme bölmesi" (trial division) yöntemiyle test eder. Tamsayı bölme işlemi, işlemci için maliyetli bir operasyondur.

    Kullanım: Yoğun matematiksel tamsayı yüklerini simüle eder.


8. N-Body Simülasyonu (FPU O(N^2) Bound)

    Hedef: FPU ve vektör üniteleri (SIMD potansiyeli).

    Açıklama: Uzaydaki parçacıkların birbirlerine uyguladıkları kütle çekim kuvvetini hesaplar. Her parçacık diğer tüm parçacıklarla etkileşime girer.

    Kullanım: Bilimsel hesaplama yüklerini temsil eder.


9. Monte Carlo Pi (Embarrassingly Parallel CPU Bound)

    Hedef: Çok çekirdek ölçeklenebilirliği.

    Açıklama: Rastgele noktalar üreterek Pi sayısını tahmin eder. İş parçacıkları arasında veri paylaşımı gerektirmez, bu nedenle mükemmel şekilde paralelleştirilebilir.

    Kullanım: Thread havuzunun minimum genel gider (overhead) ile ne kadar verimli çalıştığını gösterir.


10. Özyinelemeli Fibonacci (Stack Bound)

    Hedef: Çağrı yığını (Call Stack) yönetimi.

    Açıklama: Fibonacci sayılarını derin özyineleme (recursion) ile hesaplar. Sürekli fonksiyon çağırma ve geri dönme işlemleri işlemciyi yorar.

    Kullanım: Fonksiyon çağrı maliyetlerini test eder.


11. Mandelbrot Seti (FPU & Branch Heavy)

    Hedef: FPU ve dallanma tahmincisi.

    Açıklama: Fraktal görüntü oluşturma. Her piksel için yapılan hesaplama miktarı farklıdır, bu da iş yükü dengesizliğine (load imbalance) yol açabilir.

    Kullanım: Dengesiz iş yüklerinin paralelleştirilmesini test eder.


12. Naive DFT (Trigonometric O(N^2) Bound)

    Hedef: Ağır FPU (trigonometri).

    Açıklama: Ayrık Fourier Dönüşümü'nün yavaş versiyonu. Çok yoğun sinüs/kosinüs hesaplamaları içerir.

    Kullanım: Sinyal işleme uygulamalarının işlemci üzerindeki etkisini gösterir.


13. Pathfinding BFS (Branch & Memory Latency Intense)

    Hedef: Bellek gecikmesi ve dallanma.

    Açıklama: Bir ızgara üzerinde engellerin etrafından dolaşarak en kısa yolu bulur. Grafik algoritmalarının tipik yükünü oluşturur.

    Kullanım: Yapay zeka ve oyun mantığı yüklerini simüle eder.


14. Sıkıştırma Simülasyonu (Memory Alloc Intense)

    Hedef: Dinamik bellek yönetimi (Heap).

    Açıklama: LZW benzeri bir algoritma ile veri sıkıştırma simülasyonu. Sürekli yeni bellek alanları tahsis edilir ve serbest bırakılır.

    Kullanım: Bellek yöneticisinin (memory allocator) performansını ve thread güvenliğini zorlar.


15. Sieve of Eratosthenes (Memory Write Intense)

    Hedef: Bellek yazma hızı.

    Açıklama: Asal sayıları bulmak için büyük bir bellek alanını sürekli olarak işaretler (yazar). Okumadan çok yazma işlemi baskındır.

    Kullanım: Bellek yazma bant genişliğini test eder.