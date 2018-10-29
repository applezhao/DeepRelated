#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>
#include <numeric> 
#include <set>
#include <string> 
#include <queue>
#include <thread>
#include <future>
#include <atomic>

#include "LoadDeepExr.h"


using namespace std;



#ifdef NDEBUG
static void init_log(void)
{
	/* init boost log
	* 1. Add common attributes
	* 2. set log filter to trace
	*/
	boost::log::add_common_attributes();
}
#endif



int main()
{

#ifdef NDEBUG
	init_log();
#endif
	
	loadExrAsPointCloud loader("C:\\Users\\Apple\\Documents\\GitHub\\DeepRelated\\masterLayer.0003.exr");
	loader.loadImage();
}