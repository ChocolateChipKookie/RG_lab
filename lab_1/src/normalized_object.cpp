//
// Created by kookie on 17. 10. 2020..
//

#include "normalized_object.h"
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <glad/glad.h>

normalized_object::normalized_object(const char *data, size_t len) {
    load(data, len);
    setup();
}

normalized_object::normalized_object(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    assert(in.is_open());
    size_t len = in.tellg();
    in.seekg(0);
    std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    load(data.data(), data.length());
    setup();
}

void normalized_object::load(const char *data, size_t len) {
    size_t no_vertices  = data[0] == 'v' ? 1 : 0;
    size_t no_faces     = data[0] == 'f' ? 1 : 0;

    for (size_t i = 0; i < len - 1; ++i){
        if (data[i] == '\n'){
            if      (data[i + 1] == 'f') ++no_faces;
            else if (data[i + 1] == 'v') ++no_vertices;
        }
    }

    vertices_.reserve(no_vertices);
    faces_.reserve(no_faces);

    std::stringstream ss;
    ss.write(data, len);
    std::string line;

    while (std::getline(ss, line)){
        switch (line[0]) {
            case 'g':
            {
                group_ = line.substr(2);
                break;
            }
            case 'v':
            {
                std::stringstream s(line.substr(2));
                std::vector<float> input_vec { std::istream_iterator<float>(s), std::istream_iterator<float>()};
                vertices_.push_back(vertex{{input_vec[0], input_vec[1], input_vec[2]}, {0.f, 0.f, 0.f}});
                break;
            }
            case 'f':
            {
                std::stringstream s(line.substr(2));
                std::vector<int> input_vec { std::istream_iterator<int>(s), std::istream_iterator<int>()};
                faces_.emplace_back(input_vec[0] - 1, input_vec[1] - 1, input_vec[2] - 1);
                break;
            }
        }
    }

    calculate_normals();
}

normalized_object::normalized_object(std::vector<vertex> vertices, std::vector<face> faces, std::string  group) : faces_(std::move(faces)), vertices_(std::move(vertices)), group_(std::move(group))
{
    setup();
}

void normalized_object::update(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex) * vertices_.size(), vertices_.data());
}

void normalized_object::setup() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face) * faces_.size(), faces_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vertex::pos)));
}

void normalized_object::draw(meg::shader &shader) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(face) * faces_.size(), GL_UNSIGNED_INT, nullptr);
}

void normalized_object::calculate_normals() {
    for(const auto& face : faces_){
        auto normal = glm::normalize(
                glm::cross(
                vertices_[face[1]].pos - vertices_[face[0]].pos,
                vertices_[face[2]].pos - vertices_[face[0]].pos));
        for(unsigned i = 0; i < 3; ++i){
            vertices_[face[i]].normal += normal;
        }
    }

    for(auto& vertex : vertices_){
        vertex.normal = glm::normalize(vertex.normal);
    }
}

void normalized_object::recenter() {
    glm::dvec3 acc;

    for(auto& vertex : vertices_){
        acc += vertex.pos;
    }
    glm::vec3 center = acc / static_cast<double>(vertices_.size());
    for(auto& vertex : vertices_){
        vertex.pos -= center;
    }
    update();
}

void normalized_object::apply_matrix(const glm::mat4 &mat) {
    for(auto& vertex : vertices_){
        vertex.pos = mat * glm::vec4(vertex.pos, 1.f);
    }
    calculate_normals();
    update();
}
