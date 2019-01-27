#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "threemath.h"

namespace luwu
{
    //一种实现方式是记录位置和view坐标系的两个坐标轴，使用lookat来计算view的矩阵
    //一种是类似unity的camera，记录位置，角度信息

    enum Camera_Movement
    {
        kForward = 0,
        kBackward,
        kLeft,
        kRight,
    };

    //本次使用记录坐标轴的方法
    class Camera
    {
    public:
        Camera(Vector3 position,float yaw=-90.0f,float pitch=0.0f):
            position_(position),
            yaw_(yaw),
            pitch_(pitch)
        {
            worldUp_ = Vector3(0.0f,1.0f,0.0f);

            updateCameraVectors();
        }

        Matrix4x4 getMatrix()
        {
            return LookAt(position_,position_ + front_,worldUp_);
        }

        Vector3 GetPosition()
        {
            return position_;
        }

        void ProcessMouseMovement(float xOffset,float yOffset,bool constraintPitch=true)
        {
            const float sensitivity = 0.25f;
            yaw_ += xOffset * sensitivity;
            pitch_ += yOffset * sensitivity;

            if(constraintPitch)
            {
                if(pitch_ < -89.0f)
                {
                    pitch_ = 89.0f;
                }
                if(pitch_ > 89.0)
                {
                    pitch_ = 89.0;
                }
            }

            updateCameraVectors();
        }

        void ProcessKeyboard(Camera_Movement dir,float deltaTime)
        {
            float velocity = 0.5 * deltaTime;
            if(dir == Camera_Movement::kForward)
            {
                position_ += front_ * velocity;
            }
            else if(dir == Camera_Movement::kBackward)
            {
                position_ -= front_ * velocity;
            }
            else if(dir == Camera_Movement::kLeft)
            {
                position_ -= right_ * velocity;
            }
            else if(dir == Camera_Movement::kRight)
            {
                position_ += right_ * velocity;
            }
            else{
                //do nothing
            }
        }

    private:
        Vector3 position_;
        Vector3 right_;
        Vector3 up_; 
        Vector3 front_;
        Vector3 worldUp_;
        float yaw_;
        float pitch_;

        void updateCameraVectors()
        {
            front_.x = cos(RADIAN(yaw_)) * cos(RADIAN(pitch_));
            front_.y = sin(RADIAN(pitch_));
            front_.z = sin(RADIAN(yaw_)) * cos(RADIAN(pitch_));
            
            right_ = Vector3::Cross(front_,worldUp_);
            up_ = Vector3::Cross(right_,front_);
        }
    };
}

#endif