#include <iostream>
#include "singleton.h"
#include "prototypes.h"
#include "factories.h"
#include "my_lib.h"
#include "html_lib.h"
#include "di.h"
#include <memory>


#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>



using namespace std;
using namespace boost;






int main()
{

 
    // easier
    HtmlBuilder builder{ "ul" };
    builder.add_child("li", "hello").add_child("li", "world");
    cout << builder.str() << endl;


    auto builder2 = HtmlElement::create("ul")
    ->add_child_2("li", "hello")->add_child_2("li", "my name is");
    cout << builder2 << endl;


    auto p = Point::Factory.NewCartesian(1,2);
    auto p2 = Point::Factory.NewPolar(0.5,0.5);
    cout<< p << endl;

    //abstract factory -> yields Segmentation fault (core dumped)
    // DrinkFactory df;
    // auto c = df.make_drink("coffee");
    DrinkWithVolumeFactory df;
    auto c = df.make_drink("coffee");

    //prototype
    Contact john{"John Doe", new Address{"123 East Dr ", "street", 123}};
    Contact jane{john};
    jane.name = "Jane Smith";
    jane.address->suite = 103;
    cout << john << endl << jane << endl;

    auto john2 = EmployeeFactory::new_main_office_employee("John", 123);
    cout << *john2 << endl;
    

    auto clone = [](const Contact& c)
    {
        ostringstream oss;
        archive::text_oarchive oa(oss);
        oa << c;
        string s = oss.str();
        cout << s << endl; //just check raw representation

        istringstream iss(s);
        archive::text_iarchive ia(iss);
        Contact result;
        ia >> result;
        return result;
    };
    auto johnserial = EmployeeFactory::new_main_office_employee("John", 123);
    auto janeserial = clone(*johnserial);
    janeserial.name = "Jane";
    cout << *johnserial << "\n" << janeserial << "\n";


    //singleton
    string city = "Tokyo";
    cout << city << " has population " << 
        SingletonDatabase::get().get_population(city) << endl;

    //singleton CI style
    auto injector = di::make_injector(
        di::bind<IFoo>().to<Foo>().in(di::singleton)
    );
    auto bar1 = injector.create<std::shared_ptr<Bar>>();
    auto bar2 = injector.create<std::shared_ptr<Bar>>();
    cout << bar1->foo->name() << endl;
    cout << bar2->foo->name() << endl;


    cout_hello_world();
    linalg::aliases::float3  somevector = print_linalg_vector();
    print_boost_version();

    return 0;
}