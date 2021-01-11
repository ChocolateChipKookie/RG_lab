//
// Created by kookie on 17. 10. 2020..
//

#ifndef LAB_1_CAMERA_H
#define LAB_1_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cmath>

class camera {
public:
    camera() = default;
    camera(const glm::vec3& pos, float pitch, float yaw) : position_{pos}, pitch_{pitch}, yaw_{yaw}{};

    void update();
    const glm::mat4& get_look_at();


    inline float get_pitch() const{
        return pitch_;
    }

    inline float get_yaw() const{
        return yaw_;
    }

    inline void set_pitch(float pitch){
        updated = false;
        pitch_ = std::min(std::max(pitch, -89.f), 89.f);
    }

    inline void set_yaw(float yaw){
        updated = false;
        yaw_ = std::fmod(yaw, 360.f);
    }

    inline const glm::vec3& get_position() const{
        return position_;
    }

    inline void set_position(const glm::vec3& position){
        updated = false;
        position_ = position;
    }


    inline const glm::vec3& direction() {
        if(!updated)
            update();
        return direction_;
    }

    inline glm::vec3& right() {
        if(!updated)
            update();
        return right_;
    }

    inline glm::vec3& up() {
        if(!updated)
            update();
        return up_;
    }


private:
    glm::vec3 position_{};
    glm::vec3 direction_{};
    glm::vec3 right_{};
    glm::vec3 up_{};
    float pitch_{0.f};
    float yaw_{0.f};

    glm::mat4 look_at_{};
    bool updated = false;
};


#endif //LAB_1_CAMERA_H
