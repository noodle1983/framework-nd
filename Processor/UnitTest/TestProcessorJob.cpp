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
		NullParamJob* job = NullParamJob::AllocatorSingleton::instance()->newData(0);
		cout << "pool used " << NullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(sayHello);
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << NullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	{
		//OneParamJob<const char*> job(say, "say sth");
		//job();
        typedef OneParamJob<const char*> StrJob;
		StrJob* job = StrJob::AllocatorSingleton::instance()->newData(0);
		cout << "pool used " << StrJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(say, "say sth");
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << StrJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	{
		A a;
		//NullParamClassJob<A> job(&A::sayHello, &a);
		//job();
        typedef NullParamClassJob<A> ANullParamJob;
		ANullParamJob* job = ANullParamJob::AllocatorSingleton::instance()->newData(0);
		cout << "pool used " << ANullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(&A::sayHello, &a);
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << ANullParamJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	{
		A a;
		//OneParamClassJob<A, const char*> job(&A::say, &a, "something");
		//job();
        typedef OneParamClassJob<A, const char*> AOneParamJob;
		AOneParamJob* job = AOneParamJob::AllocatorSingleton::instance()->newData(0);
		cout << "pool used " << AOneParamJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(&A::say, &a, "something");
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << AOneParamJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	{
		A a;
		//TwoParamClassJob<A, const char*, const char*> job(&A::talk, &a, "something", " and something");
		//job();
        typedef TwoParamClassJob<A, const char*, const char*> ATwoParamJob;
		ATwoParamJob* job = ATwoParamJob::AllocatorSingleton::instance()->newData(0);
		cout << "pool used " << ATwoParamJob::AllocatorSingleton::instance()->getUsed() << endl;
		job->init(&A::talk, &a, "something", "sth");
		IJob* job2 = job;
		(*job2)();
		job2->returnToPool();
		cout << "pool used " << ATwoParamJob::AllocatorSingleton::instance()->getUsed() << endl;
	}
	return 0;
}


