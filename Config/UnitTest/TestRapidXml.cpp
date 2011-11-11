#include <string>
#include <iostream>
#include <fstream>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
 
using namespace rapidxml;
using namespace std;

void writeXml()
{
    xml_document<> doc;  

    xml_node<>* rot = doc.allocate_node(node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(rot);

    xml_node<>* node =  doc.allocate_node(node_element,"Configuration",NULL);  
    doc.append_node(node);

    xml_node<>* group =  doc.allocate_node(node_element,"Group",NULL);  
    group->append_attribute(doc.allocate_attribute("name","Test Group"));
    node->append_node(group);

    xml_node<>* param =  doc.allocate_node(node_element,"Parameter",NULL);  
    param->append_attribute(doc.allocate_attribute("name","Test Parameter"));
    param->append_attribute(doc.allocate_attribute("id","test.test"));
    param->append_node(doc.allocate_node(node_element, "Description", "the description"));
    param->append_node(doc.allocate_node(node_element,"ValueType","Int"));
    param->append_node(doc.allocate_node(node_element,"ValueRange","0-100"));
    param->append_node(doc.allocate_node(node_element,"DefaultValue","0"));
    param->append_node(doc.allocate_node(node_element,"Value","0"));
    group->append_node(param);

    string text;  
    rapidxml::print(back_inserter(text), doc, 0);  

    cout << text << endl; 

    ofstream out("TestRapidXml.xml");
    out  <<  doc;
    out.close();
}

void readXml()
{
    file<> fdoc("TestRapidXml.xml");  
    xml_document<>  doc;      
    doc.parse<0>(fdoc.data());   

    cout << "doc.name:" << doc.name() << endl;  

    xml_node<>* root = doc.first_node();  
    cout << "root.name:" << root->name() << endl;  

    xml_node<>* node1 = root->first_node();  
    cout << "node1.name:" << node1->name() << endl;   

    xml_node<>* node11 = node1->first_node();  
    cout << node11->name() << endl;  
    cout << node11->value() << endl;

}

int main()
{
    writeXml();
    readXml();
    return 0;
}
