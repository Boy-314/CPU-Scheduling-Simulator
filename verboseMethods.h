/*-----------------------------------------------------------------------------
 * Class: verboseMethods.h
 * Author: Ricardo N. Guntur <ricardo.guntur@nyu.edu>
 * Course: Operating Systems -- Professor Allan Gottlieb
 -----------------------------------------------------------------------------*/

#ifndef LAB_2_TESTS_VERBOSEMETHODS_H
#define LAB_2_TESTS_VERBOSEMETHODS_H
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include "schedulerMethods.h"

using namespace std;

//Debugging class
class verboseMethods {
    static bool is_verbose;

public:
    //verboseMethods();
    static void setVerbosity(bool debug);
    void log(string message);
    void log(const char *message, int var);
    void log(const char *message, string str);
    void print(int currentCycle, vector<Process> process_vector);

    bool getVerbosity();
};

#endif //LAB_2_TESTS_VERBOSEMETHODS_H
