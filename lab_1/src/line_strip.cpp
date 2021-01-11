//
// Created by kookie on 18. 10. 2020..
//

#include <glad/glad.h>
#include "line_strip.h"

void line_strip::draw(meg::shader &shader) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, points_.size());
}

void line_strip::setup() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points_.size(), points_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
}

void line_strip::apply_matrix(const glm::mat4 &mat) {
    for(auto& point : points_){
        point = mat * glm::vec4(point, 1.f);
    }
    setup();
}


