/*
 * @Description: 主函数
 * @Author: Yaodecheng
 * @Date: 2019-10-09 09:08:07
 * @LastEditTime : 2020-02-07 13:45:42
 * @LastEditors  : Yaodecheng
 **/
#include "ProtocolAnalysis.h"
#include "agv_ins.h"
#include "agv_msg.h"
#include "adeall_udp.h"

struct TESTdata
{
    std::string A;
};



//正确收到数据后会调用此函数进行数据解包
void Callback_outdata(ReturnFrameData in)
{
    //根据指令集分类
    switch (in.ins)
    {
    case CAMERA_INS:
    {
        //根据类型进行分类
        switch (in.cmd_type)
        {
        case ISSA_RES_PUT_EMPTY_STOCK_NAV:
        {
            //todo
            //定义对应结构体指针
            /*PutEmptyStockNavData *p = (PutEmptyStockNavData *)&in._databuff[0];
            printf("PutEmptyStockNavData:\n");
            printf("from %d,%s\n", in.prot, in.ip);

            printf("id=%d\n", p->id);
            printf("x=%f\ny=%f\nz=%f\nyaw=%f\n\n", p->x, p->y, p->z, p->yaw);*/
            TESTdata *x=(TESTdata *)&in._databuff[0];
            printf("recv:%s\n",x->A.c_str());
            for (size_t i = 0; i < in._databuff.size(); i++)
            {
               printf("%d_",in._databuff[i]);
            }
            printf("\n");
            for (size_t i = 0; i < in._databuff.size(); i++)
            {
               printf("%c_",in._databuff[i]);
            }
            printf("\n");
            break;
        }
        default:
            printf("error !cmd_type=%d\n", in.cmd_type);
            break;
        }
    }
    break;
    default:
        break;
    }
}
class APP : private ProtocolAnalysis
{
public:
    APP() : ProtocolAnalysis(Callback_outdata){};
    void run()
    {
        init(9001);
    };
    void send()
    {
        //确定数据结构和填充发送数据
        FrameDataStruct Xdata;
        //填写采用的指令集
        Xdata.ins = CAMERA_INS;
        //填写源id
        Xdata.source_id = AGV;
        //填写数据类型
        Xdata.cmd_type = ISSA_RES_PUT_EMPTY_STOCK_NAV;
        //添加数据区到帧数据
        /*PutEmptyStockNavData x;
        x.id = 125;
        x.x = 1;
        x.y = 2;
        x.z = 3;
        x.yaw = 1.25;*/
        TESTdata x;
        x.A="123456789";
        Add_T_2_sendData(x, &Xdata);
         printf("send:%s\n",x.A.c_str());
        //发送
        for (size_t i = 0; i < Xdata._databuff.size(); i++)
            {
               printf("%d^",Xdata._databuff[i]);
            }
            printf("\n");
             for (size_t i = 0; i < Xdata._databuff.size(); i++)
            {
               printf("%c^",Xdata._databuff[i]);
            }
            printf("\n");
        sendData("127.0.0.1", 9001, Xdata);
    };
};

class APP2
{
private:
    //定义一个想要的结构体
    struct TEST1
    {
        int a;
        uint8_t b;
        int c;
    };
    adeall_udp Adeall_msg;
    void static action1(std::vector<uint8_t> data, char *Client_ip, int Client_prot, void *ctx)
    {
        //重构上下文信息
        adeall_udp *msg = (adeall_udp *)ctx;
        //判断数据结构体是否正确
        if(sizeof(TEST1)!=data.size())
        {
            //数据类型不匹配
            //返回错误信息
            msg->send_topic_data("ERROR", -1, Client_ip, Client_prot);
            return;
        }
        //重构数据结构体
        TEST1 *x1 = (TEST1 *)&data[0];
        printf("a=%d\n", x1->a);
        printf("b=%d\n", x1->b);
        printf("c=%d\n", x1->c);
        printf("ip=%s\n", Client_ip);
        printf("port=%d\n", Client_prot);
        Sleep(10);
        //反馈相应的数据给请求方
        //msg->send_topic_data("OK", 1, "127.0.0.1", 9002);
    }
    void static action_ok(std::vector<uint8_t> data, char *Client_ip, int Client_prot, void *ctx)
    {
        //重构上下文信息
        adeall_udp *msg = (adeall_udp *)ctx;
        //判断数据结构体是否正确
        if(sizeof(int)!=data.size())
        {
            return;
        }
        printf("OK=%d\n",*(int*)&data[0]);
    }
    void static action_error(std::vector<uint8_t> data, char *Client_ip, int Client_prot, void *ctx)
    {
        //重构上下文信息
        adeall_udp *msg = (adeall_udp *)ctx;
        //判断数据结构体是否正确
        if(sizeof(int)!=data.size())
        {
            return;
        }
        printf("ERROR=%d\n",*(int*)&data[0]);
    }
public:
    APP2();
    ~APP2();
    void run()
    {
        //初始化9002端口
        Adeall_msg.init(9003); 
        //添加一个APP2的topic以及绑定到action1
        Adeall_msg.add_topic("APP2", action1, &Adeall_msg);
        //添加一个ok响应的topic以及绑定到action1
        Adeall_msg.add_topic("OK", action_ok, &Adeall_msg);
        //添加一个error响应的topic以及绑定到action1
        Adeall_msg.add_topic("ERROR", action_error, &Adeall_msg);
    }
    void send()
    {
        //测试发送
        TEST1 a;
        a.a = 123;
        a.b = 56;
        a.c = 66;
        //发送信息
        //Adeall_msg.send_topic_data("APP2", a, "127.0.0.1", 9003);
    }
};

APP2::APP2()
{
}

APP2::~APP2()
{
}

int main()
{
    APP app;
    //开始运行
    app.run();
    
    while (1)
    {
        app.send();
#ifdef _WIN32
        Sleep(1000);
#else
        usleep(1000 * 1000);
#endif
    }
    return 0;
}
