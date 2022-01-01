#include "Renderer.h"
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGLError]" << error << "Function: " << function << "File: " << file << "Line: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    
}

void Renderer::DrawTriangles(const VertexArray& va, const Shader& shader) const
{
    shader.Bind();
    va.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
