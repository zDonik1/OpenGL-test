// Headers
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core

    in float inValue;

    out float outValue;

    void main()
    {
        outValue = sqrt(inValue);
    }
)glsl";

GLfloat values[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

int main()
{
    sf::Context context;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create VAOs
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Load values
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(values), values, GL_STATIC_DRAW);

    // Create shader programs
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &vertexSource, nullptr);
    glCompileShader(shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, shader);

    const GLchar *feedbackVaryings[] = { "outValue" };
    glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(program);
    glUseProgram(program);

    // Specify vertex attribs
    GLint inputAttrib = glGetAttribLocation(program, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Create output buffer
    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(values), nullptr, GL_STATIC_READ);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

    // Run shaders
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();
    glFlush();

    GLfloat feedback[5];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    for (unsigned int i = 0; i < 5; ++i) {
        std::cout << feedback[i] << std::endl;
    }

    // Cleanup
    glDeleteBuffers(1, &tbo);

    glDeleteProgram(program);
    glDeleteShader(shader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    return 0;
}
