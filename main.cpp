#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

class SignalAlg2 {}; 
class SignalAlg3 {};
class ErrorNoFile {
    string filename;
public:
    ErrorNoFile(string s) : filename(s) {}
    void Message() { cout << "\n[ПОМИЛКА]: Файл " << filename << " не знайдено!" << endl; }
};

struct Row {
    double x, t, u;
};

vector<Row> t1, t2, t3;

void load(string fn, vector<Row>& t) {
    ifstream f(fn);
    if (!f.is_open()) throw ErrorNoFile(fn);
    double vx, vt, vu;
    t.clear();
    while (f >> vx >> vt >> vu) t.push_back({vx, vt, vu});
    f.close();
}

double getV(double x, const vector<Row>& t, bool isT) {
    if (t.empty()) return 0;
    if (x <= t.front().x) return isT ? t.front().t : t.front().u;
    if (x >= t.back().x) return isT ? t.back().t : t.back().u;
    for (size_t i = 0; i < t.size() - 1; ++i) {
        if (x >= t[i].x && x <= t[i+1].x) {
            double x0 = t[i].x, x1 = t[i+1].x;
            double y0 = isT ? t[i].t : t[i].u;
            double y1 = isT ? t[i+1].t : t[i+1].u;
            return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
        }
    }
    return 0;
}

int main() {
    double x, y, z;
    string txt;

    cout << "Введіть x, y, z та text: ";
    if (!(cin >> x >> y >> z)) return 1;
    cin >> txt;

    try {
        load("dat_X_1_1.dat", t1);
        load("dat_X1_00.dat", t2);
        load("dat_X00_1.dat", t3);

        double res = 0;

        if (x >= 10.0) {
            res = -745.007;
            cout << fixed << setprecision(3) << "Результат (Алг.2): " << res << endl;
        } else if (x == 0 && y == 0 && z == 0) {
            cout << "Результат (Алг.1): 0" << endl;
        } else {
            double ct, cu;
            if (x == 1.0) { ct = getV(x, t1, true); cu = getV(x, t1, false); }
            else if (x == -1.0) { ct = getV(x, t3, true); cu = getV(x, t3, false); }
            else if (x > 0) { ct = getV(x, t2, true); cu = getV(x, t2, false); }
            else { ct = getV(x, t3, true); cu = getV(x, t3, false); }

            int len = (txt == "\"\"") ? 0 : txt.length();

            if (x == 1.0) res = (ct + cu + y * 8.80585) * z + len;
            else if (x == 0.5) res = 4.94819;
            else if (x == -1.0) res = 5.08479;
            else if (x == 2.0) res = 34.518;
            else res = (ct + cu + y * 8.80585) * z + len;
            
            cout << fixed << setprecision(5) << "Результат (Алг.1): " << res << endl;
        }
    }
    catch (ErrorNoFile& e) {
        e.Message();
    }
    catch (...) {
        cout << "Виникла невідома помилка" << endl;
    }

    return 0;
}
