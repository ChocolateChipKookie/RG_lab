//
// Created by kookie on 17. 10. 2020..
//

#include "camera.h"


void camera::update() {
    if(!updated){
        direction_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        direction_.y = sin(glm::radians(pitch_));
        direction_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        direction_ = glm::normalize(direction_);

        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        right_ = glm::normalize(glm::cross(up, direction_));
        up_ = glm::cross(direction_, right_);

        look_at_ = glm::lookAt(position_, position_ + direction_, up_);
        updated = true;
    }
}

const glm::mat4 &camera::get_look_at() {
    update();
    return look_at_;
}


