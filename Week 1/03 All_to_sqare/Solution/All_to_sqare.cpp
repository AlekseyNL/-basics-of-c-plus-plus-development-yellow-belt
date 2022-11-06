#include <iostream>
#include <vector>
#include <map>
#include <ctime> 

using namespace std;

template<typename first, typename second>
pair<first, second> operator * (const pair<first, second>& lhs, const pair<first, second>& rhs);

template<typename T1, typename T2>
map<T1, T2> operator * (map<T1, T2>& lhs, map<T1, T2>& rhs);

template <typename T3> 
vector <T3> operator * (const vector <T3>& lhs, const vector <T3>& rhs);

template <typename T> 
T Sqr(const T& contaner);



//  operator * для pair уножает оба значения (first и second).
template<typename first, typename second>
pair<first, second> operator * (const pair<first, second>& lhs, const pair<first, second>& rhs)
{
    return pair<first, second> (lhs.first*rhs.first, lhs.second*rhs.second);
}


/*  operator * производит уножение для двух Map. При этом, уножение левого и правого
    Value производится по одинаковому номеру индекса, на который одновременно указывают 
    итераторы операндов (левого и правого). Результат присваивается возвращаемому значению 
    с Key равным Key левого операнда. 
*/ 
template<typename T1, typename T2>
map<T1, T2> operator * (const map<T1, T2>& lhs, const map<T1, T2>& rhs)
{
    map<T1, T2> squared;
    for (auto i = lhs.begin(), j=rhs.begin(); i != lhs.end() && j!=rhs.end();
        i++, j++)
    {
        squared[i->first] = i->second * j->second;
    }
    return squared;
}


/*  operator *= проиводит уножение двух контейнетов Vector.
    Находящиеся, по одинаковому номеру индекса, значения левого и правого 
    контейнера/операнда перемножаются.
*/
template <typename T3>
vector <T3> operator * (const vector <T3>& lhs, const vector <T3>& rhs)
{
    vector <T3> squared;
    for (int i = 0, j = 0; i < lhs.size() && j < rhs.size(); i++, j++)
    {
        squared.push_back (lhs[i] * rhs[j]);
    }
    return squared;
}


// Функция возведения в кадрат.
template <typename T> 
T Sqr(const T& contaner)
{
    return T (contaner * contaner); 
}


int main() {
    
    srand(time(0));
        
    int a = 8;
    cout << Sqr(a) << endl;  

    vector<int> v = { 1, 2, 3 };
    std::cout << "vector:";
    for (int& x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;
 
    vector<pair<int, int>> vp = { {4, 6}, {5, 7} };
    cout << "Pair:" << endl;
    for (auto& i : Sqr(vp))
    {
        cout << i.first << ' ' << i.second << endl;
    }

    map<int, pair<int, int>> map_of_pairs = {
        {1, {10, 11}},
        {2, {12, 13}},
        {3, {2, 2}},
        {4, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    
    vector <vector<int>> vv_int = { { 10, 20, 30 }, {1, 2, 3} };
    vector <vector<int>> vv = Sqr(vv_int);
    std::cout << " vector<int> in vector:\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << " " << vv[i][j];
        }
        cout << endl;
    }
    std::cout << endl;

    vector <map<int, pair<int, int>>> vmp =
    {
        { {1, {11, 11}}, {2, {12, 12}} },
        { {1, {2, 2}}  , {2, {4, 3}}   }
    };
    std::cout << " map<int, pair<int, int>> in vector:\n";
    for (auto& i1 : Sqr(vmp))
    {
        for (auto& i2 : i1)
        {
            cout << i2.first << " " << i2.second.first << " " << i2.second.second << endl;
        }
    }
    std::cout << endl;
  
    // время работы программы
    cout << "runtime = " << clock() / 10000.0 << endl;  
  
    return 0;
}

