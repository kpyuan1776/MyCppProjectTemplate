#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

class HtmlBuilder; //cpp forward declaration

struct HtmlElement
{
  string name;
  string text;
  vector<HtmlElement> elements;
  const size_t indent_size = 2;

  HtmlElement();
  HtmlElement(const string& name, const string& text);


  string str(int indent = 0) const;

  static unique_ptr<HtmlBuilder> create(string root_name);
};





class HtmlBuilder
{
private:
  HtmlElement root;
public:
  HtmlBuilder(string root_name);



  HtmlBuilder& add_child(string child_name, string child_text);

  // pointer based
  HtmlBuilder* add_child_2(string child_name, string child_text);

  HtmlElement build();

  string str();

  operator HtmlElement() const { return root; }
  
};