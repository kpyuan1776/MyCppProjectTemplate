#include <string>
#include <ostream>
#include <list>
#include <iostream>
using namespace std;

struct Attribute
{
    string type;
    string name;
    
    Attribute(string t, string n): type(t), name(n) {};
};


class CodeBuilder
{
    string classname;
    list<Attribute> attributes; 
public:
  CodeBuilder(const string& class_name)
  {
      classname = class_name;
  }


  CodeBuilder& add_field(const string& name, const string& type)
  {
    attributes.push_back(Attribute{type,name});
    return *this;
  }


  friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
  {
    os << "class " << obj.classname << "\n";
    os << "{\n";
    for(auto const& el : obj.attributes)
    {
        os << "  " << el.type << " " << el.name << ";\n";
    }
    os << "};";
  }
};


int main()
{
    auto cb = CodeBuilder{"Person"}.add_field("name", "string").add_field("age", "int");
    cout << cb;
}