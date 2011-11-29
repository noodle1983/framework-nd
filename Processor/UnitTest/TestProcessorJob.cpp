#include "ProcessorJob.hpp"

#include <iostream>
#include <string>

using namespace Processor;
using namespace std;

void sayHello()
{
	cout << "Hello" << endl;
}

void say(const char* sth)
{
	cout << sth << endl;
}

class A
{
public:
	void sayHello()
	{
		cout << "A say Hello" << endl;
	}

	void say(const char* sth)
	{
		cout << "A say:" << sth << endl;
	}

	void talk(const char* sth, const char* andSth)
	{
		cout << "A talk " << sth << " and " << andSth << endl;
	}
};

int main()
{

	{
		NullParamJob* job = NullParamJob::AllocatorSingleton::instance()->newData();
		cout << "pool used " << NullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(sayHello);
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << NullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	{
		OneParamJob<const char*> job(say, "say sth");
		job();
	}
	{
		A a;
		NullParamClassJob<A> job(&A::sayHello, &a);
		job();
	}
	{
		A a;
		OneParamClassJob<A, const char*> job(&A::say, &a, "something");
		job();
	}
	{
		A a;
		TwoParamClassJob<A, const char*, const char*> job(&A::talk, &a, "something", " and something");
		job();
	}
	return 0;
}


