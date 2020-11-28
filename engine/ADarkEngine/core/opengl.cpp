
#include "ADarkEngine/core/opengl.h"

#define GL_LOAD_PROC(type, procName) \
glProcedures->procName = (type*)os->LoadOpenGLProcedure(#procName); \
if(!glProcedures->procName) \
return 0

internal b32
LoadAllOpenGLProcedures(os_call* os, gl_procedures* glProcedures)
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
    
    return 1;
}