#include "header/CustomThread//updateposedatathread.h"
#include "GRS_Common.h"
#include "GRS_SharedMemory.h"
#include <QStringList>
#include <math.h>

UpdatePoseDataThread::UpdatePoseDataThread(QObject *parent) :
    QThread(parent)
{
    // nothing
}

UpdatePoseDataThread::~UpdatePoseDataThread()
{
    QThread::terminate();
}

/*
 * @brief 机器人位姿数据刷新线程的主函数
 *
 * @param 无
 *
 * @return 无
 */
void UpdatePoseDataThread::run()
{
    QString poseData[POSITION_ROWS];    // 包含一个周期的机器人位姿数据
    double w, x, y, z;  // 四元数
    double ex, ey, ez;  // 欧拉角

    while(1)
    {
        /* ***************************************************************************
         * 根据当前对关节、坐标操作，四元数、欧拉角的选择来确定【位置】表中第二列的显示数据。
         * 注意要控制显示精度，这里规定：单位为度的精确到小数点后两位；单位为弧度的精确到小数点后五位；
         * 单位为毫米的精确到小数点后一位；四元数精确到小数点后五位。
         *************************************************************************** */
        if(joggingPar.motionMode == Jogging::PAR_AXIS1_6)   // ---------------- 关节操作 ---------------- （1、2、3、4、5、6）
        {
            for(int i=0; i<POSITION_ROWS-1; i++) {
                if(displayFmt.curUnit == Jogging::PAR_SHOW_DEGREES)  // 关节操作 & 单位度
                {   poseData[i] = QString::number(ITP_radian2angle(pOutputData->curJointTarget.robax.rax[i]), 'f', 2);}
                else if(displayFmt.curUnit == Jogging::PAR_SHOW_RADIANS) // 关节操作 & 单位弧度
                {   poseData[i] = QString::number(pOutputData->curJointTarget.robax.rax[i], 'f', 5);}
            }
            poseData[POSITION_ROWS-1] = tr(" ");
        }
        else    // -------------------- 坐标操作 --------------------
        {
            poseData[0] = QString::number(pOutputData->curRob.pose.trans.x, 'f', 1);    // X
            poseData[1] = QString::number(pOutputData->curRob.pose.trans.y, 'f', 1);    // Y
            poseData[2] = QString::number(pOutputData->curRob.pose.trans.z, 'f', 1);    // Z

#if 1
            w = pOutputData->curRob.pose.rot.s;
            x = pOutputData->curRob.pose.rot.x;
            y = pOutputData->curRob.pose.rot.y;
            z = pOutputData->curRob.pose.rot.z;
#else
            w = pOutputData->curRob.pose.rot.z;
            x = pOutputData->curRob.pose.rot.y;
            y = pOutputData->curRob.pose.rot.x;
            z = pOutputData->curRob.pose.rot.s;
#endif

            if(displayFmt.curOrientFmt == Jogging::PAR_SHOW_QUATERNION)  // 坐标操作 & 四元数 （X、Y、Z、q1、q2、q3、q4）
            {
                poseData[3] = QString::number(w, 'f', 5);
                poseData[4] = QString::number(x, 'f', 5);
                poseData[5] = QString::number(y, 'f', 5);
                poseData[6] = QString::number(z, 'f', 5);
            }
            else if(displayFmt.curOrientFmt == Jogging::PAR_SHOW_EULERANGLES)    // 坐标操作 & 欧拉角 （X、Y、Z、EZ、EY、EX）
            {
                /* ************************************************************************
                 * 由于欧拉角的万向锁特性造成的不确定性，机器人运动控制算法并没有使用欧拉角来计算，
                 * 故在此系统中欧拉角的作用仅仅是提供给用户更直观的数据（相比于四元数）。
                 * 所以我们在这里要自己做四元数到欧拉角的转换。
                 * （pOutputData->curRob.euler.ex 或 ey 或 ez 数据无效）
                 ************************************************************************ */
                ex = atan2((2*w*x + 2*y*z), (-2*x*x - 2*y*y + 1));
                ey = asin(2*w*y - 2*z*x);
                ez = atan2((2*w*z + 2*x*y), (-2*y*y - 2*z*z + 1));

                if(displayFmt.curUnit == Jogging::PAR_SHOW_DEGREES)  // 坐标操作 & 欧拉角 & 单位度
                {
                    poseData[3] = QString::number(ITP_radian2angle(ez), 'f', 2);
                    poseData[4] = QString::number(ITP_radian2angle(ey), 'f', 2);
                    poseData[5] = QString::number(ITP_radian2angle(ex), 'f', 2);
                }
                else if(displayFmt.curUnit == Jogging::PAR_SHOW_RADIANS) // 坐标操作 & 欧拉角 & 单位弧度
                {
                    poseData[3] = QString::number(ez, 'f', 5);
                    poseData[4] = QString::number(ey, 'f', 5);
                    poseData[5] = QString::number(ex, 'f', 5);
                }
                poseData[POSITION_ROWS-1] = tr(" ");
            }
            else    // -------------------- 其他情况 --------------------
            {
                for(int i=0; i<POSITION_ROWS; i++) {poseData[i] = tr("999999.99");}
            }
        }

        emit poseDataUpdated(poseData); // 发送信号，包含一组机器人位置数据<poseData>


        msleep(100);    // 数据刷新周期为100ms
    }
}
