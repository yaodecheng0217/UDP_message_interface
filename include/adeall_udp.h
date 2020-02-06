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
struct TEST
{
    std::string topic;
    int a;
};
class adeall_udp : protected ProtocolAnalysis
{
private:
public:
    adeall_udp();
    ~adeall_udp();
    //初始化监听端口
    int init(int prot);
    //添加一个topic，并且绑定该主题到action
    int add_topic(std::string, action_Callback);
    //向某个topic发送数据
    template <typename T>
    int send_topic_data(const std::string topic, T data, const char *ip, int prot)
    {
        //确定数据结构和填充发送数据
        FrameDataStruct Xdata;
        //填写采用的指令集
        Xdata.ins = 3;
        Topicdata tipdata;
        tipdata.topic = topic;

        tipdata.data.resize(sizeof(data));
        memcpy(&tipdata.data[0], &data, sizeof(data));
        Xdata._databuff.resize(sizeof(tipdata)+sizeof(data));
        memcpy(&Xdata._databuff[0], &tipdata, sizeof(tipdata));
        memcpy(&Xdata._databuff[0+sizeof(tipdata)], &data, sizeof(data));
        Topicdata * temp=(Topicdata *)&Xdata._databuff[0];
        sendData(ip, prot, Xdata);
        //测试广播发送
        //sendData("192.168.3.31", prot, Xdata);
        return 1;
    }
};
#endif