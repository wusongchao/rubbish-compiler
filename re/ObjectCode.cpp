#include "ObjectCode.h"
#include <sstream>

using std::ostringstream;
using std::endl;
using std::to_string;

string ObjectCode::Read::toString()
{
    ostringstream outputStream;
	outputStream << "RED" << ' '
		<< depth
		<< " , "
		<< offset;
    return outputStream.str();
}

string ObjectCode::Load::toString()
{
	ostringstream outputStream;
	outputStream << "LOD" << ' '
		<< depth
		<< " , "
		<< offset;
	return outputStream.str();
}

string ObjectCode::Lit::toString()
{
    return "LIT 0 , " + constant;
}

string ObjectCode::Store::toString()
{
	ostringstream outputStream;
	outputStream << "STO" << ' '
		<< depth
		<< " , "
		<< offset;
	return outputStream.str();
}

string ObjectCode::BinaryOp::toString()
{
    return "OPR 0 , " + opStr;
}

string ObjectCode::Write::toString()
{
	return string("WRT 0 , 0");
}

string ObjectCode::Int::toString()
{
	return string("INT 0 , ") + to_string(value);
}

string ObjectCode::Jmp::toString()
{
	ostringstream outputStream;
    outputStream << op << ' '
        << '0'
        << " , "
        << target;
	return outputStream.str();
}

string ObjectCode::Call::toString()
{
	
	return "CAL 0 , " + func;
}
