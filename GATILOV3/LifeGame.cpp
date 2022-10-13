#include <iostream>
#include <vector>
#include "Life.h"



int main(int argc,char* argv[])
{

	ui ss(offline,argc,argv);
	while (true)
	{	
		ss.work();
	}
	return 0;
}