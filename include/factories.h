#pragma once

#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <functional>
using namespace std;

/**
 * @brief Innerfactory example to construct a point in cartesian and polar coordinates.
 * 
 */
class Point
{
 

 private:
        class PointFactory
        {
          PointFactory();
         public:
                static Point NewCartesian(float x, float y);

                static Point NewPolar(float r, float theta);
        };

 public:
         Point(float x, float y);
         float x, y;

         friend ostream &operator<<(ostream& os, const Point& p);

        static PointFactory Factory;

};


struct IHotDrink
{
    virtual ~IHotDrink() = default; 
    virtual void prepare(int volume) = 0;//virtual alone doesn't enforce implementation, =0 enforces that any class deriving from the class that implements the method (through either public or protected) to implement some kind of behavior/body for this method.
};

struct Tea : IHotDrink
{
    void prepare(int volume) override {
        cout << "Take tea bag, boil water, pour" << volume << "ml, add some lemon\n";
    }

};

struct Coffee : IHotDrink
{
    void prepare(int volume) override {
        cout << "Grind some beans, boil water, pour" << volume << "ml, add some cream, enjoy\n";
    }

};

/**
 * @brief abstract factory
 * 
 */
struct IHotDrinkFactory
{
    virtual unique_ptr<IHotDrink> make() const = 0; //const memberfunctions can be called by any type of objects const and non-const
};

struct TeaFactory : IHotDrinkFactory
{
    unique_ptr<IHotDrink> make() const override {
        return make_unique<Tea>(); //constructs a tea object and wraps it in a unique_ptr
    }
};

struct CoffeeFactory : IHotDrinkFactory
{
    unique_ptr<IHotDrink> make() const override {
        return make_unique<Coffee>();
    }
};

class DrinkFactory
{
    map<string, unique_ptr<IHotDrinkFactory>> hot_factories;
   public:
    DrinkFactory()
    {
        hot_factories["Coffee"] = make_unique<CoffeeFactory>();
        hot_factories["tea"] = make_unique<TeaFactory>();
    }

    unique_ptr<IHotDrink> make_drink(const string& name)
    {
        auto drink = hot_factories[name]->make();
        drink->prepare(200);
        return drink;
    } 
};


/**
 * @brief more functional approach, not so much based on lot's of inheritance
 * 
 */
class DrinkWithVolumeFactory
{
    map<string, function<unique_ptr<IHotDrink>()>> factories;
 public:
    DrinkWithVolumeFactory()
    {
        factories["tea"] = [] {
            auto tea = make_unique<Tea>();
            tea->prepare(200);
            return tea;
        };
        factories["coffee"] = [] {
            auto coffee = make_unique<Coffee>();
            coffee->prepare(20);
            return coffee;
        };
    }

    unique_ptr<IHotDrink> make_drink(const string& name)
    {
        return factories[name]();
    }
};