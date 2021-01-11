//
// Created by kookie on 16. 10. 2020..
//

#include "object.h"
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <glad/glad.h>

object::object(const char *data, size_t len) {
    load(data, len);
    setup();
}

object::object(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    assert(in.is_open());
    size_t len = in.tellg();
    in.seekg(0);
    std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    load(data.data(), data.length());
    setup();
}

void object::load(const char *data, size_t len) {
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
                vertices_.emplace_back(input_vec[0], input_vec[1], input_vec[2]);
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
}

object::object(std::vector<vertex> vertices, std::vector<face> faces, std::string  group) : faces_(std::move(faces)), vertices_(std::move(vertices)), group_(std::move(group))
{
    setup();
}

void object::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face) * faces_.size(), faces_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void object::draw(meg::shader &shader) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(face) * faces_.size(), GL_UNSIGNED_INT, 0);
}
