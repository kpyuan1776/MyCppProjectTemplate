#pragma once


#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;
using namespace boost;


struct Address
{
    string street, city;
    int suite;

    Address();

    Address(const string& street, const string& city, int suite);

    Address(const Address& address);

    friend ostream &operator<<(ostream &os, const Address& address);

 private:
    friend class serialization::access;

    template <class archive>
        void serialize(archive& ar, const unsigned version)
    {
        ar & street;
        ar & city;
        ar & suite;
    }
};



struct Contact
{
    string name;
    Address* address;

    Contact();

    Contact(const string& name, Address *address);

    Contact(const Contact& other);

    friend ostream &operator<<(ostream &os, const Contact &contact);

    ~Contact();

 private:
    friend class serialization::access;

    //didn't split template into source an header files (otherwise you have to remove the template and just declare the function 
    //else one get's undefined reference to `void Contact::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int) ...)
    template <class archive>
        void serialize(archive& ar, const unsigned version)
    {
        ar & name;
        ar & address;
    }
};



struct EmployeeFactory
{
    static unique_ptr<Contact> new_main_office_employee (const string& name, const int suite);

 private:
    static unique_ptr<Contact> new_employee(const string& name, int suite, const Contact& prototype);

};

