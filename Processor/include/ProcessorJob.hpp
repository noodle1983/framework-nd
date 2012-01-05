#ifndef PROCESSORJOB_HPP
#define PROCESSORJOB_HPP

#include "ThreadSafeAllocator.hpp"
#include "Singleton.hpp"
#include "Log.h"

#include <boost/shared_ptr.hpp>

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

	template<typename Type>
	void finiObject(boost::shared_ptr<Type>& theObj)
	{
		theObj.reset();
	}

	template<typename Type>
	inline void finiObject(Type theObj)
	{}


	class NullParamJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<NullParamJob, 100000> Allocator;
        typedef Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef NullParamJob CacheData;
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
			allocatorM->freeData(this);
		}

    private:
        Func funcM;

	public:
        union
        {
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
        };
	};

	template<typename Param>
	class OneParamJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<OneParamJob, 100000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
		typedef OneParamJob<Param> CacheData;
		typedef void (*Func)(Param theParam);
		OneParamJob(Func theFunc, Param theParam)
			: funcM(theFunc)
			, paramM(theParam)
		{}
		~OneParamJob(){}

		OneParamJob()
			:funcM(NULL)
		{}
		void init(Func theFunc, Param theParam)
		{
			funcM = theFunc;
			paramM = theParam;
		}


		virtual void operator()()
		{
			funcM(paramM);
		}

		virtual void returnToPool()
		{
			finiObject(paramM);
			allocatorM->freeData(this);
		}

	private:
        Func funcM;
		Param paramM;

	public:
        union
        {
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
        };
	};


	template<typename ClassType>
	class NullParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<NullParamClassJob, 100000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
        typedef NullParamClassJob<ClassType> CacheData;
		typedef void (ClassType::*Func)();

		NullParamClassJob(Func theFunc, ClassType*const theObj)
			: funcM(theFunc)
			, objM(theObj)
		{}
		~NullParamClassJob(){}

		NullParamClassJob()
            : funcM(NULL)
            , objM(NULL)
        {}
        void init(Func theFunc, ClassType*const theObj)
		{
			funcM = theFunc;
			objM = theObj;
        }

		virtual void operator()()
		{
			(objM->*funcM)();
		}

		virtual void returnToPool()
		{
			allocatorM->freeData(this);
		}
	private:
		Func funcM;
        ClassType* objM;
	public:
        union
        {
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
        };
	};

	template<typename ClassType>
	class NullParamClassEJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<NullParamClassEJob, 100000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
        typedef NullParamClassEJob<ClassType> CacheData; 
        typedef boost::shared_ptr<ClassType> ClassPtr;
		typedef void (ClassType::*Func)();

		NullParamClassEJob(Func theFunc, ClassPtr theObj)
			: funcM(theFunc)
			, objM(theObj)
		{}
		~NullParamClassEJob(){}

		NullParamClassEJob()
            : funcM(NULL)
        {}
        void init(Func theFunc, ClassPtr theObj)
		{
			funcM = theFunc;
			objM = theObj;
        }

		virtual void operator()()
		{
			(objM.get()->*funcM)();
		}

		virtual void returnToPool()
		{
            objM.reset();
			allocatorM->freeData(this);
		}
	private:
        Func funcM;
        ClassPtr objM;
	public:
        union
        {
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
        };
	};


	template<typename ClassType, typename ParamType1>
	class OneParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<OneParamClassJob, 100000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
        typedef OneParamClassJob<ClassType, ParamType1> CacheData;
		typedef void (ClassType::*Func)(ParamType1);

		OneParamClassJob(Func theFunc, ClassType*const theObj, ParamType1 theParam1)
			: funcM(theFunc)
			, objM(theObj)
			, param1M(theParam1)
		{}
		~OneParamClassJob(){}
		OneParamClassJob()
			: funcM(NULL)
			, objM(NULL)
        {}
        void init(Func theFunc, ClassType*const theObj, ParamType1 theParam1)
        {
			funcM = theFunc;
			objM = theObj;
			param1M = theParam1;
        }
		

		virtual void operator()()
		{
			(objM->*funcM)(param1M);
		}

		virtual void returnToPool()
		{
			finiObject(param1M);
			allocatorM->freeData(this);
		}
    private:
		Func funcM;
        ClassType* objM;
		ParamType1 param1M;
	public:
		union
		{
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
		};
	};

	template<typename ClassType, typename ParamType1, typename ParamType2>
	class TwoParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<TwoParamClassJob, 100000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
        typedef TwoParamClassJob<ClassType, ParamType1, ParamType2> CacheData;
		typedef void (ClassType::*Func)(ParamType1, ParamType2);

		TwoParamClassJob(
				Func theFunc, 
				ClassType*const theObj, 
				ParamType1 theParam1,
				ParamType2 theParam2)
			: funcM(theFunc)
			, objM(theObj)
			, param1M(theParam1)
			, param2M(theParam2)
		{}
		~TwoParamClassJob(){}
		TwoParamClassJob()
			: funcM(NULL)
			, objM(NULL)
		{}
		void init(
				Func theFunc, 
				ClassType*const theObj, 
				ParamType1 theParam1,
				ParamType2 theParam2)
        {
			funcM = theFunc;
			objM = theObj;
			param1M = theParam1;
			param2M = theParam2;
		}

		virtual void operator()()
		{
			(objM->*funcM)(param1M, param2M);
		}

		virtual void returnToPool()
		{
			finiObject(param1M);
			finiObject(param2M);
			allocatorM->freeData(this);
		}
	private:
		Func funcM;
        ClassType* objM;
		ParamType1 param1M;
		ParamType2 param2M;
	public:
		union
		{
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
		};
	};

	template<typename ClassType, 
        typename ParamType1, 
        typename ParamType2,
        typename ParamType3>
	class ThreeParamClassJob: public IJob
	{
	public:
		typedef Data::ThreadSafeAllocator<ThreeParamClassJob, 10000> Allocator;
        typedef typename Allocator::AllocatorType AllocatorType;
		typedef DesignPattern::Singleton<Allocator> AllocatorSingleton;
        typedef ThreeParamClassJob<ClassType, ParamType1, ParamType2, ParamType3> CacheData;
		typedef void (ClassType::*Func)(ParamType1, ParamType2, ParamType3);

		ThreeParamClassJob(
				Func theFunc, 
				ClassType*const theObj, 
				ParamType1 theParam1,
				ParamType2 theParam2,
                ParamType3 theParam3)
			: funcM(theFunc)
			, objM(theObj)
			, param1M(theParam1)
			, param2M(theParam2)
			, param3M(theParam3)
		{}
		~ThreeParamClassJob(){}
		ThreeParamClassJob()
			: funcM(NULL)
			, objM(NULL)
		{}
		void init(
				Func theFunc, 
				ClassType*const theObj, 
				ParamType1 theParam1,
				ParamType2 theParam2,
                ParamType3 theParam3)
        {
			funcM = theFunc;
			objM = theObj;
			param1M = theParam1;
			param2M = theParam2;
			param3M = theParam3;
		}

		virtual void operator()()
		{
			(objM->*funcM)(param1M, param2M, param3M);
		}

		virtual void returnToPool()
		{
			finiObject(param1M);
			finiObject(param2M);
			finiObject(param3M);
			allocatorM->freeData(this);
		}
	private:
		Func funcM;
        ClassType* objM;
		ParamType1 param1M;
		ParamType2 param2M;
		ParamType3 param3M;
	public:
		union
		{
            AllocatorType* allocatorM; 
            CacheData* nextFreeM;
		};
	};
}
#endif /* PROCESSORJOB_HPP */

