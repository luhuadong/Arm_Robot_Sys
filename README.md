# GSK_Robot_Sys
GSK Robot Controller System with Qt

##介绍

　　本项目设计一个工业机器人控制系统，包括界面设计、机器人控制语言解释器、插补算法等等。项目目前包含以下部分：
  
 - MFC框架显示库 —— 是一个基于 MFC 的工业机器人模拟器，运行在 Windows 平台，支持 scara、delta、puma 模型。
 - GRS_Pro —— 目前包含 GR_HMI、GRS_Core、robDecodePrg 三个部分。其中是 GR_HMI 是基于 Qt 设计的界面，GRS_Core 是系统的核心和通用模块，robDecodePrg 是控制语言解释器模块。
