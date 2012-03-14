#include "IntTree.h"
#include "UserAgentParser.h"

#include <iostream>
#include <string>
#include <assert.h>

using namespace Utility;
using namespace std;

int main()
{
	IntTree<PhoneInfo> testTree;
	PhoneInfo testReturnPhoneInfo;
	PhoneInfo* testPhoneInfo1 = new PhoneInfo;
	testPhoneInfo1->idM = 102;

	testTree.build( "102", 3, testPhoneInfo1);

	PhoneInfo* testPhoneInfo2 = new PhoneInfo;
	testPhoneInfo2->idM = 1024;
	testTree.build( "1024", 4, testPhoneInfo2 );


	testTree.find( "102", 3, testReturnPhoneInfo );
	assert( testReturnPhoneInfo.idM == 102 );

	testTree.find( "1025", 4, testReturnPhoneInfo );
	assert( testReturnPhoneInfo.idM == 102 );

	testTree.find( "1024", 4, testReturnPhoneInfo );
	assert( testReturnPhoneInfo.idM == 1024 );
	return 0;
