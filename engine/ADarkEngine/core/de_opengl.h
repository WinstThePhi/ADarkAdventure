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
typedef float GLfloat;

#ifndef GL_FALSE
#define GL_FALSE                          0
#endif

#ifndef GL_TRUE 
#define GL_TRUE                           1
#endif 

#ifndef GL_TRIANGLES
#define GL_TRIANGLES                      0x0004
#endif

#ifndef GL_UNSIGNED_INT
#define GL_UNSIGNED_INT                   0x1405
#endif 

#ifndef GL_FLOAT
#define GL_FLOAT                          0x1406
#endif

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER                   0x8892
#endif

#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#endif

#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW                    0x88E4
#endif

#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER                0x8B30
#endif

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER                  0x8B31
#endif

#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS                 0x8B81
#endif

#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS                    0x8B82
#endif 

typedef void gl_gen_buffers(GLsizei n, GLuint *buffers);
typedef void gl_bind_buffer(GLenum target, GLuint buffer);
typedef void gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef GLuint gl_create_shader(GLenum type);
typedef void gl_shader_source(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void gl_compile_shader(GLuint shader);
typedef void gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
typedef void gl_get_shader_info_log(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint gl_create_program(void);
typedef void gl_attach_shader(GLuint program, GLuint shader);
typedef void gl_link_program(GLuint program);
typedef void gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
typedef void gl_get_program_info_log(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void gl_use_program(GLuint program);
typedef void gl_delete_shader(GLuint shader);
typedef void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void gl_enable_vertex_attrib_array(GLuint index);
typedef void gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
typedef void gl_draw_arrays(GLenum mode, GLint first, GLsizei count);
typedef void gl_bind_vertex_array(GLuint array);
typedef void gl_polygon_mode(GLenum face, GLenum mode);
typedef void gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays);
typedef void gl_delete_buffers(GLsizei n, const GLuint *buffers);
typedef void gl_delete_program(GLuint program);
typedef GLint gl_get_uniform_location(GLuint program, const GLchar *name);
typedef void gl_uniform_4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

extern void glGenBuffers(GLsizei n, GLuint *buffers);
extern void glBindBuffer(GLenum target, GLuint buffer);
extern void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern GLuint glCreateShader(GLenum type);
extern void glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern void glCompileShader(GLuint shader);
extern void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
extern void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern GLuint glCreateProgram(void);
extern void glAttachShader(GLuint program, GLuint shader);
extern void glLinkProgram(GLuint program);
extern void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
extern void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void glUseProgram(GLuint program);
extern void glDeleteShader(GLuint shader);
extern void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern void glEnableVertexAttribArray(GLuint index);
extern void glGenVertexArrays(GLsizei n, GLuint *arrays);
extern void glBindVertexArray(GLuint array);
extern void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
extern void glDeleteBuffers(GLsizei n, const GLuint *buffers);
extern void glDeleteProgram(GLuint program);
extern GLint glGetUniformLocation(GLuint program, const GLchar *name);
extern void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void glDrawArrays(GLenum mode, GLint first, GLsizei count);
extern void glPolygonMode(GLenum face, GLenum mode);
extern void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);

typedef struct gl_procedures
{
    gl_gen_buffers*                glGenBuffers;
    gl_bind_buffer*                glBindBuffer;
    gl_buffer_data*                glBufferData;
    gl_create_shader*              glCreateShader;
    gl_shader_source*              glShaderSource;
    gl_compile_shader*             glCompileShader;
    gl_get_shader_iv*              glGetShaderiv;
    gl_get_shader_info_log*        glGetShaderInfoLog;
    gl_create_program*             glCreateProgram;
    gl_attach_shader*              glAttachShader;
    gl_link_program*               glLinkProgram;
    gl_get_program_iv*             glGetProgramiv;
    gl_get_program_info_log*       glGetProgramInfoLog;
    gl_use_program*                glUseProgram;
    gl_delete_shader*              glDeleteShader;
    gl_vertex_attrib_pointer*      glVertexAttribPointer;
    gl_enable_vertex_attrib_array* glEnableVertexAttribArray;
    gl_gen_vertex_arrays*          glGenVertexArrays;
    gl_draw_arrays*                glDrawArrays;
    gl_bind_vertex_array*          glBindVertexArray;
    gl_polygon_mode*               glPolygonMode;
    gl_draw_elements*              glDrawElements;
    gl_delete_vertex_arrays*       glDeleteVertexArrays;
    gl_delete_buffers*             glDeleteBuffers;
    gl_delete_program*             glDeleteProgram;
    gl_get_uniform_location*       glGetUniformLocation;
    gl_uniform_4f*                 glUniform4f;
} gl_procedures;

typedef struct gl_color
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} gl_color;

internal b32 DE_LoadAllOpenGLProcedures(os_call* osCall, 
                                        gl_procedures* glProcedures);

inline gl_color DE_RGBAToGLColor(v4 rgba);

inline char* DE_ReadShader(temp_memory& memory, char* path);

internal u32 _DE_CreateShader(gl_procedures* gl, 
                              char* shaderSource, 
                              u32 shaderType, 
                              char* filename, u32 lineNumber);

internal u32 _DE_CreateShaderProgram(gl_procedures* gl, 
                                     u32 vertexShader, u32 fragmentShader,
                                     char* filename, u32 lineNumber);

#define DE_CreateShader(glProc, shaderSource, shaderType) \
_DE_CreateShader(glProc, shaderSource, shaderType, __FILE__, __LINE__);

#define DE_CreateShaderProgram(glProc, vertexShader, fragmentShader) \
_DE_CreateShaderProgram(glProc, vertexShader, fragmentShader, __FILE__, __LINE__);

#define DE_FreeShader(glProc, shader) \
glProc.glDeleteShader(shader);

#endif
