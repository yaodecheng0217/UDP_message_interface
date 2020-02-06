/*
 * @Author: Yaodecheng
 * @Date: 2020-02-05 21:58:28
 * @LastEditors  : Yaodecheng
 */
#include"adeall_udp.h"


std::vector<action> actionlist;

void Adeall_UDP_Callback(ReturnFrameData in)
{
    if(in.ins==3)
    {
        Topicdata *p = (Topicdata *)&in._databuff[0];
        for (size_t i = 0; i < actionlist.size(); i++)
        {   
            if(p->topic==actionlist[i].Topic)
            {
                std::vector<uint8_t> data;
                data.resize(p->data.size());
                memcpy(&data[0], &in._databuff[sizeof(Topicdata)], p->data.size());
                actionlist[i].action_Callback(data,in.ip,in.prot);
                return;
            }
        }
    }
}


adeall_udp::adeall_udp()
:ProtocolAnalysis(Adeall_UDP_Callback)
{

}
adeall_udp::~adeall_udp()
{

}
int adeall_udp::init(int prot)
{
ProtocolAnalysis::init(prot);
return 1;
}
int adeall_udp::add_topic(std::string topic, action_Callback action_callback)
{
   action xx;
   xx.action_Callback=action_callback;
   xx.Topic=topic;
   actionlist.push_back(xx);
   return 1;
}