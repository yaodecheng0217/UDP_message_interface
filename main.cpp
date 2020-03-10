/*
 * @Description: 主函数
 * @Author: Yaodecheng
 * @Date: 2019-10-09 09:08:07
 * @LastEditTime: 2020-03-10 17:49:14
 * @LastEditors: Yaodecheng
 **/
#include "ProtocolAnalysis.h"
#include "agv_ins.h"
#include "agv_msg.h"
#include "CJson/CJsonObject.hpp"
//正确收到数据后会调用此函数进行数据解包
void Callback_outdata(ReturnFrameData in)
{

     neb::CJsonObject oJson((char*)&in._databuff[0]);

    printf("RECV:[");
    //printf("%s", oJson.ToFormattedString().c_str());
    std::cout << oJson["test_float"].GetArraySize() << std::endl;
     float fTestValue = 0.0;
     for (int i = 0; i < oJson["test_float"].GetArraySize(); ++i)
     {
         oJson["test_float"].Get(i, fTestValue);
         std::cout << fTestValue << std::endl;
     }
   /* for (size_t i = 0; i < in._databuff.size(); i++)
    {
        printf("%c ", in._databuff[i]);
    }*/
    printf("]\n");
    return;
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
            PutEmptyStockNavData *p = (PutEmptyStockNavData *)&in._databuff[0];
            printf("PutEmptyStockNavData:\n");
            printf("from %d,%s\n", in.prot, in.ip);

            printf("id=%d\n", p->id);
            printf("x=%f\ny=%f\nz=%f\nyaw=%f\n\n", p->x, p->y, p->z, p->yaw);
            //printf("ret=%d\nposition_deviation=%f\n", p->ret, p->position_deviation);
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
        PutEmptyStockNavData x;
        x.id = 125;
        x.x = 1;
        x.y = 2;
        x.z = 3;
        x.yaw = 1.25;
        std::string aa="Test send and restore strings. <<---- Adeall AGV projce ---->> ";
        neb::CJsonObject oJson("{\"refresh_interval\":60,"
                        "\"test_float\":[18.01, 10.0, 5.06],"
                        "\"timeout\":12.5,"
                        "\"dynamic_loading\":["
                            "{"
                                "\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
                                "\"cmd\":["
                                     "{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
                                     "{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
                                "],"
                                "\"module\":["
                                     "{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
                                     "{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
                                "]"
                             "},"
                             "{"
                             "\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
                                 "\"cmd\":["
                                      "{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
                                 "],"
                             "\"module\":[]"
                             "}"
                        "]"
                    "}");
        Add_T_2_sendData(oJson.ToString(), &Xdata);
        //发送
        sendData("127.0.0.1", 9001, Xdata);
    };
};

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
