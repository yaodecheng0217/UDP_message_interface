#include"adeall_udp.h"


std::vector<action> actionlist;

void Callback_outdata(ReturnFrameData in)
{
    
    if(in.ins==3)
    {
        Topicdata *p = (Topicdata *)in._databuff[0];
        for (size_t i = 0; i < actionlist.size(); i++)
        {
            if(p->topic==actionlist[i].Topic)
            {
                ;
            }
        }
    }
}


adeall_udp::adeall_udp()
:ProtocolAnalysis(Callback_outdata)
{

}
adeall_udp::~adeall_udp()
{

}
int adeall_udp::init(int prot)
{
ProtocolAnalysis::init(prot);
}
adeall_udp::add_topic()