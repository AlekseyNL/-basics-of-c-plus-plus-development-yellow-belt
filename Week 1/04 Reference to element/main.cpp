#include <iostream>
#include <map>
#include <cstring>


using namespace std;

template <typename T1, typename T2>
T2& GetRefStrict(map <T1, T2>& collection, T1 key)
{
    try {
        return collection.at(key);
    }
    catch (out_of_range& e) {
        throw runtime_error("Out of Rang");
    }
}

int main()
{
    map<int, string> m = { {0, "value"} };
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
}
