#include <iostream>
#include "graphics/context.h"
#include "object.h"
#include "normalized_object.h"
#include "graphics/shader.h"
#include "camera.h"
#include "line_strip.h"
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iterator>

// Camera
camera cam({0.5, 0.5, 0}, 0, 90);
float cam_sensitivity_angle = 1.f;
float cam_sensitivity_movement = 0.01;

void cam_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Key callback for camera
        switch (key) {
            case GLFW_KEY_ESCAPE:
            {
                glfwSetWindowShouldClose(window, true);
                break;
            }
            case GLFW_KEY_A:
            {
                auto right = cam.right();
                cam.set_position(cam.get_position() + right * cam_sensitivity_movement);
                break;
            }
            case GLFW_KEY_D:
            {
                auto right = cam.right();
                cam.set_position(cam.get_position() - right * cam_sensitivity_movement);
                break;
            }
            case GLFW_KEY_W:
            {
                cam.set_position(cam.get_position() + cam.direction() * cam_sensitivity_movement);
                break;
            }
            case GLFW_KEY_S:
            {
                cam.set_position(cam.get_position() - cam.direction() * cam_sensitivity_movement);
                break;
            }
            case GLFW_KEY_RIGHT:
            {
                cam.set_yaw(cam.get_yaw() + cam_sensitivity_angle);
                break;
            }
            case GLFW_KEY_LEFT:
            {
                cam.set_yaw(cam.get_yaw() - cam_sensitivity_angle);
                break;
            }
            case GLFW_KEY_UP:
            {
                cam.set_pitch(cam.get_pitch() + cam_sensitivity_angle);
                break;
            }
            case GLFW_KEY_DOWN:
            {
                cam.set_pitch(cam.get_pitch() - cam_sensitivity_angle);
                break;
            }
        }
}

glm::vec3 calculate_b_spline(const std::vector<glm::vec3>& handles, float t, size_t current_handle){
    // Calculate b spline
    const glm::mat4 b = glm::mat4{-1.f,  3.f, -3.f, 1.f,
                                   3.f, -6.f,  3.f, 0.f,
                                  -3.f,  0.f,  3.f, 0.f,
                                   1.f,  4.f,  1.f, 0.f} / 6.f;

    const glm::vec4 t4{t*t*t, t*t, t, 1.f};

    const glm::mat4x3 r = {handles[current_handle - 1],
                           handles[current_handle - 0],
                           handles[current_handle + 1],
                           handles[current_handle + 2]};

    return r * b * t4;
}

glm::vec3 calculate_b_tangent(const std::vector<glm::vec3>& handles, float t, size_t current_handle){
    // Calculate the b_spline tangent in given point
    const glm::mat3x4 b = glm::mat3x4{-1.f,  3.f, -3.f, 1.f,
                                       2.f, -4.f,  2.f, 0.f,
                                      -1.f,  0.f,  1.f, 0.f} / 2.f;

    const glm::vec3 t3{t*t, t, 1.f};
    const glm::mat4x3 r = {handles[current_handle - 1],
                           handles[current_handle - 0],
                           handles[current_handle + 1],
                           handles[current_handle + 2]};

    return r * b * t3;
}

std::vector<glm::vec3> load_path(const std::string& path){
    // Load the path that the object should follow
    std::ifstream file(path);
    std::string line;
    std::vector<glm::vec3> handles;
    while (std::getline(file, line)){
        std::stringstream s(line);
        std::vector<float> input_vec { std::istream_iterator<float>(s), std::istream_iterator<float>()};
        handles.emplace_back(input_vec[0], input_vec[1], input_vec[2]);
    }
    return handles;
}

glm::mat4 rotation_matrix(const glm::vec3& s, const glm::vec3& e){
    // Calculates the rotation matrix
    glm::vec3 os{  s.y * e.z - e.y * s.z,
                 -(s.x * e.z - e.x * s.z),
                   s.x * e.y - s.y  *e.x};
    float angle = glm::cos(glm::dot(s, e)/(glm::length(s) * glm::length(e)));
    return glm::rotate(glm::mat4(1.f), angle, os);
}

int main() {
    // Init window
    float width{720.f};
    float height{480.f};
    meg::context window(static_cast<int>(width), static_cast<int>(height), "Test window", true);
    window.set_process_input(cam_key_callback);

    // Define used matrices
    glm::mat4 identity(1.f);
    glm::mat4 model(1.f);
    glm::mat4 view(1.f);
    glm::mat4 projection = glm::perspective(glm::radians(45.f), width/height, 0.1f, 100.f);

    //Define the triangle shader
    meg::shader triangle_shader("../src/graphics/shaders/triangle_lighting.sha");
    triangle_shader.use();
    // Model
    triangle_shader.set_mat4f("projection", projection);
    triangle_shader.set_mat4f("model", model);
    triangle_shader.set_mat4f("view", view);
    // Light
    triangle_shader.set3f("objectColour", 1, 1, 1);
    triangle_shader.set3f("lightColour", 1, 1, 1);
    triangle_shader.set3f("lightPos", 0, 100, -100);

    // Define the line shader
    meg::shader line_shader("../src/graphics/shaders/triangle.sha");
    line_shader.use();
    line_shader.set_mat4f("projection", projection);
    line_shader.set_mat4f("model", identity);
    line_shader.set_mat4f("view", view);
    line_shader.set4f("colour", 1, 0, 0, 1);

    // Load object
    normalized_object galleon("../resources/galleon.obj");
    // Recenter object
    galleon.recenter();
    // Rotate the object so it is facing in the positive z direction
    galleon.apply_matrix(glm::rotate(identity, glm::radians(-90.f), {1, 0, 0}));
    // Load object
    normalized_object peter("../resources/peter_pan.obj");
    // Recenter object
    peter.recenter();
    // Rotate the object so it is facing in the positive z direction
    peter.apply_matrix(
            glm::rotate(identity, glm::radians(90.f), {0, 1, 0}) *
            glm::translate(identity, {-0.25, 0.05, 0})*
            glm::scale(identity, {0.075, 0.075, 0.075})
            );

    // Set the s vector to 1
    glm::vec3 s = {0, 0, 1};

    // Load the path to follow and construct the b_spline for the lines
    auto path = load_path("../resources/path.txt");
    for(auto& handle : path){
        handle /= 10.f;
    }
    std::vector<glm::vec3> b_spline;
    for(unsigned i = 1; i < path.size() - 2; ++i){
        for(float t = 0.f; t <= 1.f; t+=0.01){
            b_spline.push_back(calculate_b_spline(path, t, i));
        }
    }

    // Construct line objects
    line_strip ls_b(b_spline);
    line_strip ls_p(path);

    // Declare variables used for the b_spline calculations
    float t = 0;
    float increment = 0.01;
    size_t current_handle = 1;

    std::chrono::milliseconds delay(33);

    while (!window.should_close()){
        //window.bind();
        //window.set_viewport();

        auto frame_begin = std::chrono::steady_clock::now();

        // Clear window
        glClearColor(207/255.f, 225/255.f, 252/255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get view matrix from camera
        view = cam.get_look_at();

        // Update line shader
        line_shader.use();
        line_shader.set_mat4f("view", view);
        // Set colour for b_spline
        line_shader.set4f("colour", 1, 0, 0, 1);
        ls_b.draw(line_shader);
        // Set colour for lines defining the b_spline
        line_shader.set4f("colour", 0, 1, 0, 1);
        ls_p.draw(line_shader);

        // Update the b_spline variables
        t += increment;
        if(t > 1.f){
            t = 0.f;
            current_handle = current_handle < (path.size() - 3) ? current_handle + 1 : 1;
        }


        // Use the trinalge shader
        triangle_shader.use();
        // Calculate the translation matrix
        auto trans = calculate_b_spline(path, t, current_handle);
        // Calculate the direction matrix
        auto direction = calculate_b_tangent(path, t, current_handle);
        // Calculate the rotation matrix using the direction and s vector
        auto rotation = rotation_matrix(s, direction);

        // Calculate the final translation + rotation matrix
        model = glm::translate(identity, trans) * rotation;

        // Set the uniforms for drawing the object
        triangle_shader.set_mat4f("model", model);
        triangle_shader.set_mat4f("view", view);
        // Draw object
        triangle_shader.set3f("objectColour", 1, 1, 1);
        galleon.draw(triangle_shader);
        triangle_shader.set3f("objectColour", 0, 1, 0);
        peter.draw(triangle_shader);

        // Swap buffers and poll events
        window.swap_buffers();
        window.poll_events();

        // 30 fps
        auto frame_end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(delay - (frame_end - frame_begin));
    }
    return 0;
}
