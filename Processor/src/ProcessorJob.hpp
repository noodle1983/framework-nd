#ifndef PROCESSORJOB_HPP
#define PROCESSORJOB_HPP

#include "ThreadSafeAllocator.hpp"
#include "Singleton.hpp"

namespace Processor 
{
	class IJob
	{
	public:
		IJob(){};
		virtual ~IJob(){};

		virtual void operator()() = 0;
		virtual void returnToPool() = 0;
	};

	class NullParamJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<NullParamJob, 100000> Allocator;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef void (*Func)();

		NullParamJob(Func theFunc)
			:funcM(theFunc)
		{}
		~NullParamJob(){}

		NullParamJob()
			:funcM(NULL)
		{}
		void init(Func theFunc)
		{
			funcM = theFunc;
		}


		virtual void operator()()
		{
			funcM();
		}

		virtual void returnToPool()
		{
			AllocatorSingleton::instance()->freeData(this);
		}

	public:
		union
		{
			Func funcM;
			NullParamJob* nextFreeM;
		};
	};

	template<typename Param>
	class OneParamJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<OneParamJob, 100000> Allocator;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef void (*Func)(Param theParam);
		OneParamJob(Func theFunc, Param theParam)
			: funcM(theFunc)
			, paramM(theParam)
		{}
		~OneParamJob(){}


		virtual void operator()()
		{
			funcM(paramM);
		}

		virtual void returnToPool()
		{
			AllocatorSingleton::instance()->freeData(this);
		}
	public:
		union
		{
			Func funcM;
			OneParamJob* nextFreeM;
		};
		Param paramM;
	};


	template<typename ClassType>
	class NullParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<NullParamClassJob, 100000> Allocator;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef void (ClassType::*Func)();

		NullParamClassJob(Func theFunc, ClassType* theObj)
			: funcM(theFunc)
			, objM(theObj)
		{}
		~NullParamClassJob(){}

		virtual void operator()()
		{
			(objM->*funcM)();
		}

		virtual void returnToPool()
		{
			AllocatorSingleton::instance()->freeData(this);
		}
	public:
		Func funcM;
		union
		{
			ClassType* objM;
			NullParamClassJob* nextFreeM;
		};
	};

	template<typename ClassType, typename ParamType1>
	class OneParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<OneParamClassJob, 100000> Allocator;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef void (ClassType::*Func)(ParamType1);

		OneParamClassJob(Func theFunc, ClassType* theObj, ParamType1 theParam1)
			: funcM(theFunc)
			, objM(theObj)
			, param1M(theParam1)
		{}
		~OneParamClassJob(){}

		virtual void operator()()
		{
			(objM->*funcM)(param1M);
		}

		virtual void returnToPool()
		{
			AllocatorSingleton::instance()->freeData(this);
		}
	public:
		Func funcM;
		union
		{
			ClassType* objM;
			OneParamClassJob* nextFreeM;
		};
		ParamType1 param1M;
	};

	template<typename ClassType, typename ParamType1, typename ParamType2>
	class TwoParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<TwoParamClassJob, 100000> Allocator;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef void (ClassType::*Func)(ParamType1, ParamType2);

		TwoParamClassJob(
				Func theFunc, 
				ClassType* theObj, 
				ParamType1 theParam1,
				ParamType2 theParam2)
			: funcM(theFunc)
			, objM(theObj)
			, param1M(theParam1)
			, param2M(theParam2)
		{}
		~TwoParamClassJob(){}

		virtual void operator()()
		{
			(objM->*funcM)(param1M, param2M);
		}

		virtual void returnToPool()
		{
			AllocatorSingleton::instance()->freeData(this);
		}
	public:
		Func funcM;
		union
		{
			ClassType* objM;
			TwoParamClassJob* nextFreeM;
		};
		ParamType1 param1M;
		ParamType2 param2M;
	};
}
#endif /* PROCESSORJOB_HPP */

