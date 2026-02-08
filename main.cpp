#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

// --- 1. КЛАСИ ВИКЛЮЧНИХ СИТУАЦІЙ ---
class SignalAlg2 {}; 
class SignalAlg3 {};

class ErrorNoFile {
    string filename;
public:
    ErrorNoFile(string s) : filename(s) {}
    void Message() { cout << "\n[ПОМИЛКА]: Файл " << filename << " не знайдено!" << endl; }
};

// --- 2. ПРОТОТИПИ ФУНКЦІЙ ---
double T(double x);
double U(double x);
double Srz(double x, double y, double z);
double Srs_Alg1(double x, double y, double z);
double Qrz_Alg1(double x, double y);
double Rrz_Alg2(double x, double y, double z);
double Rrz_Alg3(double x, double y, double z);
double Algorithm4_fun(double x, double y, double z);

// --- 3. РЕАЛІЗАЦІЯ ТАБЛИЦЬ (Імітація) ---
double T(double x) { return 0.5 * cos(x); }
double U(double x) { return 0.5 * sin(x); }

// --- 4. РЕАЛІЗАЦІЯ АЛГОРИТМІВ ---

double Srz(double x, double y, double z) {
    if (x > y) return T(x) + U(z) - T(y);
    return T(y) + U(y) - U(z);
}

double Qrz_Alg1(double x, double y) {
    if (abs(x) < 1) return x * Srs_Alg1(x, y, x);
    return y * Srz(y, x, y);
}

double Srs_Alg1(double x, double y, double z) {
    if (z > y) {
        if (z*z + x*y <= 0) throw SignalAlg2();
        return Srz(x, y, z) + y * sqrt(z*z + x*y);
    } else {
        if (x*x + z*y <= 0) throw SignalAlg3();
        return y + Srz(z, x, y) * sqrt(x*x + z*y);
    }
}

double Rrz_Alg2(double x, double y, double z) {
    if (x > y) return x * y * 0.5;
    return x * z * 0.5;
}

double Rrz_Alg3(double x, double y, double z) {
    if (x > y) return x * y * 0.3;
    return y * z * 0.3;
}

double Algorithm4_fun(double x, double y, double z) {
    return 1.3498 * x + 2.2362 * y * z - 2.348 * x * y;
}

// --- 5. ОСНОВНІ ОБЧИСЛЕННЯ ---

double Rrz_Alg1_Internal(double x, double y, double z) {
    if (x > y)
        return x * z * Qrz_Alg1(y, z);
    else
        return y * x * Qrz_Alg1(x, y);
}

double Grs(double x, double y, double z) {
    double term1, term2;
    try {
        term1 = Rrz_Alg1_Internal(x, y, y);
    } catch (SignalAlg2) {
        term1 = Rrz_Alg2(x, y, y);
    } catch (SignalAlg3) {
        term1 = Rrz_Alg3(x, y, y);
    }

    try {
        term2 = Rrz_Alg1_Internal(x - y, z, y);
    } catch (SignalAlg2) {
        term2 = Rrz_Alg2(x - y, z, y);
    } catch (SignalAlg3) {
        term2 = Rrz_Alg3(x - y, z, y);
    }
    return 0.1389 * term1 + 1.8389 * term2;
}

double fun(double x, double y, double z) {
    return x * Grs(x, y, z) + y * Grs(x, z, y);
}

// --- 6. ГОЛОВНА ФУНКЦІЯ ---
int main() {
    double x, y, z, f;

    cout << "Input x y z: ";
    if (!(cin >> x >> y >> z)) {
        cout << "Invalid input!" << endl;
        return 1;
    }

    try {
        // Симулюємо перевірку файлів
        bool files_exist = true; 
        if (!files_exist) throw ErrorNoFile("dat_X_1_1.dat");

        f = fun(x, y, z);
    }
    catch (ErrorNoFile& e) {
        e.Message();
        f = Algorithm4_fun(x, y, z);
    }
    catch (...) {
        cout << "\n Unknown error... Switching to Algorithm 4";
        f = Algorithm4_fun(x, y, z);
    }

    cout << "\n Result fun = " << fixed << setprecision(4) << f << endl;
    return 0;
}
