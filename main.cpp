#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/program_options.hpp>

void writeJsonDataIntoFile()
{
    std::cout << "write json data" << std::endl;

    boost::property_tree::ptree  root, items;

    boost::property_tree::ptree item1;
    item1.put("id", "1");
    item1.put("name", "ibayside");
    items.push_back(std::make_pair("1", item1));

    boost::property_tree::ptree item2;
    item2.put("id", "2");
    item2.put("name", "wangyifan");
    items.push_back(std::make_pair("2", item2));

    root.put_child("users", items);
    boost::property_tree::write_json("../test.txt",root);
}

void readJsonDataFromFile()
{
    std::cout << "read json data" << std::endl;
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>("../test.txt", root);

    items = root.get_child("users");
    for (boost::property_tree::ptree::iterator it = items.begin(); it !=items.end(); ++it)
    {
        std::string key = it->first;
        std::string id  = it->second.get<std::string>("id");
        std::string name = it->second.get<std::string>("name");
        std::cout << "key: " << key.c_str() << '\t' ;
        std::cout << "id: " << id.c_str() << " name: " << name.c_str() << std::endl;

    }
    
    std::cout << "success." << std::endl;
}

int main(int argc, char** argv) 
{
    //json
    writeJsonDataIntoFile();
    readJsonDataFromFile();

    //
    namespace bpo = boost::program_options;
    bpo::options_description opts("all options");
    bpo::variables_map vm;

    opts.add_options()
    ("filename", bpo::value<std::string>(), "the file name which want to be loading")
    ("help", "this is a program to find a specified file");

    try
    {
        bpo::store(bpo::parse_command_line(argc, argv, opts), vm);
    }
    catch(...)
    {
        std::cout << "输入的参数中存在未定义的选项!" << std::endl;
    }

    if (vm.count("help"))
    {
        std::cout << opts << std::endl;
        return -1;
    }

    if (vm.count("filename"))
    {
        std::cout << "find " << vm["filename"].as<std::string>() << std::endl;
    }

    if (vm.empty())
    {
        std::cout << "no options found" << std::endl;
        return -1;
    }

    boost::property_tree::ptree pt;
    read_xml(vm["filename"].as<std::string>().c_str(), pt);

    std::cout << "ID is " << pt.get<int>("con.id") << std::endl;
    std::cout << "Try default " << pt.get<int>("con.no_prop", 100) << std::endl;
    
    boost::property_tree::ptree child = pt.get_child("con");
    std::cout << "name is :" << child.get<std::string>("name") << std::endl;

    child = pt.get_child("con.urls");
    for (BOOST_AUTO(pos, child.begin()); pos != child.end(); ++pos)
    {
        std::cout << "\t" + pos->second.data() << std::endl;
    }

    pt.put("con.name", "andy");
    pt.add("con.urls.url", "http://www.huanqiu.com");
    write_xml("cfg.xml", pt);

    getchar();

    return 0;
}
