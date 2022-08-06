#define _POSIX_THREAD_KEY_MAX 20

struct thread_state
{
    int errno_;
    void * key_[_POSIX_THREAD_KEY_MAX];
    ...
};
// thread representation


using pthread_key_t = int;

static pthread_key_t total_keys_ = 0;
static void (*thread_exit_hook_[_POSIX_THREAD_KEY_MAX]) (void);

int pthread_key_create(pthread_key_t * key, void (*destructor) (void *))
{
    if (total_keys_ >= _POSIX_THREAD_KEY_MAX)
    {
        pthread_self()->errno_ = ENOMEM;
        return -1;
    }
    thread_exit_hook_[total_keys_] = destructor;
    *key = total_keys_++;
    return 0;
}


for (int i = 0; i < total_keys_; ++i)
{
    if (pthread_self()->key_[i] && thread_exit_hook_[i])
        *(thread_exit_hook_[i])(pthread_self()->key_[i]);
}



int pthread_setspecific(pthread_key_t key, void * value)
{
    if (key < 0 || key >= total_keys_) ... // error setting
    
    pthread_self()->key_[key] = value;
    return 0;
}

int pthread_getspecific(pthread_key_t key, void ** value)
{
    if (key < 0 || key >= total_keys_) ... // error setting

    *value = pthread_self()->key_[key];
    return 0;
}