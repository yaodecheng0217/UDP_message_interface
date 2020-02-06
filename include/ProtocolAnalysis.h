/*
 * @Descripttion: Adeall udp通讯协议解析
 * @version: V1.0
 * @Author: Yaodecheng
 * @Date: 2019-10-19 10:18:47
 * @LastEditors  : Yaodecheng
 * @LastEditTime : 2020-02-06 23:00:28
 */
#ifndef PROTOCOLANALYSIS_H
#define PROTOCOLANALYSIS_H
#include "UdpMessage.h"
struct FrameDataStruct
{
    uint8_t source_id = 0;
    uint8_t cmd_id[2] = {0, 0};
    int8_t ins=-1;
    uint8_t cmd_type = 0;
    std::vector<uint8_t> _databuff;
};
struct ReturnFrameData
{
    char * ip;
    int prot;
    uint8_t source_id = 0;
    uint8_t cmd_id[2] = {0, 0};
    int8_t ins=-1;
    uint8_t cmd_type = 0;
    std::vector<uint8_t> _databuff;
};
template <typename T>
void Add_T_2_sendData(T in, FrameDataStruct *out)
{
    out->_databuff.resize(sizeof(in));
    memcpy(&out->_databuff[0], &in, sizeof(in));
}

class ProtocolAnalysis : protected UdpMessage
{
    typedef void (*OutputDataFun)(ReturnFrameData data);
private:
    OutputDataFun _outputfun;
    void CallBackFuntion(std::vector<uint8_t>,void *);

public:
    ProtocolAnalysis(OutputDataFun);
    ~ProtocolAnalysis();
    int init(const int port);
    void sendData(const char *ip, int prot, FrameDataStruct sdata);
};
#endif
