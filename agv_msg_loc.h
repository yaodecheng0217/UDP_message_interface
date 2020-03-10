/*
 * @Author: Yaodecheng
 * @Date: 2019-12-24 14:12:37
 * @LastEditors  : Yaodecheng
 */
#ifndef AGV_MSG_LOC_H
#define AGV_MSG__LOC_H

#include <iostream>
//叉车AGV本地消息数据定义
//**************************************AGV Control Datt**************************************
typedef enum AGV_CONTROL_CMD_LIST
{
	CAR_STATE_DRIVE = 0x01,
	CAR_STATE_SAFE = 0x02,
	CAR_STATE_TASK = 0x03,
	CAR_STATE_LASSER = 0x04,

	CAR_OUTPUT_STATE = 0x05,

	DEBUG_IMU_DATA = 0x06,
	DEBUG_UWB_DATA = 0x07,

} CTRL_CMD;
//车辆驱动状态数据
typedef struct CarStateDriveData
{
	float speed; //行驶速度
	float yaw;   //车辆航向
	float posx;	 //坐标x
	float posy;	 //坐标y
};
//车辆传感器正常数据
typedef struct CarStateSafeData
{
	bool sick;	//激光避障
	bool acmera;  //相机
	bool uwb;	 //定位标签
	bool TL740D;  //转角仪
	bool high;	//高度传感器
	bool slope;   //倾斜传感器
	bool side;	//侧移传感器
	bool forward; //前移传感器
	bool tary;	//托盘传感器
};
//车辆任务执行状态数据
typedef struct CarStateTask
{
	uint8_t id;		 //指示当前正在执行wms的上位机执行序号
	uint8_t process; //指示当前任务已经进行到的步骤
	uint8_t stpe;	//执行当前任务需要总步骤数
	uint8_t result;  //任务执行结果0正在执行，1成功完成，-1失败结束
};
//车辆激光测距数据
typedef struct CarStateLasserData
{
	float high;	//高度传感器
	float slope;   //倾斜传感器
	float side;	//侧移传感器
	float forward; //前移传感器
	float tary;	//托盘传感器
	
};
//车辆控制状态数据
typedef struct CarOutControlData
{
	float rise;	//举升控制值
	float slope;   //倾斜控制值
	float side;	//侧移控制量
	float forward; //前移控制量
	float walk;	//行走输出
	float wheel;   //转向角输出
	float brake;   //刹车输出
};
//IMU数据
typedef struct DebugImuData
{
	double yaw;   //偏航
	double pitch; //俯仰
	double roll;  //横滚
	int rate;	 //解算速率

	int Accx;  //x加速度
	int Accy;  //y
	int Accz;  //z
	int Gyrox; //陀螺仪x
	int Gyroy; //y
	int Gyroz; //z
	int Mx;	//磁力计
	int My;	//y
	int Mz;	//z

	int height, temperature, prssure;
};
//uwb数据
typedef struct DebugLocalSenerData
{
	int Tagid, Posx, Posy, Posz; //标签id    坐标  x，y，z
	long long int TimeStam;		 //时间锉
};
//**************************************AGV Control Datt**************************************
#endif