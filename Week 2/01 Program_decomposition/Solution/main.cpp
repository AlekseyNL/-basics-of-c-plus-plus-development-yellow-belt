#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>
#include <chrono>


using namespace std;

// Перечисление хранит типы запросов
enum class QueryType {
    NewBus,         // добавить маршрут автобуса

    BusesForStop,   // вывести названия всех маршрутов автобуса, 
                    // проходящих через остановку

    StopsForBus,    // вывести названия всех остановок маршрута  
                    // со списком автобусов, на которые можно пересесть 
                    // на каждой из остановок.

    AllBuses        // вывести список всех маршрутов с остановками
};

// Атрибуты запроса
struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string typeQuery;
    is >> typeQuery;

    if (typeQuery == "NEW_BUS")
    {
        q.type = QueryType::NewBus;
        is >> q.bus >> q.stop;

        q.stops.clear();
        int vector_size = stoi(q.stop);
        for (int i = 0; i < vector_size; i++)
        {
            string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
    }

    if (typeQuery == "BUSES_FOR_STOP")
    {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }

    if (typeQuery == "STOPS_FOR_BUS")
    {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }

    if (typeQuery == "ALL_BUSES")
    {
        q.type = QueryType::AllBuses;
    }

    return is;
}

// Остановка и список автобусов, проезжающих через эту остановку
struct BusesForStopResponse {
    string stop;
    vector<string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty())
    {
        os << "No stop";
    }
    else
    {
        for (const auto& bus : r.buses)
        {
            os << bus << " ";
        }
    }
    return os;
}

// Маршрут bus со всеми его остановоками stops и марщруты на которые 
// можно пересесть на этих остановках
struct StopsForBusResponse {
    string bus;
    vector<string> stops;
    map<string, vector<string>> buses_to_stop;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {

    if (r.stops.empty())
    {
        os << "No bus";
    }
    else
    {
        bool line_feed = false;     //  для форматирования вывода чтоб
                                    //  небыло лишних переводов строк

        for (const string& stop : r.stops)
        {
            if (line_feed)
            {
                os << "\n";
            }

            os << "Stop " << stop << ": ";
            if (r.buses_to_stop.at(stop).size() == 1)
            {
                os << "no interchange";
            }
            else
            {
                for (const string& other_bus : r.buses_to_stop.at(stop)) {
                    if (r.bus != other_bus)
                    {
                        os << other_bus << " ";
                    }
                }
            }
            line_feed = true;
        }
    }
    return os;
}

// Марщруты всех автобусов со списком их остановок
struct AllBusesResponse {
    const map<string, vector<string>>& stops_to_bus;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {

    if (r.stops_to_bus.empty())
    {
        os << "No buses";
    }
    else
    {
        bool line_feed = false;     // для форматирования вывода чтоб
                                    // небыло лишних переводов строк

        for (const auto& bus_item : r.stops_to_bus)
        {
            if (line_feed)
            {
                os << "\n";
            }

            os << "Bus " << bus_item.first << ": ";
            for (const string& stop : bus_item.second)
            {
                os << stop << " ";
            }
            line_feed = true;
        }
    }
    return os;
}

class BusManager {
public:
    /*  Добавляет маршрут автобуса bus и остановки  stop1, stop2 ...
    */
    void AddBus(const string& bus, const vector<string>& stops) {

        m_stops_to_bus[bus] = stops;

        for (const auto& stop : stops)
        {
            m_buses_to_stop[stop].push_back(bus);
        }
    }

    /*  Выводит список автобусов, проезжающих через эту остановку.
        Если остановка stop не существует, выведит No stop.
    */
    BusesForStopResponse GetBusesForStop(const string& stop) const {

        if (!m_buses_to_stop.count(stop))
        {
            return BusesForStopResponse{ };
        }
        return BusesForStopResponse{ stop, m_buses_to_stop.at(stop) };
    }

    /*  Выводит названия всех остановок маршрута bus со списком
        автобусов, на которые можно пересесть на каждой из остановок.
    */
    StopsForBusResponse GetStopsForBus(const string& bus) const {

        if (!m_stops_to_bus.count(bus))
        {
            return StopsForBusResponse{ };
        }
        const vector<string>& stops = m_stops_to_bus.at(bus);
        return StopsForBusResponse{ bus, stops, m_buses_to_stop };
    }

    /* Марщруты всех автобусов со спискои остановок.
       Если автобусы отсутствуют, выводит No buses
    */
    AllBusesResponse GetAllBuses() const {

        const map<string, vector<string>>& ref_m_stops_to_bus = m_stops_to_bus;
        return AllBusesResponse{ ref_m_stops_to_bus };
    }

private:
    map<string, vector<string>> m_buses_to_stop, m_stops_to_bus;
};


bool operator == (Query aa, Query bb)
{
    if (aa.bus == bb.bus && aa.stop == bb.stop && aa.stops == bb.stops)
    {
        if (aa.type == bb.type)
        {
            return true;
        }
    }
    return false;
}
void TestQuery_1_()
{
    string newBus = "NEW_BUS 0 2 a b";

    Query aa{ QueryType::NewBus, "0", "2", { "a", "b"} };

    Query bb;

    stringstream ss;
    ss << newBus;
    while (ss)
    {
        ss >> bb;
    }

    assert(aa == bb);
    cout << " TestQuery_1_ is Ok" << endl;
}

void TestBusesForStopResponse_2_()
{
    {
        string is_Empty = "No stop";

        BusesForStopResponse aa;

        stringstream ss;
        ss << aa;
        assert(is_Empty == ss.str());
    }
    {
        string is_Not_Empty = "1 2 0 ";

        BusesForStopResponse aa;
        aa.buses = { "1", "2", "0" };

        stringstream ss;
        ss << aa;
        assert(is_Not_Empty == ss.str());
    }
    cout << " TestBusesForStopResponse_2_ is Ok" << endl;
}

void TestStopsForBusResponse_3_()
{
    {
        string no_interchange = "Stop b: no interchange";

        StopsForBusResponse aa{ "1", { "b" }, { {"b", {"1"}} } };

        stringstream ss;
        ss << aa;
        assert(no_interchange == ss.str());
    }
    {
        string interchange = "Stop b: no interchange\nStop c: 2 3 \nStop d: 0 ";

        StopsForBusResponse aa{ "1",
                                 { "b", "c", "d" },
                                 {
                                    {"b", {"1"}},
                                    {"c", {"1", "2", "3"}},
                                    {"d", {"0", "1"}}
                                 }
                              };
        stringstream ss;
        ss << aa;
      
        assert(interchange == ss.str());
    }
    cout << " TestStopsForBusResponse_3_ is Ok" << endl;
}
/*
bool operator == (BusManager aa, BusManager bb)
{
    if (aa.m_buses_to_stop == bb.m_buses_to_stop &&
        aa.m_stops_to_bus == bb.m_stops_to_bus)
    {
        return true;
    }
    return false;
}
void TestAddBus_4_()
{
    BusManager aa;
    aa.m_buses_to_stop = { {"s", {"1"}}, {"d", {"1"}} };
    aa.m_stops_to_bus = { {"1", {"s", "d"}} };

    BusManager bb;
    vector<string> stops{ "s", "d" };
    string bus = "1";

    bb.AddBus(bus, stops);

    assert(aa == bb);
    cout << " TestAddBus_4_ is Ok" << endl;
}
*/
void TestGetBusesForStop_5_()
{
    {
        string is_Empty = "No stop";

        BusManager bm;

        Query q;
        q.stop = "c";
        stringstream ss;
        ss << bm.GetBusesForStop(q.stop);

        assert(is_Empty == ss.str());
    }
    {
        string is_Not_Empty = "1 3 ";

        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "a", "b" });
        bm.AddBus("3", { "b" , "c" });

        Query q;
        q.stop = "c";
        stringstream ss;
        ss << bm.GetBusesForStop(q.stop);

        assert(is_Not_Empty == ss.str());
    }
    cout << " TestGetBusesForStop_5_ is Ok" << endl;
}

void TestGetStopsForBus_6_()
{
    {
        Query q;
        q.bus = "1";

        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "c", "b" });
        bm.AddBus("3", { "d" , "c" });

        string interchange = "Stop a: no interchange\nStop b: 2 \nStop c: 2 3 ";
        stringstream ss;
        ss << bm.GetStopsForBus(q.bus);
        assert(interchange == ss.str());
    }
    {
        Query q;
        q.bus = "8";

        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "c", "b" });
        bm.AddBus("3", { "d" , "c" });

        string no_such_bus = "No bus";
        stringstream ss;
        ss << bm.GetStopsForBus(q.bus);
        assert(no_such_bus == ss.str());
    }
    cout << " TestGetStopsForBus_6_ is Ok" << endl;
}

void TestGetAllBuses_7_()
{
    {
        BusManager bm;
        string isEmpty = "No buses";

        stringstream ss;
        ss << bm.GetAllBuses();
        assert(isEmpty == ss.str());
    }
    {
        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "c", "b" });
        bm.AddBus("3", { "d" , "c" });

        string bus_and_stops = "Bus 1: a b c \nBus 2: c b \nBus 3: d c ";
        stringstream ss;
        ss << bm.GetAllBuses();
        assert(bus_and_stops == ss.str());
    }
    cout << " TestGetAllBuses_7_ is Ok" << endl;
}

void TestCOUT_8_()
{
    {
        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "c", "b" });
        bm.AddBus("3", { "d" , "c" });

        Query q;
        q.stop = "b";
        q.bus = "2";

        string must_be1 = "1 2 ";
        cout << must_be1 << " = ";
        cout << bm.GetBusesForStop(q.stop) << endl;

        string must_be2 = "Stop c: 1 3 \nStop b: 1 \n";
        cout << must_be2 << "equal\n";
        cout << bm.GetStopsForBus(q.bus) << endl;
    }
    {
        BusManager bm;
        bm.AddBus("1", { "a", "b" , "c" });
        bm.AddBus("2", { "c", "b" });
        bm.AddBus("3", { "d" , "c" });

        Query q;
        q.stop = "e";
        q.bus = "5";

        string must_be1 = "No stop";
        cout << must_be1 << " = ";
        cout << bm.GetBusesForStop(q.stop) << endl;

        string must_be2 = "No bus";
        cout << must_be2 << " = ";
        cout << bm.GetStopsForBus(q.bus) << endl;
    }
    cout << " TestCOUT_8_ is OK" << endl;
}

void AllTest()
{
    TestQuery_1_();
    TestBusesForStopResponse_2_();
    TestStopsForBusResponse_3_();
    //TestAddBus_4_();
    TestGetBusesForStop_5_();
    TestGetStopsForBus_6_();
    TestGetAllBuses_7_();
    //TestCOUT_8_();
}

int main() {

    //AllTest();
    //return 0;

    int query_count;
    Query q;

    cin >> query_count;

    auto start = chrono::high_resolution_clock::now();  // Время начала выполнения программы

    BusManager bm;
    for (int i = 0; i < query_count; ++i)
    {
        cin >> q;
        switch (q.type)
        {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();    // Время окончания выполненмя программы
    chrono::duration<float> duration = end - start;     // Продрлжительность выполнения программы 

    cout << "Duration: " << duration.count() << "s" << endl;

    return 0;
}
