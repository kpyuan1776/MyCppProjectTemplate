#include "singleton.h"


SingletonDatabase::SingletonDatabase()
    {
        cout << "Initilize database\n";
        ifstream ifs("capitals.txt");

        string s, s2;

        while (getline(ifs, s))
        {
            getline(ifs, s2);
            int pop = lexical_cast<int>(s2);
            capitals[s] = pop;
        }
    }

SingletonDatabase& SingletonDatabase::get()
    {
        static SingletonDatabase db;
        return db;
    }

int SingletonDatabase::get_population(const string& name)
    {
        return capitals[name];
    }

int SingletonRecordFinder::total_population(vector<string> names)
{
    int result{0};
    for (auto& name : names)
        result += SingletonDatabase::get().get_population(name);
    return result;
}