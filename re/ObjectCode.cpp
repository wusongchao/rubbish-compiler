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
		<< offset << endl;
    return outputStream.str();
}

string ObjectCode::Lod::toString()
{
	ostringstream outputStream;
	outputStream << "RED" << ' '
		<< depth
		<< " , "
		<< offset<< endl;
	return outputStream.str();
}

string ObjectCode::Lit::toString()
{
	ostringstream outputStream;
	outputStream << "LIT" << ' '
		<< '0'
		<< " , "
		<< constant<<endl;
	return outputStream.str();
}

string ObjectCode::Store::toString()
{
	ostringstream outputStream;
	outputStream << "STO" << ' '
		<< depth
		<< " , "
		<< offset <<endl;
	return outputStream.str();
}

string ObjectCode::BinaryOp::toString()
{
	ostringstream outputStream;
	outputStream << "RED" << ' '
		<< '0'
		<< " , "
		<< opStr<<endl;
	return outputStream.str();
}

string ObjectCode::Write::toString()
{
	return string("WRT 0 , 0\n");
}

string ObjectCode::Int::toString()
{
	return string("INT 0,") + to_string(value) + '\n';
}

string ObjectCode::Jmp::toString()
{
	ostringstream outputStream;
	outputStream<<op<<' '
		<< '0'
		<< " , "
		<< target
		<< endl;
	return outputStream.str();
}

string ObjectCode::Call::toString()
{
	
	return string("CAL 0,")+func+'\n';
}
