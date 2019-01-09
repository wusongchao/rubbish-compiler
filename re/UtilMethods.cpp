#include "UtilMethods.h"

string toUpper(const string & str)
{
    string res(str);
    std::transform(str.begin(), str.end(), res.begin(), ::toupper);
    return res;
}

string toUpper(string && str)
{
    string res(std::move(str));
    std::transform(str.begin(), str.end(), res.begin(), ::toupper);
    return res;
}
