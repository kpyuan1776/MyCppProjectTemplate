#include "prototypes.h"



Address::Address(){}

Address::Address(const string& street, const string& city, int suite) : street(street), city(city), suite(suite){}

Address::Address(const Address& address): street{address.street},city{address.city},suite{address.suite} {}

ostream &operator<<(ostream &os, const Address& address){
        os << "street: " << address.street << " city: " << address.city << " suite: " << address.suite;
        return os;
    }

/*template <class archive>
void Address::serialize(archive& ar, const unsigned version)
{
    ar & street;
    ar & city;
    ar & suite;
}
*/



Contact::Contact() {}

Contact::Contact(const string& name, Address *address): name(name), address(address) {}

Contact::Contact(const Contact& other) : name {other.name}, address {new Address {*other.address}} {}

Contact::~Contact() {delete address; }

ostream &operator<<(ostream &os, const Contact &contact){
        os << "name: " << contact.name << " address: " << *contact.address;
        return os;
    }

/*
template <class archive>
void Contact::serialize(archive& ar, const unsigned version)
{
    ar & name;
    ar & address;
}
*/





unique_ptr<Contact> EmployeeFactory::new_main_office_employee (const string& name, const int suite)
{
    static Contact p{"", new Address{"123 East Dr", "London", 0}};
    return new_employee(name, suite, p);
}


unique_ptr<Contact> EmployeeFactory::new_employee(const string& name, int suite, const Contact& prototype)
{
    auto result = make_unique<Contact>(prototype);
    result->name = name;
    result->address->suite = suite;
    return result;
}
