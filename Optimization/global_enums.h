#include <iostream>

using namespace std;

typedef enum RaceType{
    PROTOSS = 1,
    ZERG = 2,
    TERRAN = 3
} RaceType;

typedef enum list_status{
    UNTESTED = -33,
    INVALID = -404,
    MAX_TIME = INT32_MAX
} lstat;

int seed = 666;