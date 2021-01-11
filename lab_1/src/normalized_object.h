//
// Created by kookie on 17. 10. 2020..
//

#ifndef LAB_1_NORMALIZED_OBJECT_H
#define LAB_1_NORMALIZED_OBJECT_H


#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "graphics/shader.h"

class normalized_object {
public:

    struct vertex {
        glm::vec3 pos;
        glm::vec3 normal;
    };
    using face = glm::ivec3;

    explicit normalized_object(const std::string& filename);
    normalized_object(const char* data, size_t len);
    normalized_object(std::vector<vertex>  vertices, std::vector<face>  faces, std::string  group);

    void draw(meg::shader& shader);
    void recenter();
    void apply_matrix(const glm::mat4& mat);

private:
    void calculate_normals();
    void load(const char* data, size_t len);
    void update();
    void setup();

    unsigned int VAO{}, VBO{}, EBO{};
    std::string group_;
    std::vector<vertex> vertices_;
    std::vector<face> faces_;
};


#endif //LAB_1_NORMALIZED_OBJECT_H
