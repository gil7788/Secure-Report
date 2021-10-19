#include "JsonParser.h"


void JsonParser::add_value(const string& key, const string& value) {
    MyPair item(key ,value);
    _values.push_back(item);
}

string JsonParser::to_json() {
    if(_values.size() == 0) {
        string json = "";
        return json;
    }
    
    if(_values.size() == 1) {
        string key = _values[0]._key;
        string value = _values[0]._value;
        string json = add_single_json_element_content(key, value);
        return json;
    }
    
    string first_key = _values[0]._key;
    string first_value = _values[0]._value;
    string json = add_first_json_element(first_key, first_value);

    for(int i = 1; i < _values.size()-1; ++i) {
        string key = _values[i]._key;
        string value = _values[i]._value;
        json += add_middle_json_element(key, value);
    }
    
    int last_index = _values.size()-1;
    string last_key = _values[last_index]._key;
    string last_value = _values[last_index]._value;
    json +=  add_last_json_element(last_key, last_value);

    return json;
}

string JsonParser::vector_to_json_string(vector<string>& v) {
    if(v.size() == 0) {
        string json = "[]";
        return json;
    }
    string json = "[ \n";

    for (int i = 0; i < v.size() - 1; ++i) {
        json += v[i] + ", \n";
    }
    int last_index = v.size()-1;
    json += v[last_index] + "] \n";
    return json;
}

string JsonParser::add_first_json_element(string name, string value) {
    string json = "{\n" + add_json_element_content(name, value) + ",\n";
    return json;
}

string JsonParser::add_middle_json_element(string name, string value) {
    string json = add_json_element_content(name, value) +  ",\n";
    return json;
}

string JsonParser::add_last_json_element(string name, string value) {
    string json = add_json_element_content(name, value) + "}\n";
    return json;
}

string JsonParser::add_single_json_element_content(string name, string value) {
    string json = "{" + add_json_element_content(name, value) + "}\n";
    return json;
}

string JsonParser::add_json_element_content(string name, string value) {
    // if value is json (object or array)
    if(value.size() >= 2) {
        char first_letter_value = value.at(0);
        const char object_prefix = '{';
        const char array_prefix = '[';
        if(first_letter_value == object_prefix || first_letter_value == array_prefix) {
           return "\"" + name + "\": " + value; 
        }
    }

    return "\"" + name + "\": \"" + value + "\"";
}
