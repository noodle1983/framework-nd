#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <boost/thread.hpp>
#include <memory>

namespace DesignPattern
{
    template<typename DataType, int instanceId = 0>
    class Singleton 
    {
    public: 
        static DataType* instance()
        {
            if (NULL == dataHolderM.get())
            {
                boost::lock_guard<boost::mutex> lock(dbLockMutexM);
                if (NULL == dataHolderM.get())
                {
                    dataHolderM.reset(new DataType);
                }
            }
            return dataHolderM.get();

        }
    private:
        Singleton(){};
        ~Singleton(){};

        static std::auto_ptr<DataType> dataHolderM;
        static boost::mutex dbLockMutexM;
    };

    template<typename DataType, int instanceId>
    std::auto_ptr<DataType> Singleton<DataType, instanceId>::dataHolderM;

    template<typename DataType, int instanceId>
    boost::mutex Singleton<DataType, instanceId>::dbLockMutexM;
}

#endif /* SINGLETON_HPP */

