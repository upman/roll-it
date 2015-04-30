#ifndef LIB_H_
#define LIB_H_
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include <stdarg.h>
using namespace std;
using namespace libconfig;
int readFiles();
float loadConfig(string,string,string);
#endif
