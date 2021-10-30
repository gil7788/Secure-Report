//
// Created by gil on 9.1.2021.
//

#ifndef SECURE_DATA_H
#define SECURE_DATA_H

#include <iostream>

using namespace std;

class Data {
public:
    virtual ~Data() {};

    virtual string to_json() = 0;

protected:
    string add_first_json_element(string name, string value);

    string add_middle_json_element(string name, string value);

    string add_last_json_element(string name, string value);

private:

    string add_json_element_content(string name, string value);
};



#endif //SECURE_DATA_H
