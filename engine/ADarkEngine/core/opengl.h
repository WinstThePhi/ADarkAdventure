#ifndef OPENGL_H
#define OPENGL_H

typedef void GLvoid;
typedef unsigned int GLenum;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef signed long long int GLsizeiptr;
typedef char GLchar;

#define GL_GEN_BUFFERS(name) void name(GLsizei n, GLuint *buffers)
#define GL_BIND_BUFFER(name) void name(GLenum target, GLuint buffer)
#define GL_BUFFER_DATA(name) void name(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
#define GL_CREATE_SHADER(name) GLuint name(GLenum type)
#define GL_SHADER_SOURCE(name) void name(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
#define GL_COMPILE_SHADER(name) void name(GLuint shader)
#define GL_GET_SHADER_IV(name) void name(GLuint shader, GLenum pname, GLint *params)
#define GL_GET_SHADER_INFO_LOG(name) void name(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
#define GL_CREATE_PROGRAM(name) GLuint name(void)
#define GL_ATTACH_SHADER(name) void name(GLuint program, GLuint shader)
#define GL_LINK_PROGRAM(name) void name(GLuint program)
#define GL_GET_PROGRAM_IV(name) void name(GLuint program, GLenum pname, GLint *params)
#define GL_GET_PROGRAM_INFO_LOG(name) void name(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
#define GL_USE_PROGRAM(name) void name(GLuint program)
#define GL_DELETE_SHADER(name) void name(GLuint shader)
#define GL_VERTEX_ATTRIB_POINTER(name) void name(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
#define GL_ENABLE_VERTEX_ATTRIB_ARRAY(name) void name(GLuint vaobj, GLuint index)
#define GL_GEN_VERTEX_ARRAYS(name) void name(GLsizei n, GLuint *arrays)
#define GL_DRAW_ARRAYS(name) void name(GLenum mode, GLint first, GLsizei count)

typedef GL_GEN_BUFFERS(gl_gen_buffers);
typedef GL_BIND_BUFFER(gl_bind_buffer);
typedef GL_BUFFER_DATA(gl_buffer_data);
typedef GL_CREATE_SHADER(gl_create_shader);
typedef GL_SHADER_SOURCE(gl_shader_source);
typedef GL_COMPILE_SHADER(gl_compile_shader);
typedef GL_GET_SHADER_IV(gl_get_shader_iv);
typedef GL_GET_SHADER_INFO_LOG(gl_get_shader_info_log);
typedef GL_CREATE_PROGRAM(gl_create_program);
typedef GL_ATTACH_SHADER(gl_attach_shader);
typedef GL_LINK_PROGRAM(gl_link_program);
typedef GL_GET_PROGRAM_IV(gl_get_program_iv);
typedef GL_GET_PROGRAM_INFO_LOG(gl_get_program_info_log);
typedef GL_USE_PROGRAM(gl_use_program);
typedef GL_DELETE_SHADER(gl_delete_shader);
typedef GL_VERTEX_ATTRIB_POINTER(gl_vertex_attrib_pointer);
typedef GL_ENABLE_VERTEX_ATTRIB_ARRAY(gl_enable_vertex_attrib_array);
typedef GL_GEN_VERTEX_ARRAYS(gl_gen_vertex_arrays);
typedef GL_DRAW_ARRAYS(gl_draw_arrays);

typedef struct gl_procedures
{
    gl_gen_buffers* glGenBuffers;
    gl_bind_buffer* glBindBuffer;
    gl_buffer_data* glBufferData;
    gl_create_shader* glCreateShader;
    gl_shader_source* glShaderSource;
    gl_compile_shader* glCompileShader;
    gl_get_shader_iv* glGetShaderiv;
    gl_get_shader_info_log* glGetShaderInfoLog;
    gl_create_program* glCreateProgram;
    gl_attach_shader* glAttachShader;
    gl_link_program* glLinkProgram;
    gl_get_program_iv* glGetProgramiv;
    gl_get_program_info_log* glGetProgramInfoLog;
    gl_use_program* glUseProgram;
    gl_delete_shader* glDeleteShader;
    gl_vertex_attrib_pointer* glVertexAttribPointer;
    gl_enable_vertex_attrib_array* glEnableVertexAttribArray;
    gl_gen_vertex_arrays* glGenVertexArrays;
    gl_draw_arrays* glDrawArrays;
} gl_procedures;

internal b32 LoadAllOpenGLProcedures(os_call* osCall, 
                                     gl_procedures* glProcedures);

#endif
