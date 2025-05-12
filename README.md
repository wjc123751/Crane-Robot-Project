# Crane-Robot-Project
本科竞赛
# 智能物流机器人电控系统

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![Build Status](https://img.shields.io/badge/Platform-STM32F4-yellowgreen)

## 项目概述
本仓库为2023年全国大学生智能物流机器人挑战赛获奖作品电控系统，提供完整的物流机器人运动控制解决方案。系统通过FreeRTOS实现多电机协同控制，具备**全流程自动化执行**、**高精度运动控制**和**异常状态自检**等核心功能。

## 核心功能
### 🚀 运动控制模块
- **四轮差速驱动**  
  基于PID的麦克纳姆轮闭环控制（500Hz刷新率）
- **旋转机构精确定位**  
  17位绝对值编码器反馈，定位精度±0.5°
- **双机械臂协同**  
  抓取机构6自由度联动控制（前/后各3电机）

### ⚙️ 执行机构
| 组件            | 控制方式       | 关键指标                  |
|-----------------|--------------|-------------------------|
| 气泵装置         | PWM调压      | 气压控制精度±2kPa        |
| 舵机云台         | 位置伺服      | 响应时间＜80ms          |
| 伸缩滑台         | 闭环步进      | 重复定位精度0.1mm       |

### 🧠 智能决策
- **一键启动流程**  
  全自动任务序列执行（包含10+个动作阶段）
- **异常处理机制**  
  电流/温度/位置三重安全监测
- **能耗优化策略**  
  动态功率分配算法（节能35%）

## 技术实现
### 硬件架构
```c
// 典型控制信号流
Sensors → STM32F407 → DRV8323驱动 → 电机
                   │
                   ├─PCA9685 → 舵机组
                   └─TLE9201 → 气泵阀

## 软件设计
FreeRTOS任务结构：
├── 主控任务（优先级5）
│    ├── 流程状态机
│    └── 系统心跳包
├── 运动控制任务（优先级6）
│    ├── 底盘解算
│    ├── 机械臂逆运动学
│    └── PID计算线程  
└── 安全监控任务（优先级7）
     ├── 过流保护
     └── 紧急制动

## 自适应PID调参算法​
def auto_tune(kp, ki, kd):
    while abs(error) > threshold:
        # 基于梯度下降的在线优化
        kp += α * (dE/dkp)
        ki += β * (dE/dki) 
        kd += γ * (dE/dkd)
    return optimized_params

## 开发环境
STM32CubeIDE 1.11
FreeRTOS 10.4.3
C/C++11标准

## 快速部署
git clone https://github.com/wjc123751/Crane-Robot-Project.git
cd src && make flash BOARD=STM32F407VET6
