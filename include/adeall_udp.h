/*
 * @Author: Yaodecheng
 * @Date: 2020-02-05 15:41:12
 * @LastEditors  : Yaodecheng
 */
#ifndef ADEALL_UDP_H
#define ADEALL_UDP_H
#include "ProtocolAnalysis.h"
#include <vector>
#include <string>

typedef void (*action_Callback)(std::vector<uint8_t> data, char *ip, int prot);

struct action
{
    std::string Topic;
    action_Callback action_Callback;
};
struct Topicdata
{
    std::string topic;
    std::vector<uint8_t> data;
};

class adeall_udp : protected ProtocolAnalysis
{
private:
    /* data */

public:
    adeall_udp();
    ~adeall_udp();
    /*初始化监听端口*/
    int init(int prot);
    /*添加一个topic，并且绑定该主题到action*/
    int add_topic(std::string, action_Callback);
    /*向某个topic发送数据*/
    template <typename T>
    int send_topic_data(std::string topic, int prot, T data)
    {
        //确定数据结构和填充发送数据
        FrameDataStruct Xdata;
        //填写采用的指令集
        Xdata.ins = 3;
        Topicdata x;
        x.topic = topic;
        x.data.resize(sizeof(data));
        memcpy(&x.data[0], &data, sizeof(data));
        for (size_t i = 0; i < x.data.size(); i++)
        {
             printf("%d_", x.data[i]);
            /* code */
        }
         printf("\n");
        
        Add_T_2_sendData(x, &Xdata);
        //发送,测试广播发送
        sendData("127.0.0.1", prot, Xdata);
        sendData("192.168.3.31", prot, Xdata);
        return 1;
    }
};
#endif