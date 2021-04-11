#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <map>

#include <boost/lexical_cast.hpp>

#include "di.h"

using namespace std;
using namespace boost;


class Database
{
  public:
    virtual int get_population(const string& name) = 0;
};

class SingletonDatabase : public Database
{
    SingletonDatabase();

    map<string, int> capitals;
  public:
    SingletonDatabase(SingletonDatabase const&) = delete;
    void operator=(SingletonDatabase const&) = delete;

    static SingletonDatabase& get();

    int get_population(const string& name);

};


class DummyDatabase: public Database
{
    map<string,int> capitals;
  public:
    DummyDatabase() {
        capitals["alpha"] = 1;
        capitals["beta"] = 2;
        capitals["gamma"] = 3;
    }

    int get_population(const string& name) override {
        return capitals[name];
    }
};

struct SingletonRecordFinder
{
    int total_population(vector<string> names);
};


struct ConfigurableRecordFinder
{
    Database& db;

    ConfigurableRecordFinder(Database &db): db(db) {}

    int total_population(vector<string> names)
    {
        int result{0};
        for (auto& name : names)
            result += db.get_population(name);
        return result;
    }
};


namespace { //anonymous namespace required to prevent the error: for initilization of static field ctr: multiple definition of `Foo::ctr'
struct IFoo
{
    virtual string name() = 0;
};


struct Foo : IFoo
{
    static int ctr;
    Foo() {++ctr; }

    string name() override {
        return "foo"s + lexical_cast<string>(ctr);
    }
};

int Foo::ctr = 0;

struct Bar
{
    std::shared_ptr<IFoo> foo;
};
}

