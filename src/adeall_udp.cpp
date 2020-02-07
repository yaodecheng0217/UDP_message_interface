/*
 * @Author: Yaodecheng
 * @Date: 2020-02-05 21:58:28
 * @LastEditors  : Yaodecheng
 */
#include"adeall_udp.h"
#include"thread_base.h"

std::vector<action> actionlist;

struct Tempdata
{
   char * ip;
   int port;
   action ac;
   std::vector<uint8_t> data;
};
//DWORD WINAPI threadfun(void *ptr)
void *fffun(void *ptr)
{
  Tempdata f=*(Tempdata *)ptr;
  f.ac.action_Callback(f.data,f.ip,f.port,f.ac.ctx);
  return 0;
}

void Adeall_UDP_Callback(ReturnFrameData in)
{
    
    if(in.ins==123)
    {       
        Topicdata *p = (Topicdata *)&in._databuff[0];
        for (size_t i = 0; i < in._databuff.size(); i++)
        {
            printf("%c_",in._databuff[i]);
        }
        printf("\n");
        
        for (size_t i = 0; i < actionlist.size(); i++)
        {  printf(">>%s\n",actionlist[i].Topic.c_str());
            if(p->topic==actionlist[i].Topic)
            {
                printf(">>%s\n",p->topic.c_str());
                std::vector<uint8_t> data;
                data.resize(p->data.size());
                memcpy(&data[0], &in._databuff[sizeof(Topicdata)], p->data.size());
                actionlist[i].action_Callback(data,in.ip,in.prot,actionlist[i].ctx);
                /*Tempdata tempdata;
                tempdata.ip=in.ip;
                tempdata.ac=actionlist[i];
                tempdata.port=in.prot;
                tempdata.data.resize(p->data.size());
                memcpy(&tempdata.data[0], &in._databuff[sizeof(Topicdata)], p->data.size());
                thread_base thre(fffun,&tempdata);  */
                //CreateThread(NULL, 0, threadfun, &tempdata, 0, NULL);
                //tempdata.ac.action_Callback(tempdata.data,tempdata.ip,tempdata.port,tempdata.ac.ctx);
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
int adeall_udp::add_topic(std::string topic, action_Callback action_callback,void *ctx)
{
   action xx;
   xx.action_Callback=action_callback;
   xx.Topic=topic;
   xx.ctx=ctx;
   actionlist.push_back(xx);
   return 1;
}