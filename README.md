<!--
 * @Author: Yaodecheng
 * @Date: 2019-10-26 13:39:25
 * @LastEditTime : 2019-12-26 13:48:39
 * @LastEditors  : Yaodecheng
 * @Description: ProtocolAnalysis
 * @FilePath: D:\Project\model\ProtocolAnalysis\README.md
 -->
# UDP-communication-interface-
Adeall 通用udp通讯协议接口

---

# 更新日志
* 2019-10-27
   1. 修改了UdpMessage接口为纯虚函数，由子类ProtocolAnalysis继承实现
   2. 增加了校验和函数
* 2019-12-25
   1. 增加指令集字段
   2. 增加发送方ip和端口号返回

---

# 初始化
```c++
ProtocolAnalysis miananglysis(Callback_outdata);//定义ProtocolAnalysis数据分析类，传入Callback_outdata自定义解包函数
miananglysis.init(9000);//解析函数初始化，传入端口号
```
# 数据打包方法
```c++
   //创建发送帧
    FrameDataStruct Xdata;
    Xdata.source_id = AGV;//填写发送方id
    Xdata.ins=CAMERA_INS;//填写所采用的指令集
    Xdata.cmd_id[0] = 0x00;//填写指令码1，暂时无用
    Xdata.cmd_id[1] = 0x01;//填写指令码2，暂时无用
    Xdata.cmd_type = ISSA_RES_PUT_EMPTY_STOCK_NAV;//填写数据类型，用于接收方解析

    //定义发送数据
    PutEmptyStockNavData x;
    //填充发送数据
    x.id=125;
    x.yaw=1.25;
    Add_T_2_sendData(x,&Xdata);//将结构体添加到发送帧
```
# 数据发送
```c++
miananglysis.sendData("127.0.0.1", 9000, Xdata);
```
# 数据解包方法
```c++
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
            PutEmptyStockNavData *p = (PutEmptyStockNavData *)&in._databuff[0];
            printf("PutEmptyStockNavData:\n");
            printf("from %d,%s\n", in.prot, in.ip);//获取发送方ip和端口号
            printf("id=%d\n", p->id);//获取发送方id
            printf("x=%f\ny=%f\nz=%f\nyaw=%f\n\n", p->x, p->y, p->z, p->yaw);//获取数据
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
```

# 校验和计算:
### 发送方：对要数据累加，得到一个数据和，对和求反，即得到我们的校验值。然后把要发的数据和这个校验值一起发送给接收方。
### 接收方：对接收的数据（包括校验和）进行累加，然后加1，如果得到0，那么说明数据没有出现传输错误。
    （注意，此处发送方和接收方用于保存累加结果的类型一定要一致uint8_t，否则加1就无法实现溢出从而无法得到0，校验就会无效）

# 注意
* 使用该接口时，传入的数据结构不应使用含有不定长度数据结构如vector，指针等;这样会使接受收方数据混乱和丢失。
