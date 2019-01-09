#include "ObjectCode.h"
#include <sstream>

using std::ostringstream;

string ObjectCode::Read::toString()
{
    ostringstream outputStream;
    outputStream << "RED" << ' '
        << depth
        << " , "
        << offset;
    return outputStream.str();
}
