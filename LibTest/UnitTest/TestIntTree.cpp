#include "IntTree.h"
#include "UserAgentParser.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace Utility;
using namespace std;

int main()
{
	IntTree<PhoneInfo> testTree;
	PhoneInfo testReturnPhoneInfo;
	PhoneInfo* testPhoneInfo1 = new PhoneInfo;
	testTree.build( "102", 3, testPhoneInfo1);

	PhoneInfo* testPhoneInfo2 = new PhoneInfo;
	testTree.build( "1024", 4, testPhoneInfo2 );

	assert(testTree.find( "102", 3, testReturnPhoneInfo ));

	assert(testTree.find( "1025", 4, testReturnPhoneInfo ));

	assert(testTree.find( "1024", 4, testReturnPhoneInfo ));

	return 0;
}
