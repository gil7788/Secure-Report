#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <iostream>
#include <vector>


using namespace std;

class JsonParser{
    class MyPair{
    public:
        string _key;
        string _value;

        MyPair(std::string key, std::string value): _key{key}, _value{value} {}
    };

    vector<MyPair> _values;

public:
    void add_value(const string& key, const string& value);

    string to_json();

    static string vector_to_json_string(vector<string>& v);

private:
    string add_first_json_element(string name, string value);

    string add_middle_json_element(string name, string value);

    string add_last_json_element(string name, string value);

    string add_single_json_element_content(string name, string value);

    string add_json_element_content(string name, string value);
};

#endif //JSONPARSER_H