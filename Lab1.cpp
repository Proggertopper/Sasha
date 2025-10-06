#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <windows.h>
using namespace std;


template <typename T>
T A_value(int i, int j) {
    return static_cast<T>((i + j) % 10); 
}


template <typename T>
vector<T> multiply_Ax(const vector<vector<T>>& A, const vector<T>& x) {
    int N = A.size();
    vector<T> y(N, 0);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    return y;
}


template <typename T>
vector<T> multiply_xA(const vector<vector<T>>& A, const vector<T>& x) {
    int N = A.size();
    vector<T> y(N, 0);
    for (int j = 0; j < N; j++)
        for (int i = 0; i < N; i++)
            y[j] += x[i] * A[i][j];
    return y;
}


template <typename T>
void smallExample(ofstream& fout, const string& typeName, int N) {
    fout << "\nЗамір швидкості для типу " << typeName
        << " (матриця " << N << "x" << N << ")\n\n";
    cout << "\nЗамір швидкості для типу " << typeName
        << " (матриця " << N << "x" << N << ")\n\n";

    vector<vector<T>> A(N, vector<T>(N));
    vector<T> x(N);

   
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = A_value<T>(i, j);

    
    for (int i = 0; i < N; i++)
        x[i] = static_cast<T>(i);

    
    fout << "1. створення квадратної матриці розміром " << N << "x" << N << ":\n";
    cout << "1. створення квадратної матриці розміром " << N << "x" << N << ":\n";
    fout << "[";
    for (int i = 0; i < N; i++) {
        fout << "\n ";
        for (int j = 0; j < N; j++)
            fout << setw(6) << A[i][j];
    }
    fout << "\n]\n";
    cout << "Матриця створена.\n";

   
    fout << "2. створення вектору розміром " << N << ":\n[ ";
    cout << "2. створення вектору розміром " << N << ":\n";
    for (int i = 0; i < N; i++) {
        fout << setw(6) << x[i];
        cout << x[i] << " ";
    }
    fout << " ]\n";

    
    vector<T> y = multiply_Ax(A, x);
    fout << "3. перемноження квадратної матриці на вектор:\n[ ";
    cout << "\n3. перемноження A*x:\n";
    for (int i = 0; i < N; i++) {
        fout << setw(6) << y[i];
        cout << y[i] << " ";
    }
    fout << " ]\n";

    
    vector<T> z = multiply_xA(A, x);
    fout << "\n4. перемноження вектора на квадратну матрицю:\n[ ";
    cout << "\n4. перемноження x*A:\n";
    for (int i = 0; i < N; i++) {
        fout << setw(6) << z[i];
        cout << z[i] << " ";
    }
    fout << "\n ]\n";
    cout << "\n";
}


template <typename T>
void testType(const string& typeName) {
    
    cout << "\n=== Тип даних: " << typeName << " ===\n";
    cout << setw(10) << "N"
        << setw(15) << "A*x (мс)"
        << setw(20) << "x*A (мс)"
        << setw(25) << "x*A оптиміз. (мс)\n";

    for (int N = 1000; N <= 46000; N += 1000) {
        try {
            vector<T> x(N), y(N), z(N), z_opt(N);
            for (int i = 0; i < N; i++)
                x[i] = static_cast<T>(i % 10);

            
            clock_t start1 = clock();
            for (int i = 0; i < N; i++) {
                T sum = 0;
                for (int j = 0; j < N; j++)
                    sum += A_value<T>(i, j) * x[j];
                y[i] = sum;
            }
            double t1 = 1000.0 * (clock() - start1) / CLOCKS_PER_SEC;

            
            clock_t start2 = clock();
            for (int j = 0; j < N; j++) {
                T sum = 0;
                for (int i = 0; i < N; i++)
                    sum += x[i] * A_value<T>(i, j);
                z[j] = sum;
            }
            double t2 = 1000.0 * (clock() - start2) / CLOCKS_PER_SEC;

            
            clock_t start3 = clock();
            for (int j = 0; j < N; j++) {
                T sum = 0;
                for (int i = 0; i < N; i++)
                    sum += x[i] * A_value<T>(j, i);
                z_opt[j] = sum;
            }
            double t3 = 1000.0 * (clock() - start3) / CLOCKS_PER_SEC;

            cout << setw(10) << N
                << setw(15) << fixed << setprecision(2) << t1
                << setw(20) << t2
                << setw(25) << t3 << "\n";

        }
        catch (bad_alloc&) {
            cout << " Не вистачає пам’яті для N = " << N << "\n";
            break;
        }
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    ofstream fout("C:\\Users\\Sasha\\Downloads\\results.txt");
    if (!fout.is_open()) {
        cerr << " не вдалося створити файл results.txt\n";
        return 1;
    }

    cout << "Порівняння часу множення матриці на вектор для різних типів даних\n";
    cout << "-----------------------------------------------------------------\n";

    
    smallExample<int>(fout, "int", 4);
    smallExample<float>(fout, "float", 4);
    smallExample<double>(fout, "double", 4);

    smallExample<int>(fout, "int", 8);
    smallExample<float>(fout, "float", 8);
    smallExample<double>(fout, "double", 8);

    smallExample<int>(fout, "int", 16);
    smallExample<float>(fout, "float", 16);
    smallExample<double>(fout, "double", 16);

    
    testType<int>("int");
    testType<float>("float");
    testType<double>("double");

    cout << "\nТестування завершено. Результати записані у файл results.txt\n";
    fout << "\n Тестування завершено.\n";
    fout.close();
    return 0;
}