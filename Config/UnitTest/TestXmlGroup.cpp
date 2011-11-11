#include <string>
#include <iostream>
#include <fstream>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <XmlGroup.h>
 
using namespace Config;
using namespace rapidxml;
using namespace std;

void writeXml(XmlGroup& group)
{
    xml_document<> doc;  

    xml_node<>* rot = doc.allocate_node(node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(rot);

    xml_node<>* node =  doc.allocate_node(node_element,"Configuration",NULL);  
    doc.append_node(node);

    node->append_node(group.genNode(&doc));

    string text;  
    rapidxml::print(back_inserter(text), doc, 0);  

    cout << text << endl; 

    ofstream out("TestXmlGroup.xml");
    out  <<  doc;
    out.close();
}

void readXml(XmlGroup& group)
{
    file<> fdoc("TestRapidXml.xml");  
    xml_document<>  doc;      
    doc.parse<0>(fdoc.data());   

    cout << "doc.name:" << doc.name() << endl;  

    xml_node<>* root = doc.first_node();  
    cout << "root.name:" << root->name() << endl;  

    group.parse(root->first_node());
}

int main()
{
    XmlGroup group;
    readXml(group);
    writeXml(group);
    return 0;
}

