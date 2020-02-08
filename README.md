<!--
 * @Author: Yaodecheng
 * @Date: 2019-10-26 13:39:25
 * @LastEditTime : 2020-02-08 18:08:23
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
# 《通讯框架使用说明书》
---
# 框架概述
&emsp;&emsp;采用自封装UDP形式进行通信，端对端通信方式，由统一的通讯接口保证通讯数据收发，各个子系统向其他系统发送数据时需要朝该子系统的所对应ip地址和端口号进行发送消息包，接收反馈数据由通讯接口回调函数处理。各子系统间主要通过UDP端口来进行标识，位于不同主机的子系统需要通过ip地址进行区分。
## 使用概述 
&emsp;&emsp;通讯框架主要提供结构化数据的打包和发送，发送时需要事先定义结构化数据的数据结构，然后将利用api进行数据大打包和发送
&emsp;&emsp;通讯过程主要为：
>1. 进行初始化，所有节点首先利用API进行端口绑定，并确定回调函数入口
>2. 将待发送的数据利用打包API打包到发送帧
>3. 调用API进行发送
>4. 接收方利用回调函数进行解析活动（注意：每次回调函数为独立线程，当消息不断到来，而处理较慢时应考虑做线程安全处理）
---
# 详细使用方法如下：
### 初始化
&emsp;&emsp;首先是接口初始化，主要进行回调函数的绑定和端口的绑定，绑定后所有消息将通过该端口发出，并且监听该端口
&emsp;&emsp;回调函数参数形式为
>void OutputDataFun(FrameDataStruct data);

&emsp;&emsp;代码示例如下：
```c++
ProtocolAnalysis MianAnglysis(Callback_outdata);//定义ProtocolAnalysis数据分析类，传入Callback_outdata自定义解包函数
                                                //typedef void (*OutputDataFun)(FrameDataStruct data);
MianAnglysis.init(9000);//解析函数初始化，传入端口号
```
### 数据打包方法
&emsp;&emsp;数据的打包利用指定的发送帧FrameDataStruct数据结构进行打包，发送帧中有其固定的数据数据结构，而待发送的数据需要利用函数Add_T_2_sendData进行封装到发送帧中去。注意数据的打包支持任意可确定数据长度大小的数据类型以及结构体，但可变长度的数据及结构体无法进行打包发送，需要进行分包发送
打包接口为
>void Add_T_2_sendData(T in, FrameDataStruct *out)

发送帧主要包含以下几方面信息：
>1. source_id,发送方id，用于识别当前数据包来自哪个子系统；
>2. cmd_id,暂时无用，留空；
>3. ins,数据集,主要用于区分不同情况下数据类型重复的问题；
>4. cmd_type,命令类型，用于数据结构重构，通讯双方事先约定好对应的数据结构。

发送帧数据结构为
>struct FrameDataStruct
{
    uint8_t source_id = 0;
    uint8_t cmd_id[2] = {0, 0};
    int8_t ins=-1;
    uint8_t cmd_type = 0;
    std::vector<uint8_t> _databuff;
};

使用例子如下:
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
### 数据发送
&emsp;&emsp;数据包打包好后直接调用发送函数即可发送
发送函数为：
>ProtocolAnalysis::sendData(const char *ip, int prot, FrameDataStruct sdata);

使用例子为：
```c++
miananglysis.sendData("127.0.0.1", 9000, Xdata);
```
### 数据解包方法
接收处理回调函数需要定义成
>void (*OutputDataFun)(ReturnFrameData data);

其中返回帧数据结构ReturnFrameData包含发送方ip和端口号信息，具体数据结构如下：
>struct ReturnFrameData
{
    char * ip;
    int prot;
    uint8_t source_id = 0;
    uint8_t cmd_id[2] = {0, 0};
    int8_t ins=-1;
    uint8_t cmd_type = 0;
    std::vector<uint8_t> _databuff;
};

收到数据的一般处理过程为：
1. 根据ins进行分类处理
2. 根据数据类型进行数据结构重构
3. 进行数据处理

使用例子如下
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
