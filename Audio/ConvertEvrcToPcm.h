#pragma once
#include <vector>
class ConvertEvrcToPcm
{
public:
	ConvertEvrcToPcm();
	~ConvertEvrcToPcm();
	void convert_msg(const std::vector<unsigned char>& in);
private:

};

