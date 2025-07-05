//NURULLAH YILDIRIM
//22100011045
/*Bu kodda inputA.txt dosyasını default olarak okuyor kodu çalıştırdığınız zaman dosya adı belirtmeye gerek yok*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 20  // Maksimum matris boyutu (20x20)

// Global değişkenler
int A[MAX_SIZE][MAX_SIZE]; // A matrisi
int B[MAX_SIZE][MAX_SIZE]; // B matrisi (kayma sonrası)
int C[MAX_SIZE][MAX_SIZE]; // Sonuç matrisi (Çarpım sonucu)
int n;  // Matris boyutu
int tempSum = 0;  // Geçici sonuç değişkeni (toplam değeri tutar)
pthread_mutex_t mutex;  // Mutex, geçici sonuçların güvenli bir şekilde yazılmasını sağlar

// Yapılacak işlemlerin tanımları
void readMatrixFromFile(const char* filename);
void writeMatrixToFile(const char* filename, int matrix[MAX_SIZE][MAX_SIZE]);
void* shiftRight(void* arg);
void* shiftUp(void* arg);
void* multiplyCells(void* arg);

// Matrisin her bir hücresini tutacak yapı
typedef struct {
    int row;
    int col;
} Cell;

int main() {
    pthread_t thread1, thread2;

    // Mutex'i başlatıyoruz
    pthread_mutex_init(&mutex, NULL);

    // Step 1: Matris A'yı dosyadan okuma
    readMatrixFromFile("inputA.txt");

    // Step 2: Kayma miktarını hesaplama (saniye değeri üzerinden)
    time_t t;
    time(&t);
    int shift = localtime(&t)->tm_sec % n;  // Kayma, saniye değeri üzerinden hesaplanır

    // Step 3: Kayma işlemlerini başlatıyoruz (sağa ve yukarı kaydırma)
    pthread_create(&thread1, NULL, shiftRight, &shift);
    pthread_join(thread1, NULL); // Sağ kaydırma tamamlandıktan sonra yukarı kaydırma yapılır

    pthread_create(&thread2, NULL, shiftUp, &shift);
    pthread_join(thread2, NULL); // Yukarı kaydırma tamamlanır

    // Step 4: Matris B'yi dosyaya yazma
    writeMatrixToFile("inputB.txt", B);

    // Step 5: Matris çarpımını yapacak iş parçacıkları (threads) oluşturuluyor
    pthread_t threads[MAX_SIZE * MAX_SIZE];
    Cell cells[MAX_SIZE * MAX_SIZE];
    int threadIndex = 0;

    // Sonuç matrisinin her elemanı için thread başlatıyoruz
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cells[threadIndex].row = i;
            cells[threadIndex].col = j;
            pthread_create(&threads[threadIndex], NULL, multiplyCells, &cells[threadIndex]);
            threadIndex++;
        }
    }

    // Tüm thread'lerin tamamlanmasını bekliyoruz
    for (int i = 0; i < threadIndex; i++) {
        pthread_join(threads[i], NULL);
    }

    // Step 6: Sonuç matrisini ekrana yazdırıyoruz ve dosyaya kaydediyoruz
    printf("Resultant Matrix C:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    writeMatrixToFile("outputC.txt", C);

    // Mutex'i yok ediyoruz
    pthread_mutex_destroy(&mutex);

    return 0;
}

/**
 * Dosyadan bir matrisi okur.
 */
void readMatrixFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &n);
    if (n > MAX_SIZE) {
        fprintf(stderr, "Matrix size exceeds maximum allowed size\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &A[i][j]);
        }
    }

    fclose(file);
}

/**
 * Bir matrisi dosyaya yazar.
 */
void writeMatrixToFile(const char* filename, int matrix[MAX_SIZE][MAX_SIZE]) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/**
 * A matrisini sağa kaydırma işlemi.
 */
void* shiftRight(void* arg) {
    int shift = *(int*)arg;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][(j + shift) % n] = A[i][j];
        }
    }

    pthread_exit(NULL);
}

/**
 * B matrisini yukarı kaydırma işlemi.
 */
void* shiftUp(void* arg) {
    int shift = *(int*)arg;

    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[(i + n - shift) % n][j] = B[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = temp[i][j];
        }
    }

    pthread_exit(NULL);
}

/**
 * İki matrisi çarpar ve sonucu C matrisine yazar.
 */
void* multiplyCells(void* arg) {
    Cell* cell = (Cell*)arg;
    int row = cell->row;
    int col = cell->col;

    int localSum = 0;
    for (int k = 0; k < n; k++) {
        localSum += A[row][k] * B[k][col];
    }

    pthread_mutex_lock(&mutex);
    tempSum = localSum;
    C[row][col] = tempSum;
    pthread_mutex_unlock(&mutex);

    FILE* file = fopen("outputC.txt", "a");
    if (file == NULL) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    fprintf(file, "C[%d][%d] = %d\n", row, col, tempSum);
    fclose(file);

    pthread_exit(NULL);
}
