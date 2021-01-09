//
// Created by gil on 9.1.2021.
//

#include "Data.h"

string Data::add_first_json_element(string name, string value) {
    string json = "{\n" + add_json_element_content(name, value) + ",\n";
    return json;
}

string Data::add_middle_json_element(string name, string value) {
    string json = add_json_element_content(name, value) +  ",\n";
    return json;
}

string Data::add_last_json_element(string name, string value) {
    string json = add_json_element_content(name, value) + "}\n";
    return json;
}

string Data::add_json_element_content(string name, string value) {
    return "\"" + name + "\": \"" + value + "\"";
}
