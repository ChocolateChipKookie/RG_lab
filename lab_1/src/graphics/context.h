//
// Created by kookie on 27/03/2020.
//

#ifndef MEEGERENGEN_CONTEXT_H
#define MEEGERENGEN_CONTEXT_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <functional>

namespace meg{
    class context {
    public:
        static void default_hints();
        static void default_options();

        // Context using custom window hints and options
        context(
                int width,
                int height,
                const std::string& name,
                const std::function<void()>& window_hints = default_hints,
                const std::function<void()>& window_options = default_options);
        // Windowed context
        context(int width, int height, const std::string& name, bool depth = false, bool alpha = false);
        // Destructor
        ~context();

        // Apply options
        void apply_options(const std::function<void()>& window_options);

        [[nodiscard]] bool should_close() const;

        void set_process_input(void(*function)(GLFWwindow*, int, int, int, int)) const;
        void set_resize_callback(void(*function)(GLFWwindow*, int, int)) const;
        void set_should_close(bool value) const;
        void swap_buffers() const;
        void poll_events();
        //Blocks thread, waits for seconds amount of seconds for new event
        //Unblocks when event occurs
        //If seconds is < 0 it blocks the thread completly
        void wait_events(double seconds = -1.);
        void set_viewport() const;
        void resize(int width, int height) const;
        [[nodiscard]] unsigned get_width() const;
        [[nodiscard]] unsigned get_height() const;
        void bind();

        void set_clear_colour(float r, float g, float b, float a = 1.f);
        void set_clear_colour(const glm::vec4& colour);
        void set_clear_colour(const glm::vec3& colour);


    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        GLFWwindow* context_{nullptr};
    };
}





#endif //MEEGERENGEN_CONTEXT_H
