//
// Created by kookie on 16. 10. 2020..
//

#ifndef LAB_1_OBJECT_H
#define LAB_1_OBJECT_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "graphics/shader.h"

class object {
public:

    using vertex = glm::vec3;
    using face = glm::ivec3;

    explicit object(const std::string& filename);
    object(const char* data, size_t len);
    object(std::vector<vertex>  vertices, std::vector<face>  faces, std::string  group);

    void draw(meg::shader& shader);

private:
    void load(const char* data, size_t len);
    void setup();

    unsigned int VAO{}, VBO{}, EBO{};
    std::string group_;
    std::vector<vertex> vertices_;
    std::vector<face> faces_;
};


#endif //LAB_1_OBJECT_H
