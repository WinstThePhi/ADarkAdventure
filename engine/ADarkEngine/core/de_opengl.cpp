
#include "core/de_opengl.h"

#define GL_LOAD_PROC(type, procName) \
glProcedures->procName = (type*)os->LoadOpenGLProcedure(#procName); 

internal b32
DE_LoadAllOpenGLProcedures(os_call* os, gl_procedures* glProcedures)
{
    GL_LOAD_PROC(gl_gen_buffers, glGenBuffers);
    GL_LOAD_PROC(gl_bind_buffer, glBindBuffer);
    GL_LOAD_PROC(gl_buffer_data, glBufferData);
    GL_LOAD_PROC(gl_create_shader, glCreateShader);
    GL_LOAD_PROC(gl_shader_source, glShaderSource);
    GL_LOAD_PROC(gl_compile_shader, glCompileShader);
    GL_LOAD_PROC(gl_get_shader_iv, glGetShaderiv);
    GL_LOAD_PROC(gl_get_shader_info_log, glGetShaderInfoLog);
    GL_LOAD_PROC(gl_create_program, glCreateProgram);
    GL_LOAD_PROC(gl_attach_shader, glAttachShader);
    GL_LOAD_PROC(gl_link_program, glLinkProgram);
    GL_LOAD_PROC(gl_get_program_iv, glGetProgramiv);
    GL_LOAD_PROC(gl_get_program_info_log, glGetProgramInfoLog);
    GL_LOAD_PROC(gl_use_program, glUseProgram);
    GL_LOAD_PROC(gl_delete_shader, glDeleteShader);
    GL_LOAD_PROC(gl_vertex_attrib_pointer, glVertexAttribPointer);
    GL_LOAD_PROC(gl_enable_vertex_attrib_array, glEnableVertexAttribArray);
    GL_LOAD_PROC(gl_gen_vertex_arrays, glGenVertexArrays);
    GL_LOAD_PROC(gl_draw_arrays, glDrawArrays);
    GL_LOAD_PROC(gl_bind_vertex_array, glBindVertexArray);
    GL_LOAD_PROC(gl_polygon_mode, glPolygonMode);
    GL_LOAD_PROC(gl_draw_elements, glDrawElements);
    GL_LOAD_PROC(gl_delete_vertex_arrays, glDeleteVertexArrays);
    GL_LOAD_PROC(gl_delete_buffers, glDeleteBuffers);
    GL_LOAD_PROC(gl_delete_program, glDeleteProgram);
    GL_LOAD_PROC(gl_get_uniform_location, glGetUniformLocation);
    GL_LOAD_PROC(gl_uniform_4f, glUniform4f);
    
    return 1;
}

inline gl_color
DE_RGBAToGLColor(v4 rgba)
{
    gl_color result = 
    {
        (((f32)rgba.r) / 255.0f),
        (((f32)rgba.g) / 255.0f),
        (((f32)rgba.b) / 255.0f),
        (((f32)rgba.a) / 255.0f)
    };
    return result;
}

inline char* 
DE_ReadShader(temp_memory& memory, char* path)
{
    return DE_ReadFileByLine(memory, path);
}

internal u32
_DE_CreateShader(gl_procedures* gl, 
                 char* shaderSource, 
                 u32 shaderType,
                 char* filename, u32 lineNumber)
{
    GLuint shader;
    shader = gl->glCreateShader(shaderType);
    gl->glShaderSource(shader, 1, &shaderSource, 0);
    gl->glCompileShader(shader);
    
    GLint success;
    GLchar infoLog[512];
    
    gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        gl->glGetShaderInfoLog(shader, 512, 0, infoLog);
        DE_LogError(infoLog);
    }
    else
    {
        GLchar buffer[256];
        if(shaderType == GL_FRAGMENT_SHADER)
            snprintf(buffer, 256, 
                     "OpenGL (%s, line %d): Fragment shader successfully compiled.",filename, lineNumber);
        else
            snprintf(buffer, 256, 
                     "OpenGL (%s, line %d): Vertex shader successfully compiled.",filename, lineNumber);
        
        DE_LogInfo(buffer);
    }
    
    return shader;
}

internal u32 
_DE_CreateShaderProgram(gl_procedures* gl, 
                        u32 vertexShader, u32 fragmentShader,
                        char* filename, u32 lineNumber)
{
    
    u32 shaderProgram;
    shaderProgram = gl->glCreateProgram();
    
    gl->glAttachShader(shaderProgram, vertexShader);
    gl->glAttachShader(shaderProgram, fragmentShader);
    gl->glLinkProgram(shaderProgram);
    
    i32 success;
    char infoLog[512];
    
    gl->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        gl->glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        DE_LogError(infoLog);
    }
    else
    {
        char buffer[256];
        snprintf(buffer, 256, 
                 "OpenGL (%s, line %d): Shader program successfully linked.", filename, lineNumber);
        
        DE_LogInfo(buffer);
    }
    
    return shaderProgram;
}

