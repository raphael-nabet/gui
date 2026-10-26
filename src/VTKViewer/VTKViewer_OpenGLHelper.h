// Copyright (C) 2007-2026  CEA, EDF, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef VTKVIEWER_OPENGLHELPER_H
#define VTKVIEWER_OPENGLHELPER_H

#include <vtkObject.h>
#include <vtkMatrix4x4.h>
#include "VTKViewer.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
typedef GLuint VTKViewer_GLhandle;
#else
#ifndef GL_ARB_shader_objects
typedef GLuint GLhandleARB;
#endif
typedef GLhandleARB VTKViewer_GLhandle;
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GL_ARB_shader_objects
typedef char GLcharARB;
#endif

#ifndef GL_VERTEX_PROGRAM_POINT_SIZE_ARB
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB  0x8642
#endif

#ifndef GL_VERTEX_SHADER_ARB
#define GL_VERTEX_SHADER_ARB              0x8B31
#endif

#ifndef GL_FRAGMENT_SHADER_ARB
#define GL_FRAGMENT_SHADER_ARB            0x8B30
#endif

#ifndef GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862
#endif

#ifndef GL_ARB_vertex_buffer_object
typedef ptrdiff_t GLsizeiptrARB;

#define GL_ARRAY_BUFFER_ARB               0x8892
#define GL_STATIC_DRAW_ARB                0x88E4
#endif

#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS                 0x8B81
#endif

#ifndef GL_CURRENT_PROGRAM
#define GL_CURRENT_PROGRAM                0x8B8D
#endif

#ifndef GL_DEBUG_TYPE_ERROR
#define GL_DEBUG_TYPE_ERROR               0x824C
#endif

#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS                    0x8B82
#endif

#ifndef GL_TEXTURE0
#define GL_TEXTURE0                       0x84C0
#endif

#ifndef GL_VALIDATE_STATUS
#define GL_VALIDATE_STATUS                0x8B83
#endif

#ifndef GL_NUM_EXTENSIONS
#define GL_NUM_EXTENSIONS                 0x821D
#endif

namespace GUI_OPENGL {
char* readFromFile( std::string fileName );
}

class VTKVIEWER_EXPORT VTKViewer_OpenGLHelper
{
public:
  VTKViewer_OpenGLHelper();
  ~VTKViewer_OpenGLHelper();

  void Init();

  bool IsInitialized() const { return mIsInitialized; }

public:
  bool CreateShaderProgram (const std::string& theFilePath,
#ifdef __APPLE__
                                            VTKViewer_GLhandle&       theProgram,
                                            VTKViewer_GLhandle&       theVertexShader,
                                            VTKViewer_GLhandle&       theFragmentShader
#else
                                                  GLhandleARB&       theProgram,
                                                  GLhandleARB&       theVertexShader,
                                                  GLhandleARB&       theFragmentShader
#endif
                                                  ) const;

  void DestroyShaderProgram (
#ifdef __APPLE__
                                                    VTKViewer_GLhandle theProgram,
                                                    VTKViewer_GLhandle theVertexShader,
                                                    VTKViewer_GLhandle theFragmentShader
#else
                                                    GLhandleARB&       theProgram,
                                                    GLhandleARB&       theVertexShader,
                                                    GLhandleARB&       theFragmentShader
#endif
                                                  ) const;

#ifdef VTK_OPENGL2
  void SetUniformMatrix (const GLint         theLocation,
                         const vtkMatrix4x4* theMatrix) const;
#endif  

protected:
#ifdef __APPLE__
  typedef void        (APIENTRYP PFNGLSHADERSOURCEARBPROC)             (VTKViewer_GLhandle shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
  typedef VTKViewer_GLhandle (APIENTRYP PFNGLCREATESHADEROBJECTARBPROC)       (GLenum shaderType);
#else
  typedef void        (APIENTRYP PFNGLSHADERSOURCEARBPROC)             (GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
  typedef GLhandleARB (APIENTRYP PFNGLCREATESHADEROBJECTARBPROC)       (GLenum shaderType);
#endif
  typedef void        (APIENTRYP PFNGLBINDBUFFERARBPROC)               (GLenum target, GLuint buffer);
  typedef void        (APIENTRYP PFNGLDELETEBUFFERSARBPROC)            (GLsizei n, const GLuint *buffers);
  typedef void        (APIENTRYP PFNGLGENBUFFERSARBPROC)               (GLsizei n, GLuint *buffers);
  typedef void        (APIENTRYP PFNGLBUFFERDATAARBPROC)               (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
#ifdef __APPLE__
  typedef void        (APIENTRYP PFNGLCOMPILESHADERARBPROC)            (VTKViewer_GLhandle shaderObj);
  typedef VTKViewer_GLhandle (APIENTRYP PFNGLCREATEPROGRAMOBJECTARBPROC)      (void);
  typedef void        (APIENTRYP PFNGLATTACHOBJECTARBPROC)             (VTKViewer_GLhandle containerObj, VTKViewer_GLhandle obj);
  typedef void        (APIENTRYP PFNGLLINKPROGRAMARBPROC)              (VTKViewer_GLhandle programObj);
  typedef void        (APIENTRYP PFNGLUSEPROGRAMOBJECTARBPROC)         (VTKViewer_GLhandle programObj);
  typedef GLint       (APIENTRYP PFNGLGETATTRIBLOCATIONARBPROC)        (VTKViewer_GLhandle programObj, const GLcharARB *name);
#else
  typedef void        (APIENTRYP PFNGLCOMPILESHADERARBPROC)            (GLhandleARB shaderObj);
  typedef GLhandleARB (APIENTRYP PFNGLCREATEPROGRAMOBJECTARBPROC)      (void);
  typedef void        (APIENTRYP PFNGLATTACHOBJECTARBPROC)             (GLhandleARB containerObj, GLhandleARB obj);
  typedef void        (APIENTRYP PFNGLLINKPROGRAMARBPROC)              (GLhandleARB programObj);
  typedef void        (APIENTRYP PFNGLUSEPROGRAMOBJECTARBPROC)         (GLhandleARB programObj);
  typedef GLint       (APIENTRYP PFNGLGETATTRIBLOCATIONARBPROC)        (GLhandleARB programObj, const GLcharARB *name);
#endif
  typedef void        (APIENTRYP PFNGLVERTEXATTRIBPOINTERARBPROC)      (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
  typedef void        (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYARBPROC)  (GLuint index);
  typedef void        (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);

#ifdef VTK_OPENGL2
  typedef void        (APIENTRYP PFNGLACTIVETEXTUREARBPROC)            (GLenum texture);
#ifdef __APPLE__
  typedef void        (APIENTRYP PFNGLDETACHOBJECTARBPROC)             (VTKViewer_GLhandle containerObj, VTKViewer_GLhandle obj);
  typedef void        (APIENTRYP PFNGLDELETEOBJECTARBPROC)             (VTKViewer_GLhandle obj);
  typedef void        (APIENTRYP PFNGLVALIDATEPROGRAMARBPROC)          (VTKViewer_GLhandle program);
  typedef GLint       (APIENTRYP PFNGLGETUNIFORMLOCATIONARBPROC)       (VTKViewer_GLhandle program, const GLcharARB *name );
#else
  typedef void        (APIENTRYP PFNGLDETACHOBJECTARBPROC)             (GLhandleARB containerObj, GLhandleARB obj);
  typedef void        (APIENTRYP PFNGLDELETEOBJECTARBPROC)             (GLhandleARB obj);
  typedef void        (APIENTRYP PFNGLVALIDATEPROGRAMARBPROC)          (GLhandleARB program);
  typedef GLint       (APIENTRYP PFNGLGETUNIFORMLOCATIONARBPROC)       (GLhandleARB program, const GLcharARB *name );
#endif
  typedef void        (APIENTRYP PFNGLGETSHADERIVARBPROC)              (GLuint shader, GLenum pname, GLint *params);
  typedef void        (APIENTRYP PFNGLGETPROGRAMIVARBPROC)             (GLenum target, GLenum pname, GLint *params);
  typedef void        (APIENTRYP PFNGLGETSHADERINFOLOGARBPROC)         (GLuint shader, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
  typedef void        (APIENTRYP PFNGLUNIFORMMATRIX4FVARBPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  typedef void        (APIENTRYP PFNGLGENVERTEXARRAYSARBPROC)          (GLsizei n, GLuint *arrays);
  typedef void        (APIENTRYP PFNGLBINDVERTEXARRAYARBPROC)          (GLuint array);
  typedef void        (APIENTRYP PFNGLUNIFORM1IARBPROC)                (GLint location, GLint v0);
  typedef const GLubyte *(APIENTRYP PFNGLGETSTRINGIPROC)               (GLenum name, GLuint index);
#endif

public:
  PFNGLSHADERSOURCEARBPROC             vglShaderSourceARB;
  PFNGLCREATESHADEROBJECTARBPROC       vglCreateShaderObjectARB;
  PFNGLCOMPILESHADERARBPROC            vglCompileShaderARB;
  PFNGLCREATEPROGRAMOBJECTARBPROC      vglCreateProgramObjectARB;
  PFNGLATTACHOBJECTARBPROC             vglAttachObjectARB;
  PFNGLLINKPROGRAMARBPROC              vglLinkProgramARB;
  PFNGLUSEPROGRAMOBJECTARBPROC         vglUseProgramObjectARB;

  PFNGLGENBUFFERSARBPROC               vglGenBuffersARB;
  PFNGLBINDBUFFERARBPROC               vglBindBufferARB;
  PFNGLBUFFERDATAARBPROC               vglBufferDataARB;
  PFNGLDELETEBUFFERSARBPROC            vglDeleteBuffersARB;
  PFNGLGETATTRIBLOCATIONARBPROC        vglGetAttribLocationARB;
  PFNGLVERTEXATTRIBPOINTERARBPROC      vglVertexAttribPointerARB;
  PFNGLENABLEVERTEXATTRIBARRAYARBPROC  vglEnableVertexAttribArrayARB;
  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC vglDisableVertexAttribArrayARB;

#ifdef VTK_OPENGL2
  PFNGLDETACHOBJECTARBPROC             vglDetachObjectARB;
  PFNGLDELETEOBJECTARBPROC             vglDeleteObjectARB;
  PFNGLVALIDATEPROGRAMARBPROC          vglValidateProgramARB;
  PFNGLGETSHADERIVARBPROC              vglGetShaderivARB;
  PFNGLGETPROGRAMIVARBPROC             vglGetProgramivARB;
  PFNGLGETSHADERINFOLOGARBPROC         vglGetShaderInfoLogARB;

  PFNGLUNIFORMMATRIX4FVARBPROC         vglUniformMatrix4fvARB;
  PFNGLGENVERTEXARRAYSARBPROC          vglGenVertexArraysARB;
  PFNGLBINDVERTEXARRAYARBPROC          vglBindVertexArrayARB;
  PFNGLUNIFORM1IARBPROC                vglUniform1iARB;
  PFNGLGETUNIFORMLOCATIONARBPROC       vglGetUniformLocationARB;
  PFNGLACTIVETEXTUREARBPROC            vglActiveTextureARB;  
  PFNGLGETSTRINGIPROC                  vglGetStringiARB;
#endif

protected:
  bool mIsInitialized;
};

#endif
