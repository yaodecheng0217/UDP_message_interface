
/*
 * @Description: In User Settings Edit
 * @Author: Yaodecheng
 * @Date: 2019-10-13 14:35:32
 * @LastEditTime : 2019-12-25 14:55:13
 * @LastEditors  : Yaodecheng
 */

#include "UdpMessage.h"
#define logi(_fmt,...) printf("[I] ");printf(_fmt "\n", ##__VA_ARGS__)
#define loge(_fmt,...) printf("[E] ");printf(_fmt "\n", ##__VA_ARGS__)
UdpMessage::UdpMessage()
{
}

UdpMessage::~UdpMessage()
{
}
//单独创建一帧数据处理线程
#ifdef _WIN32
DWORD WINAPI UdpMessage::Recv_thread(void *prt)
#else
void *UdpMessage::Recv_thread(void *prt)
#endif
{
    UdpMessage *p = (UdpMessage *)prt;
    p->recvcs.lock();//转移缓冲区数据前上锁
    std::vector<uint8_t> buffer;
    for (size_t i = 0; i < p->recvL; i++)
    {
        buffer.push_back(p->recv_buf[i]);
    }
    p->recvcs.unlock();
    p->CallBackFuntion(buffer, p->_context);
    return 0;
}
//查询接收缓冲区线程
void UdpMessage::recvudpfun()
{
#ifdef _WIN32
    if (CreateThread(NULL, 0, Recv_thread, this, 0, NULL) == NULL)
#else
    pthread_t readThread;
    if (pthread_create(&readThread, NULL, Recv_thread, this))
#endif
    {
#ifdef Printf_Error_EN
        loge("Error creating readThread.\n");
#endif
    }
}

#ifdef _WIN32
DWORD WINAPI UdpMessage::UDPrevthreadfun(void *ptr)
#else
void *UdpMessage::UDPrevthreadfun(void *ptr)
#endif
{
    UdpMessage *P = (UdpMessage *)ptr;
    //struct sockaddr_in addr_client;
    int len = sizeof(P->addr);
    while (1)
    {
#ifdef _WIN32
        P->recvL = recvfrom(P->sock_fd, P->recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&P->addr_client, &len);
#else
        P->recvL = recvfrom(P->sock_fd, P->recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&P->addr_client, (socklen_t *)&len);
#endif
        if (P->recvL <= 0)
        {
            continue;
        }
        else
        {
            //printf(">>>>%d,%s\n",ntohs(P->addr_client.sin_port),inet_ntoa(P->addr_client.sin_addr));
            P->recvudpfun();
        }
    }
    return 0;
}

int UdpMessage::init(int cliceport, void *prt)
{
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;

    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 1 ||
        HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();
        return -1;
    }
#endif

    port = cliceport;
    _context = prt;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        loge("udp %d socket error!\n", cliceport);
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
#ifdef _WIN32
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
#else
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

    int len = sizeof(addr);
    if (bind(sock_fd, (sockaddr *)&addr, len) == -1)
    {
        loge("udp %d bink error!\n", cliceport);
#ifdef _WIN32
        closesocket(sock_fd);
#else
        close(sock_fd);
#endif
        return -1;
    }
    else
    {
        logi("udp %d bink ok!\n", cliceport);
#ifdef _WIN32
        if (CreateThread(NULL, 0, UDPrevthreadfun, this, 0, NULL) == NULL)
#else
        if (pthread_create(&recvThread, NULL, UDPrevthreadfun, this))
#endif
        {
            loge("Error creating readThread.\n");
            return -1;
        }
    }

    return 0;
}

void UdpMessage::messagsend(const char *ip, int port, const char *data, int L)
{
    ScopeLocker lock(&sendcs);
    struct sockaddr_in addr_server;
    memset(&addr_server, 0, sizeof(addr_server));
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(port);
#ifdef _WIN32
    addr_server.sin_addr.S_un.S_addr = inet_addr(ip);
#else
    addr_server.sin_addr.s_addr = inet_addr(ip);
#endif

    int len = sizeof(addr_server);
    if (sendto(sock_fd, data, L, 0, (struct sockaddr *)&addr_server, len) < 0)
    {
        logi("udp ip=%s port=%d send error!%d\n", ip, port, sock_fd);
    }
    else
    {
        //logi("udp ip=%s port=%d send ok ! %d\n", ip, port,sock_fd);
    }
}
void UdpMessage::messagsend(const char *ip, int port, const std::string data)
{
    messagsend(ip, port, data.c_str(), data.length());
}
