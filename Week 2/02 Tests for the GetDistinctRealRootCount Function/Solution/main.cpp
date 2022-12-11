#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}


template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        }
        catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        }
        catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

/* Возвращает количество уникальных действительных корней
   уравнения ax² + bx + c = 0.
*/
int GetDistinctRealRootCount(double a, double b, double c) {
    
    // Если оба коэффициента (a и b) == 0, есть только с, то действительных корней несуществует.
    if (a == 0 && b == 0) { return 0; }

    // Если коэффициент а == 0 , то квадратное уравнение выраждается
    // в линейное, у которого только один действительный корнь.
    if (a == 0 ) { return 1; }

    // Высчитываем дискриминант.
    double d = b * b - 4 * a * c;
    if (d > 0) { return 2; }
    if (d < 0) { return 0; }

    // Здесь d == 0.
    return 1;
}

void TestIsOnlyConstant_C()
{
    AssertEqual(GetDistinctRealRootCount(0, 0, 1), 0, "point 1");
    AssertEqual(GetDistinctRealRootCount(0, 0, -1), 0, "point 2");
}
void TestLinerEquation()
{
    AssertEqual(GetDistinctRealRootCount(0, 3, 0), 1, "point 1");
    AssertEqual(GetDistinctRealRootCount(0, 1, 3), 1, "point 2");
    AssertEqual(GetDistinctRealRootCount(0, -3, 0), 1, "point 3");
    AssertEqual(GetDistinctRealRootCount(0, 3, -1), 1, "point 4");
    AssertEqual(GetDistinctRealRootCount(0, -1, -1), 1, "point 5");
}

void TestZeroRealRoots()
{
    AssertEqual(GetDistinctRealRootCount(1, 1, 1), 0, "point 1");
    AssertEqual(GetDistinctRealRootCount(2, 6, 9), 0, "point 2");
    AssertEqual(GetDistinctRealRootCount(9, 6, 2), 0, "point 3");
    AssertEqual(GetDistinctRealRootCount(2, 0, 9), 0, "point 4");
    AssertEqual(GetDistinctRealRootCount(2, -6, 9), 0, "point 5");
    AssertEqual(GetDistinctRealRootCount(-2, 6, -9), 0, "point 6");
    AssertEqual(GetDistinctRealRootCount(-9, 6, -2), 0, "point 7");
    AssertEqual(GetDistinctRealRootCount(-2, 0, -9), 0, "point 8");
    AssertEqual(GetDistinctRealRootCount(-2, -6, -9), 0, "point 9");
}
void TestOneRoot()
{
    AssertEqual(GetDistinctRealRootCount(3, 6, 3), 1, "point 1");
    AssertEqual(GetDistinctRealRootCount(1, 10, 25), 1, "point 2");
    AssertEqual(GetDistinctRealRootCount(25, 10, 1), 1, "point 3");
    AssertEqual(GetDistinctRealRootCount(3, 0, 0), 1, "point 4");
    AssertEqual(GetDistinctRealRootCount(-1, 6, -9), 1, "point 5");
    AssertEqual(GetDistinctRealRootCount(-9, 6, -1), 1, "point 6");
    AssertEqual(GetDistinctRealRootCount(-1, -6, -9), 1, "point 7");

}
void TestTwoRoots()
{
    AssertEqual(GetDistinctRealRootCount(2, 5, 2), 2, "point 1");
    AssertEqual(GetDistinctRealRootCount(2, 5, 3), 2, "point 2");
    AssertEqual(GetDistinctRealRootCount(3, 5, 2), 2, "point 3");
    AssertEqual(GetDistinctRealRootCount(2, 5, 0), 2, "point 4");
    AssertEqual(GetDistinctRealRootCount(2, -5, 3), 2, "point 5");
    AssertEqual(GetDistinctRealRootCount(-2, -5, 3), 2, "point 6");
    AssertEqual(GetDistinctRealRootCount(2, -5, -3), 2, "point 7");
    AssertEqual(GetDistinctRealRootCount(-2, 5, -3), 2, "point 8");
    AssertEqual(GetDistinctRealRootCount(-2, 5, 3), 2, "point 9");
    AssertEqual(GetDistinctRealRootCount(2, 5, -3), 2, "point 10");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestIsOnlyConstant_C, "TestIsOnlyConstant_C");
    runner.RunTest(TestLinerEquation, "TestLinerEquation");
    runner.RunTest(TestZeroRealRoots, "TestZeroRealRoots");
    runner.RunTest(TestOneRoot, "TestOneRoot");
    runner.RunTest(TestTwoRoots, "TestTwoRoots");
    
    return 0;
}
