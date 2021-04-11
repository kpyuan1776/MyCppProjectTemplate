#include "html_lib.h"

HtmlElement::HtmlElement() {}

HtmlElement::HtmlElement(const string& name, const string& text)
    : name(name),text(text)
{
}

// .declare const to use in const and non-const objects. In C++ an object declared as const cannot be modified and hence, can invoke only const member functions as these functions ensure not to modify the object. 
string HtmlElement::str(int indent) const
{
    ostringstream oss;
    string i(indent_size*indent, ' ');
    oss << i << "<" << name << ">" << endl;
    if (text.size() > 0)
      oss << string(indent_size*(indent + 1), ' ') << text << endl;

    for (const auto& e : elements)
      oss << e.str(indent + 1);

    oss << i << "</" << name << ">" << endl;
    return oss.str();
}

unique_ptr<HtmlBuilder> HtmlElement::create(string root_name)
{
    return make_unique<HtmlBuilder>(root_name); 
}

HtmlBuilder::HtmlBuilder(string root_name)
  {
    root.name = root_name;
  }


/**
 * @brief returns an HtmlBuilder to create a fluent interface (fluent builder, allowing the chaining of add by the dots). Thus the result of the call is a reference to HtmlBuilder and this can be used to call it again
 * 
 * @return HtmlBuilder&
 */
HtmlBuilder& HtmlBuilder::add_child(string child_name, string child_text)
  {
    HtmlElement e{ child_name, child_text };
    root.elements.emplace_back(e); //appends a new element to the end of the container
    return *this;
  }

HtmlBuilder* HtmlBuilder::add_child_2(string child_name, string child_text)
  {
    HtmlElement e{ child_name, child_text };
    root.elements.emplace_back(e);
    return this;
  }

HtmlElement HtmlBuilder::build() {return root;}


string HtmlBuilder::str() { return root.str(); }