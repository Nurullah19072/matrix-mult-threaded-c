# 🧮 Çok İş Parçacıklı Matris Çarpımı (C)  
# 🧮 Multithreaded Matrix Multiplication in C

## 📌 Proje Açıklaması | Project Description

**TR:**  
Bu proje, C dili kullanılarak POSIX `pthreads` kütüphanesi ile matris kaydırma ve çarpma işlemlerini paralel olarak gerçekleştiren çok iş parçacıklı bir uygulamadır. A matrisi bir dosyadan okunur, sistem saatine göre satır ve sütun kaydırması yapılır, ardından çarpım işlemi gerçekleştirilerek sonuç dosyaya yazılır.

**EN:**  
This project performs matrix shifting and multiplication using POSIX threads (`pthreads`) in C. Matrix A is loaded from a file, shifted based on the current second, and multiplied with the shifted version. The result is written to a file.

---

## 🔧 Özellikler | Features

- 🔄 Sistem saatine göre matrisin sağa ve yukarı kaydırılması  
- 🔢 Matris elemanlarının çarpımı için her hücreye ayrı thread atanması  
- 📂 Dosya okuma/yazma işlemleri (input/output)  
- 🔒 Paylaşılan belleğe güvenli erişim için mutex kullanımı  

- 🔄 Row and column shifting based on system time  
- 🔢 One thread per matrix cell for parallel multiplication  
- 📂 File input/output operations  
- 🔒 Mutex synchronization for safe shared memory access

---

## 🖥️ Nasıl Çalıştırılır | How to Run

**Gerekli:** `inputA.txt` dosyası proje dizininde bulunmalıdır.

**Required:** `inputA.txt` file must be present in the project directory.

```bash
gcc -pthread mxMult_22100011045.c -o matrix_exec
./matrix_exec
