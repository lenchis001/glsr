#include "srglext.h"

#include "Services/IFacade.h"

// VBO
#define srglGenBuffers \
    glsr::Services::IFacade::getInstance().getBufferService()->genBuffers
#define srglDeleteBuffers \
    glsr::Services::IFacade::getInstance().getBufferService()->deleteBuffers
#define srglBindBuffer \
    glsr::Services::IFacade::getInstance().getBufferService()->bindBuffer
#define srglBufferData \
    glsr::Services::IFacade::getInstance().getBufferService()->bufferData
#define srglBufferSubData \
    glsr::Services::IFacade::getInstance().getBufferService()->bufferSubData
#define srglGetBufferedData \
    glsr::Services::IFacade::getInstance().getBufferService()->getBufferedData
#define srglGenVertexArrays \
    glsr::Services::IFacade::getInstance().getBufferService()->genVertexArrays
#define srglBindVertexArray \
    glsr::Services::IFacade::getInstance().getBufferService()->bindVertexArray
#define srglVertexAttribPointer \
    glsr::Services::IFacade::getInstance().getBufferService()->vertexAttribPointer
#define srglEnableVertexAttribArray \
    glsr::Services::IFacade::getInstance().getBufferService()->enableVertexAttribArray
#define srglDeleteVertexArrays \
    glsr::Services::IFacade::getInstance().getBufferService()->deleteVertexArrays
#ifdef SRGL_MOCK_GL
    #define glGenBuffers srglGenBuffers
    #define glDeleteBuffers srglDeleteBuffers
    #define glBindBuffer srglBindBuffer
    #define glBufferData srglBufferData
    #define glBufferSubData srglBufferSubData
    #define glGenVertexArrays srglGenVertexArrays
    #define glBindVertexArray srglBindVertexArray
    #define glVertexAttribPointer srglVertexAttribPointer
    #define glEnableVertexAttribArray srglEnableVertexAttribArray
    #define glDeleteVertexArrays srglDeleteVertexArrays
#endif

// Shader
#define srglCreateProgram \
    glsr::Services::IFacade::getInstance().getShaderService()->createProgram
#define srglCreateShader \
    glsr::Services::IFacade::getInstance().getShaderService()->createShader
#define srglShaderSource \
    glsr::Services::IFacade::getInstance().getShaderService()->shaderSource
#define srglCompileShader \
    glsr::Services::IFacade::getInstance().getShaderService()->compileShader
#define srglGetShaderiv \
    glsr::Services::IFacade::getInstance().getShaderService()->getShaderiv
#define srglGetShaderInfoLog \
    glsr::Services::IFacade::getInstance().getShaderService()->getShaderInfoLog
#define srglDeleteShader \
    glsr::Services::IFacade::getInstance().getShaderService()->deleteShader
#define srglAttachShader \
    glsr::Services::IFacade::getInstance().getShaderService()->attachShader
#define srglLinkProgram \
    glsr::Services::IFacade::getInstance().getShaderService()->linkProgram
#define srglGetProgramiv \
    glsr::Services::IFacade::getInstance().getShaderService()->getProgramIv
#define srglGetProgramInfoLog \
    glsr::Services::IFacade::getInstance().getShaderService()->getProgramInfoLog
#define srglUseProgram \
    glsr::Services::IFacade::getInstance().getShaderService()->useProgram
#define srglGetUniformLocation \
    glsr::Services::IFacade::getInstance().getShaderService()->getUniformLocation
#define srglUniform1i \
    glsr::Services::IFacade::getInstance().getShaderService()->uniform1i
#define srglUniform3fv \
    glsr::Services::IFacade::getInstance().getShaderService()->uniform3fv
#define srglUniformMatrix4fv \
    glsr::Services::IFacade::getInstance().getShaderService()->uniformMatrix4fv
#define srglDeleteProgram \
    glsr::Services::IFacade::getInstance().getShaderService()->deleteProgram
#ifdef SRGL_MOCK_GL
    #define glCreateProgram srglCreateProgram
    #define glCreateShader srglCreateShader
    #define glShaderSource srglShaderSource
    #define glCompileShader srglCompileShader
    #define glGetShaderiv srglGetShaderiv
    #define glGetShaderInfoLog srglGetShaderInfoLog
    #define glDeleteShader srglDeleteShader
    #define glAttachShader srglAttachShader
    #define glLinkProgram srglLinkProgram
    #define glGetProgramiv srglGetProgramiv
    #define glGetProgramInfoLog srglGetProgramInfoLog
    #define glUseProgram srglUseProgram
    #define glGetUniformLocation srglGetUniformLocation
    #define glUniform1i srglUniform1i
    #define glUniform3fv srglUniform3fv
    #define glUniformMatrix4fv srglUniformMatrix4fv
    #define glDeleteProgram srglDeleteProgram
#endif

// Drawing
#define srglDrawArrays \
    glsr::Services::IFacade::getInstance().getDrawingService()->drawArrays
#define srglDrawElements \
    glsr::Services::IFacade::getInstance().getDrawingService()->drawElements
#define srglViewport \
    glsr::Services::IFacade::getInstance().getDrawingService()->viewport
#define srglReadPixels \
    glsr::Services::IFacade::getInstance().getDrawingService()->readPixels
#define srglClearColor \
    glsr::Services::IFacade::getInstance().getDrawingService()->clearColor
#define srglClear \
    glsr::Services::IFacade::getInstance().getDrawingService()->clear
#ifdef SRGL_MOCK_GL
    #define glDrawArrays srglDrawArrays
    #define glDrawElements srglDrawElements
    #define glViewport srglViewport
    #define glReadPixels srglReadPixels
    #define glClearColor srglClearColor
    #define glClear srglClear
#endif

// Texture
#define srglGenTextures \
    glsr::Services::IFacade::getInstance().getTextureService()->genTextures
#define srglActiveTexture \
    glsr::Services::IFacade::getInstance().getTextureService()->activeTexture
#define srglBindTexture \
    glsr::Services::IFacade::getInstance().getTextureService()->bindTexture
#define srglTexParameteri \
    glsr::Services::IFacade::getInstance().getTextureService()->texParameteri
#define srglTexImage2D \
    glsr::Services::IFacade::getInstance().getTextureService()->texImage2D
#define srglTexSubImage2D \
    glsr::Services::IFacade::getInstance().getTextureService()->texSubImage2D
#define srglDeleteTextures \
    glsr::Services::IFacade::getInstance().getTextureService()->deleteTextures
#ifdef SRGL_MOCK_GL
    #define glGenTextures srglGenTextures
    #define glActiveTexture srglActiveTexture
    #define glBindTexture srglBindTexture
    #define glTexParameteri srglTexParameteri
    #define glTexImage2D srglTexImage2D
    #define glTexSubImage2D srglTexSubImage2D
    #define glDeleteTextures srglDeleteTextures
#endif

// Configuration
#define srglEnable \
    glsr::Services::IFacade::getInstance().getConfigurationService()->enable
#define srglDisable \
    glsr::Services::IFacade::getInstance().getConfigurationService()->disable
#define srglGetBooleanv \
    glsr::Services::IFacade::getInstance().getConfigurationService()->getBooleanv
#define srglBlendFunc \
    glsr::Services::IFacade::getInstance().getConfigurationService()->blendFunc
#define srglCullFace \
    glsr::Services::IFacade::getInstance().getConfigurationService()->cullFace
#ifdef SRGL_MOCK_GL
    #define glEnable srglEnable
    #define glDisable srglDisable
    #define glGetBooleanv srglGetBooleanv
    #define glBlendFunc srglBlendFunc
    #define glCullFace srglCullFace
#endif
