/*
 * @Descripttion: Adeall udp通讯协议解析
 * @version: V1.0
 * @Author: Yaodecheng
 * @Date: 2019-10-19 10:18:47
 * @LastEditors: Yaodecheng
 * @LastEditTime: 2020-03-10 16:55:45
 */

#ifndef _PREOTOCOLANALYSIS_H_
#define  _PREOTOCOLANALYSIS_H_

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
    if (typeid(T) == typeid(std::string))
    {
        std::string data = (std::string)in;
        int size = data.length();
        out->_databuff.resize(size + 1);
        for (size_t i = 0; i < size; i++)
        {
            out->_databuff[i] = data[i];
        }
        out->_databuff[size] = '\0';
        return;
    }
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