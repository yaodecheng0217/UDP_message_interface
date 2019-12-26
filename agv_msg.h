/*
 * @Descripttion:
 * @version:
 * @Author: Yaodecheng
 * @Date: 2019-10-09 16:20:04
 * @LastEditors  : Yaodecheng
 * @LastEditTime : 2019-12-25 12:06:07
 */

/*
  * 定义AGV中心和其他节点之间的通信数据结构
  */
#ifndef AGV_MSG_H
#define AGV_MSG_H

#include <iostream>
//-------------------------------AGV System Common-----------------------------------------------
//整个通信系统指令码列表
//AGV操作指令类型
//ISSA返回数据类型
typedef enum AGV_SYSTEM_CMD_LIST
{
	NULL_CMD = 0x00,						//空指令,通信测试
	AGV_REQ_ISSA_WORKING_STATUS = 0x01,		//AGV查询ISSA工作状态
	AGV_REQ_FINISH_CURRENT_OPERATE = 0x02,  //AGV请求结束当前操作
	AGV_REQ_GET_EXPORT_CARGO_NAV = 0x03,	//AGV请求取出货口托盘的导航数据
	AGV_REQ_GET_EMPTY_STOCK_NAV = 0x04,		//AGV请求取空托盘的导航数据
	AGV_REQ_PUT_EMPTY_STOCK_NAV = 0x05,		//AGV请求放空托盘的导航数据
	AGV_REQ_OBSTACLE_DETECTING = 0x06,		//AGV请求障碍物检测数据
	AGV_REQ_FORK_SEPARATE_DETECTING = 0x07, //AGV请求货叉分离检测数据

	ISSA_RES_WORKING_STATUS = 0x11,			//ISSA响应工作状态查询指令
	ISSA_RES_FINISH_CURRENT_OPERATE = 0x12, //ISSA响应结束当前操作指令
	ISSA_RES_GET_EXPORT_CARGO_NAV = 0x13,
	ISSA_RES_GET_EMPTY_STOCK_NAV = 0x14,
	ISSA_RES_PUT_EMPTY_STOCK_NAV = 0x15,
	ISSA_RES_OBSTACLE_DETECTING = 0x16,
	ISSA_RES_FORK_SEPARATE_DETECTING = 0x17,

} AGV_SYSTEM_CMD_LIST;
//消息来源节点ID
typedef enum MSG_SOURCE_ID_LIST
{
	AGV = 0xD1,  //AGV主控
	ISSA = 0xD2, //AGV图像处理中心
} MSG_SOURCE_ID_LIST;
//-------------------------------AGV System Common-----------------------------------------------

//-------------------------------ISSA Response Data Special----------------------------------
//ISSA上报工作状态数据格式
typedef struct ISSAWorkinStatusData
{
	uint8_t status; //存储工作状态,数值参照ISSA工作状态列表
} ISSAWorkinStatusData;
//ISSA报工作状态列表
typedef enum ISSA_WORKING_STATUS_LIST
{
	ISSA_STANBY = 0x00,					 //准备完毕空闲，随时可以开启新任务
	ISSA_GET_EXPORT_CARGO_NAV = 0x01,	//获取出货口货物导航
	ISSA_GET_EMPTY_STOCK_NAV = 0x02,	 //获取空托盘导航
	ISSA_PUT_EMPTY_STOCK_NAV = 0x03,	 //放置空托盘导航
	ISSA_OBSTACLE_DETECTING = 0x04,		 //障碍物检测
	ISSA_FORK_SEPARATE_DETECTING = 0x05, //货叉分离检测

	ISSA_FINISH_CURRENT_OPERATE = 0x10, //正在结束当前操作
	ISSA_ERROR = 0x14,					//系统故障
} ISSA_WORKING_STATUS_LIST;

//出货口取托盘导航数据
typedef struct GetExportCargoNavData
{
	uint8_t ret;			   //存储操作结果
	int id;					   //帧ID
	double x;				   //导航点　x 坐标
	double y;				   //导航点　y 坐标
	double z;				   //导航点　z 坐标
	double yaw;				   //航线角度偏差(度)
	double position_deviation; //航线位置偏差(mm)
} GetExportCargoNavData;

//取空托盘导航数据
typedef struct GetEmptyStockNavData
{
	uint8_t ret;			   //存储操作结果
	int id;					   //帧ID
	double x;				   //导航点　x 坐标
	double y;				   //导航点　y 坐标
	double z;				   //导航点　z 坐标
	double yaw;				   //航线角度偏差(度)
	double position_deviation; //航线位置偏差(mm)
} GetEmptyStockNavData;

//放空托盘导航数据
typedef struct PutEmptyStockNavData
{
	uint8_t ret;			   //存储操作结果
	int id;					   //帧ID
	double x;				   //导航点　x 坐标
	double y;				   //导航点　y 坐标
	double z;				   //导航点　z 坐标
	double yaw;				   //航线角度偏差(度)
	double position_deviation; //航线位置偏差(mm)
} PutEmptyStockNavData;

typedef struct ObstacleInof
{
	double x;
	double y;
	double z; //障碍物中心点坐标
	double width;
	double height; //障碍物前视图宽高
} ObstacleInof;

//障碍物检测数据
typedef struct ObstaclesInfoData
{
	uint8_t ret;						 //存储操作结果0为成功
	int id;								 //帧ID
	int size;							 //障碍物数量(无障碍物检测到为0)
	std::vector<ObstacleInof> obstacles; //(预留数据暂时没用到)
} ObstaclesInfoData;

//货叉分离检测数据
typedef struct ForkSeparateData
{
	uint8_t ret;	   //存储操作结果0为成功
	int id;			   //帧ID
	uint8_t separated; //0未分离 1分离
} ForkSeparateData;

//访问ISSA操作结果列表
typedef enum ISSA_RET_LIST
{
	ISSA_SUCCESS = 0x00,	  //操作成功
	ISSA_FAIL = 0x01,		  //操作失败,无效指令,数据错误等非法操作
	ISSA_NOFOUND_OBJ = 0x02,  //未识别到目标
	ISSA_SENSOR_ERROR = 0x14, //传感器故障
} ISSA_RET_LIST;

//-------------------------------ISSA Response Data Special----------------------------------

#endif
