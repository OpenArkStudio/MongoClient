// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _MY_SINGLETON_H_
#define _MY_SINGLETON_H_

#include <mutex>

template <class T>
class MySingleton
{
public:
    MySingleton()
    {
    }

    static T* Instance()
    {
        if(m_pInstance != NULL)
        {
            return m_pInstance;
        }

        m_xMutex.lock();
        if(m_pInstance != NULL)
        {
            m_xMutex.unlock();
            return m_pInstance;
        }
        m_pInstance = new T();
        m_xMutex.unlock();
        return m_pInstance;
    }
    virtual ~MySingleton()
    {
        delete m_pInstance;
    }
private:
    static T* m_pInstance;
    static std::mutex m_xMutex;
};

template <class T>
T* MySingleton<T>::m_pInstance = NULL;

template <class T>
std::mutex MySingleton<T>::m_xMutex;

#endif // _MY_SINGLETON_H_
