/*
 * @Descripttion: 
 * @version: 
 * @Author: Yaodecheng
 * @Date: 2019-10-19 10:45:26
 * @LastEditors  : Yaodecheng
 * @LastEditTime : 2020-02-07 12:59:51
 */

#include "thread_base.h"

#ifdef _WIN32
DWORD WINAPI thread_base::threadfun(void *ptr)
#else
void *thread_base::threadfun(void *ptr)
#endif
{
    thread_base *p = (thread_base *)ptr;
    p->_fun(p->_context);
    p->~thread_base();

    return 0;
}

thread_base::thread_base(RunFun f, void *context)
    : _fun(f), _context(context)
{

#ifdef _WIN32
    if (CreateThread(NULL, 0, threadfun, this, 0, NULL) == NULL)
#else
    if (pthread_create(&recvThread, NULL, threadfun, this))
#endif
    {
        printf("Error creating readThread.\n");
    }
}

thread_base::~thread_base()
{
}
