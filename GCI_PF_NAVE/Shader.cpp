#include "Shader.h"

Shader::Shader(const std::vector<ShaderFile>& shaders) :
    _object(0)
{
    string errorMessage = "";

    if (shaders.size() <= 0)
    {
        errorMessage = "No hay shaders en la lista para crear";

        Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        throw std::runtime_error(errorMessage);
        PostQuitMessage(0);
    }

    //create the program object
    _object = glCreateProgram();

    if (_object == 0)
    {
        errorMessage = "Fallo al crear el shader";

        Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        throw std::runtime_error(errorMessage);
        PostQuitMessage(0);
    }

    //attach all the shaders
    for (unsigned i = 0; i < shaders.size(); ++i)
    {
        glAttachShader(_object, shaders[i].object());
    }

    //link the shaders together
    glLinkProgram(_object);

    //detach all the shaders
    for (unsigned i = 0; i < shaders.size(); ++i)
    {
        glDetachShader(_object, shaders[i].object());
    }

    //throw exception if linking failed
    GLint status;
    glGetProgramiv(_object, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) 
    {
        errorMessage = "Fallo al linkear los shaders: ";

        GLint infoLogLength;
        glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);

        errorMessage += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(_object); _object = 0;

        Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        throw std::runtime_error(errorMessage);
        PostQuitMessage(0);
    }
}

Shader::~Shader() 
{
    //might be 0 if ctor fails by throwing exception
    if (_object != 0) glDeleteProgram(_object);
}

GLuint Shader::object() const 
{
    return _object;
}

void Shader::use() const 
{
    glUseProgram(_object);
}

bool Shader::isInUse() const 
{
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint)_object);
}

void Shader::stopUsing() const 
{
    assert(isInUse());
    glUseProgram(0);
}

GLint Shader::attrib(const GLchar* attribName) const 
{
    string strAttribName = attribName;
    string errorMessage = "";

    if (!attribName)
    {
        errorMessage = "El nombre del atributo esta vacio";

        Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        // throw std::runtime_error(errorMessage);
        PostQuitMessage(0);
    }

    GLint attrib = glGetAttribLocation(_object, attribName);

    if (attrib == -1)
    {
        errorMessage = "No se encontro el atributo especificado <" + strAttribName + ">";

        // Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        // throw std::runtime_error(errorMessage);
        // PostQuitMessage(0);
    }

    return attrib;
}

GLint Shader::uniform(const GLchar* uniformName) const 
{
    string strUniformName = uniformName;
    string errorMessage = "";

    if (!uniformName)
    {
        errorMessage = "El nombre del uniform esta vacio";

        Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        // throw std::runtime_error(errorMessage);
        PostQuitMessage(0);
    }

    GLint uniform = glGetUniformLocation(_object, uniformName);

    if (uniform == -1)
    {
        errorMessage = "No se encontro el uniform especificado <" + strUniformName + ">";

        // Fun::ShowMessage(MSG_CAPTION_ERROR, errorMessage, Fun::TipoMensaje::Error);
        // throw std::runtime_error(errorMessage);
        // PostQuitMessage(0);
    }

    return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
    void Shader::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
    void Shader::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void Shader::setUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void Shader::setUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void Shader::setUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void Shader::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose) 
{
    assert(isInUse());
    glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* uniformName, const glm::vec3& v) 
{
    setUniform3v(uniformName, glm::value_ptr(v));
}

void Shader::setUniform(const GLchar* uniformName, const glm::vec4& v) 
{
    setUniform4v(uniformName, glm::value_ptr(v));
}