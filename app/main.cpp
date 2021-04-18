#include <iostream>
#include "singleton.h"
#include "prototypes.h"
#include "factories.h"
#include "my_lib.h"
#include "html_lib.h"
#include "di.h"
#include "myIterator.h"
#include <memory>

/*
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
*/


#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>

//DFS boost application
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/range/irange.hpp>
#include <boost/pending/indirect_cmp.hpp>

using namespace std;
using namespace boost;


enum class OutputFormat
{
    markdown,
    html
};

struct ListStrategy
{
    virtual void start(ostringstream& oss) {}
    virtual void add_list_item(ostringstream& oss, const string& item) = 0;
    virtual void end(ostringstream& oss) {}
};

//low level details
struct MarkdownListStrategy : ListStrategy
{
    void add_list_item(ostringstream& oss, const string& item) override
    {
        oss << "* " << item << "\n";
    }

};

struct HtmlListStrategy : ListStrategy
{
    void start(ostringstream &oss) override
    {
        oss << "<ul>\n";
    }

    void add_list_item(ostringstream& oss, const string& item) override
    {
        oss << "   <li>" << item << "</li>\n";
    }


    void end(ostringstream &oss) override
    {
        oss << "</ul>\n";
    }

};

//high level stuff
struct TextProcessor
{
    void clear()
    {
        oss.str("");
        oss.clear();
    }

    void append_list(const vector<string>& items)
    {
        list_strategy->start(oss);
        for (auto& item : items)
            list_strategy->add_list_item(oss, item);
        list_strategy->end(oss);
    }

    void set_output_format(const OutputFormat& format)
    {
        switch(format)
        {
            case OutputFormat::markdown:
                list_strategy = make_unique<MarkdownListStrategy>();
                break;
            case OutputFormat::html:
                list_strategy = make_unique<HtmlListStrategy>();
                break;
        }
    }

    string str() const {return oss.str(); }

  private:
    ostringstream oss;
    unique_ptr<ListStrategy> list_strategy;
};


class FloodFillExample
{
    void dfs(vector<vector<int>>& image, int sr, int sc, int newColor, int rows, int cols, int source)
    {
        if(sr<0 || sr>=rows || sc <0 || sc>=cols)
            return;
        else if(image[sr][sc]!=source)
            return;

        image[sr][sc] = newColor;

        dfs(image,sr-1,sc,newColor,rows,cols,source);
        dfs(image,sr+1,sc,newColor,rows,cols,source);
        dfs(image,sr,sc-1,newColor,rows,cols,source);
        dfs(image,sr,sc+1,newColor,rows,cols,source);
    }

  public:
    FloodFillExample(){}

    void floodFill(vector<vector<int>>& image, int sr, int sc, int newColor)
    {
        if(newColor==image[sr][sc])
            return;
        int rows = image.size();
        int cols = image[0].size();
        int source = image[sr][sc];
        dfs(image,sr,sc,newColor,rows,cols,source);

    }

    void initializeImage(vector<vector<int>>& image, int rows, int cols)
    {
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                if(i>=10 || j>=10 || i<200 || j< 500)
                    image[i][j] = 1;
                else
                    image[i][j] = rand()%10 +1;
            }
        }
    }


};




//https://www.boost.org/doc/libs/1_67_0/libs/graph/example/dfs-example.cpp
template <typename TimeMap> 
class dfs_time_visitor: public default_dfs_visitor
{
    //some explanation: (https://stackoverflow.com/questions/18385418/c-meaning-of-a-statement-combining-typedef-and-typename)
    //typedef as usual defines here a shorthand new type, here T
    //typename here is letting the compiler know that value_type is a type and not a static member of  property_traits<TimeMap>
    typedef typename property_traits<TimeMap>::value_type T;
  
  public:
    dfs_time_visitor(TimeMap dmap, TimeMap fmap, T& t): m_dtimemap(dmap), m_ftimemap(fmap), m_time(t)
    {}

    template <typename Vertex, typename Graph>
    void discover_vertex(Vertex u, const Graph& g) const
    {
        put(m_dtimemap, u, m_time++);
    }

    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph& g) const
    {
        put(m_ftimemap, u, m_time++);
    }

    TimeMap m_dtimemap;
    TimeMap m_ftimemap;
    T& m_time;
};


void checkCppCopyIf(const vector<string>& owners, const vector<string>& pets)
{
    auto dog_owners = vector<string>{};

    copy_if(
        owners.cbegin(),
        owners.cend(),
        back_inserter(dog_owners),
        [pet = pets.cbegin()](auto const& owner) mutable { //mark lambda mutable and capture an iterator into pets vector, incrementing it to check if animal is dog
            return *pet++ == "Dog"; //check some lambda stuff: https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-160auto const owners = vector<string>{"Ashwin", "Leslay", "sarah"};
        }
    );

    cout << "dog owners: " << endl;
    for (const auto& downer : dog_owners)
    {
        cout << downer << ", ";
    }
    cout << endl;
}

//add some flood fill stuff
//or a variation like finding the max number of connected boxes
// recursive (disadvantage it uses stackspace -> stack overflow)
// iteratively (not bound by stack space)
// 2D array of integers (integer=color)
// DFS is O(V+E), useful for counting connected graphs

int main()
{

    BOOST_LOG_TRIVIAL(info) << "start floodfill example";
    int numrows = 30000; //mit 30000x30000 braucht er schon 4GB RAM
    int numcols = 30000;
    vector<vector<int>> img(numrows, vector<int>(numcols));
    /*vector<vector<int>> img(numrows);
    vector<vector<int>> res(numrows);
    for(int i=0; i<numrows; i++)
    {
        img[i].resize(numcols);
        res[i].resize(numcols);
    }*/
    FloodFillExample floodfill{};
    floodfill.initializeImage(img, numrows, numcols);
    //floodfill.floodFill(img, 10, 10, 5); //gives segmentation fault at rows,cols > 400
    int rows = img.size();
    int cols = img[0].size();
    /*int ctr=0;
        for (int i=0; i<15;i++)
        {
            for (int j=0; i<15;j++)
            {
                ctr = img[i][j];
                cout << ctr << ', ';
            }
            cout << endl;
        }*/

    BOOST_LOG_TRIVIAL(info) << "start boost DFS example";
    typedef adjacency_list<vecS, vecS, directedS> graph_t;
    typedef graph_traits <graph_t>::vertices_size_type size_type;

    //set up the vertex names
    enum {u, v, w, x, y, z, N};
    char name[] = { 'u', 'v', 'w', 'x', 'y', 'z' };
     // Specify the edges in the graph
    typedef std::pair < int, int >E;
    E edge_array[] = { E(u, v), E(u, x), E(x, v), E(y, x),
    E(v, y), E(w, y), E(w, z), E(z, z)
    };
    graph_t g(edge_array, edge_array + sizeof(edge_array) / sizeof(E), N); //maybe requires extra handling for mvsc
    // discover time and finish time properties
    std::vector < size_type > dtime(num_vertices(g));
    std::vector < size_type > ftime(num_vertices(g));
    typedef iterator_property_map<std::vector<size_type>::iterator,property_map<graph_t, vertex_index_t>::const_type> time_pm_type;
    time_pm_type dtime_pm(dtime.begin(), get(vertex_index, g));
    time_pm_type ftime_pm(ftime.begin(), get(vertex_index, g));
    size_type t = 0;
    dfs_time_visitor < time_pm_type >vis(dtime_pm, ftime_pm, t);

    depth_first_search(g, visitor(vis));

    // use std::sort to order the vertices by their discover time
    std::vector < size_type > discover_order(N);
    integer_range < size_type > r(0, N);
    std::copy(r.begin(), r.end(), discover_order.begin());
    std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp < time_pm_type, std::less < size_type > >(dtime_pm));
    std::cout << "order of discovery: ";
    int i;
    for (i = 0; i < N; ++i)
        std::cout << name[discover_order[i]] << " ";

    std::vector < size_type > finish_order(N);
    std::copy(r.begin(), r.end(), finish_order.begin());
    std::sort(finish_order.begin(), finish_order.end(),
            indirect_cmp < time_pm_type, std::less < size_type > >(ftime_pm));
    std::cout << std::endl << "order of finish: ";
    for (i = 0; i < N; ++i)
        std::cout << name[finish_order[i]] << " ";
    std::cout << std::endl;

    BOOST_LOG_TRIVIAL(info) << "finished boost DFS example";

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

    
    //strategy pattern
    BOOST_LOG_TRIVIAL(info) << "strategy pattern stuff";
    vector<string> items{"foo", "bar", "baz"};
    TextProcessor tp;
    tp.set_output_format(OutputFormat::markdown);
    tp.append_list(items);
    cout << tp.str() << "\n";
    tp.clear();
    tp.set_output_format(OutputFormat::html);
    tp.append_list(items);
    cout << tp.str() << "\n";



    //iterators
    vector<string> names {"John", "jane", "jill", "jack"};
    vector<string>::iterator it = names.begin();
    cout << "first name is " << *it << endl;

    ++it;
    it->append(string(" goodall"));
    cout << "second name is " << *it << endl;

    while(++it !=names.end())
    {
        cout << "another name: " << *it << endl;
    }

    for (auto ri = names.rbegin(); ri !=names.rend(); ++ri) //rbegin begins on the last element
    {
        cout << *ri;
        if (ri+1 != names.rend())
            cout << ", ";
    }
    cout << endl;
    BinaryTree<string> family {
        new Node<string>{"me",
                        new Node<string>{"mother",
                                    new Node<string>{"mother's mother"},
                                    new Node<string>{"mother's father"}
                        },
                        new Node<string>{"father"}
        }
    };

    for(auto it = family.begin(); it !=family.end(); ++it)
    {
        cout << (*it).value << "\n";
    }
    cout << "iterator stuff finished ...." << endl;

    //some cpp arrays stuff
    auto const owners = vector<string>{"Ashwin", "Leslay", "sarah"};
    auto const pets = vector<string>{"Dog", "Cat", "Dog"};
    checkCppCopyIf(owners, pets);


    cout_hello_world();
    linalg::aliases::float3  somevector = print_linalg_vector();
    print_boost_version();

    return 0;
}