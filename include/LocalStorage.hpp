#pragma once

extern "C"
{
#include <pthread.h> // pthread library for accessing thread local storage
}

#include <mutex>

template <typename type> class LocalStorage
{
    // Static hook
public:
    static void cleanup(void * ptr)
    {
        if (ptr) delete static_cast<type *>(ptr);
    }

    // Data
private:
    bool _once{true};
    pthread_key_t _key{0};
    std::mutex _keylock;

    // Constructors & Co.
public:
    LocalStorage() {}
    ~LocalStorage() = default;

    // Operators (Work as proxy)
public:
    type * operator->() { return getSpecific(); }

    type & operator*() { return *(getSpecific()); }

    // Methods
private:
    type * getSpecific()
    {
        type * tss_data{nullptr};

        if (_once)
        {
            std::lock_guard<std::mutex> guard(_keylock);
            if (_once)
            {
                pthread_key_create(&_key, &cleanup);
                _once = false;
            }
        }

        tss_data = reinterpret_cast<type *>(pthread_getspecific(_key));

        if (!tss_data)
        {
            tss_data = new type; // Loss of functionality (WRAPPER FACADE/FACTORY)
            pthread_setspecific(_key, reinterpret_cast<void *>(tss_data));
        }

        return tss_data;
    }
};
