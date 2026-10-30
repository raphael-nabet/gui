// Copyright (C) 2007-2026  CEA, EDF, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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


#include "VTKViewer_OpenGLHelper.h"
#include <Basics_Utils.hxx>

#if defined(WIN32)
# include <wingdi.h>
#elif defined(__APPLE__)
# include <dlfcn.h>
#else
 # ifndef GLX_GLXEXT_LEGACY
 #  define GLX_GLXEXT_LEGACY
 # endif
 # include <GL/glx.h>
 # include <dlfcn.h>
 #endif

#if defined(WIN32)
#define GL_GetProcAddress( x )   wglGetProcAddress( (const LPCSTR)x )
#elif defined(__APPLE__)
#define GL_GetProcAddress( x )   dlsym( RTLD_DEFAULT, x )
#else
 #define GL_GetProcAddress( x )   glXGetProcAddressARB( (const GLubyte*)x )
#endif


// ============================================================================
// function : VTKViewer_OpenGLHelper
// purpose  :
// ============================================================================
VTKViewer_OpenGLHelper::VTKViewer_OpenGLHelper()
: vglShaderSourceARB             (NULL),
  vglCreateShaderObjectARB       (NULL),
  vglCompileShaderARB            (NULL),
  vglCreateProgramObjectARB      (NULL),
  vglAttachObjectARB             (NULL),
  vglLinkProgramARB              (NULL),
  vglUseProgramObjectARB         (NULL),
  vglGenBuffersARB               (NULL),
  vglBindBufferARB               (NULL),
  vglBufferDataARB               (NULL),
  vglDeleteBuffersARB            (NULL),
  vglGetAttribLocationARB        (NULL),
  vglVertexAttribPointerARB      (NULL),
  vglEnableVertexAttribArrayARB  (NULL),
  vglDisableVertexAttribArrayARB (NULL),
#ifdef VTK_OPENGL2
  vglDetachObjectARB             (NULL),
  vglDeleteObjectARB             (NULL),
  vglValidateProgramARB          (NULL),
  vglGetShaderivARB              (NULL),
  vglGetProgramivARB             (NULL),
  vglGetShaderInfoLogARB         (NULL),
  vglUniformMatrix4fvARB         (NULL),
  vglGenVertexArraysARB          (NULL),
  vglBindVertexArrayARB          (NULL),
  vglUniform1iARB                (NULL),
  vglGetUniformLocationARB       (NULL),
  vglActiveTextureARB            (NULL),
  vglGetStringiARB               (NULL),
#endif
  mIsInitialized                 (false)
{
  Init();
}

// ============================================================================
// function : ~VTKViewer_OpenGLHelper
// purpose  :
// ============================================================================
VTKViewer_OpenGLHelper::~VTKViewer_OpenGLHelper()
{
  //
}

// ============================================================================
// function : Init
// purpose  :
// ============================================================================
void VTKViewer_OpenGLHelper::Init()
{
  if (mIsInitialized)
    return;

  // macOS core profile: the GL_ARB_shader_objects entry points are not
  // available, the OpenGL 2.0 core ones are used instead
#ifdef __APPLE__
  vglShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)GL_GetProcAddress( "glShaderSource" );
#else
  vglShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)GL_GetProcAddress( "glShaderSourceARB" );
#endif
  if( !vglShaderSourceARB )
    return;

#ifdef __APPLE__
  vglCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)GL_GetProcAddress( "glCreateShader" );
#else
  vglCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)GL_GetProcAddress( "glCreateShaderObjectARB" );
#endif
  if( !vglCreateShaderObjectARB )
    return;

#ifdef __APPLE__
  vglCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)GL_GetProcAddress( "glCompileShader" );
#else
  vglCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)GL_GetProcAddress( "glCompileShaderARB" );
#endif
  if( !vglCompileShaderARB )
    return;

#ifdef __APPLE__
  vglCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)GL_GetProcAddress( "glCreateProgram" );
#else
  vglCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)GL_GetProcAddress( "glCreateProgramObjectARB" );
#endif
  if( !vglCreateProgramObjectARB )
    return;

#ifdef __APPLE__
  vglAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)GL_GetProcAddress( "glAttachShader" );
#else
  vglAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)GL_GetProcAddress( "glAttachObjectARB" );
#endif
  if( !vglAttachObjectARB )
    return;

#ifdef __APPLE__
  vglLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)GL_GetProcAddress( "glLinkProgram" );
#else
  vglLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)GL_GetProcAddress( "glLinkProgramARB" );
#endif
  if( !vglLinkProgramARB )
    return;

#ifdef __APPLE__
  vglUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)GL_GetProcAddress( "glUseProgram" );
#else
  vglUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)GL_GetProcAddress( "glUseProgramObjectARB" );
#endif
  if( !vglUseProgramObjectARB )
    return;

  vglGenBuffersARB = (PFNGLGENBUFFERSARBPROC)GL_GetProcAddress( "glGenBuffersARB" );
  if( !vglGenBuffersARB )
    return;

  vglBindBufferARB = (PFNGLBINDBUFFERARBPROC)GL_GetProcAddress( "glBindBufferARB" );
  if( !vglBindBufferARB )
    return;

  vglBufferDataARB = (PFNGLBUFFERDATAARBPROC)GL_GetProcAddress( "glBufferDataARB" );
  if( !vglBufferDataARB )
    return;

  vglDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)GL_GetProcAddress( "glDeleteBuffersARB" );
  if( !vglDeleteBuffersARB )
    return;

  vglGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)GL_GetProcAddress( "glGetAttribLocation" );
  if( !vglGetAttribLocationARB )
    return;

  vglVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC)GL_GetProcAddress( "glVertexAttribPointer" );
  if( !vglVertexAttribPointerARB )
    return;

  vglEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)GL_GetProcAddress( "glEnableVertexAttribArray" );
  if(!vglEnableVertexAttribArrayARB)
    return;

  vglDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)GL_GetProcAddress( "glDisableVertexAttribArray" );
  if(!vglDisableVertexAttribArrayARB)
    return;

#ifdef VTK_OPENGL2
#ifdef __APPLE__
  vglDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)GL_GetProcAddress( "glDetachShader" );
#else
  vglDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)GL_GetProcAddress( "glDetachObjectARB" );
#endif
  if( !vglDetachObjectARB )
    return;

#ifdef __APPLE__
  vglDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)GL_GetProcAddress( "glDeleteShader" );
#else
  vglDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)GL_GetProcAddress( "glDeleteObjectARB" );
#endif
  if( !vglDeleteObjectARB )
    return;

#ifdef __APPLE__
  vglValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)GL_GetProcAddress( "glValidateProgram" );
#else
  vglValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)GL_GetProcAddress( "glValidateProgramARB" );
#endif
  if ( !vglValidateProgramARB )
    return;

#ifdef __APPLE__
  vglGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)GL_GetProcAddress( "glGetUniformLocation" );
#else
  vglGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)GL_GetProcAddress( "glGetUniformLocationARB" );
#endif
  if( !vglGetUniformLocationARB )
    return;

  vglGetShaderivARB = (PFNGLGETSHADERIVARBPROC)GL_GetProcAddress( "glGetShaderiv" );
  if( !vglGetShaderivARB )
    return;

  vglActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)GL_GetProcAddress( "glActiveTexture" );
  if (!vglActiveTextureARB)
    return;

  vglGetStringiARB = (PFNGLGETSTRINGIPROC)GL_GetProcAddress("glGetStringi");
  if (!vglGetStringiARB)
    return;

  vglGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)GL_GetProcAddress( "glGetProgramiv" );
  if( !vglGetProgramivARB )
    return;

  vglGetShaderInfoLogARB = (PFNGLGETSHADERINFOLOGARBPROC)GL_GetProcAddress( "glGetShaderInfoLog" );
  if( !vglGetShaderInfoLogARB )
    return;

  vglUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)GL_GetProcAddress( "glUniformMatrix4fv" );
  if( !vglUniformMatrix4fvARB )
    return;

  vglGenVertexArraysARB = (PFNGLGENVERTEXARRAYSARBPROC)GL_GetProcAddress( "glGenVertexArrays" );
  if( !vglGenVertexArraysARB )
    return;

  vglBindVertexArrayARB = (PFNGLBINDVERTEXARRAYARBPROC)GL_GetProcAddress( "glBindVertexArray" );
  if( !vglBindVertexArrayARB )
    return;

  vglUniform1iARB = (PFNGLUNIFORM1IARBPROC)GL_GetProcAddress( "glUniform1i" );
  if( !vglUniform1iARB )
    return;
#endif

  mIsInitialized = true;
  return;
}

namespace GUI_OPENGL
{
	char* readFromFile( std::string fileName )
	{
#if defined(WIN32) && defined(UNICODE)
	  const wchar_t* wFileName = Kernel_Utils::utf8_decode( fileName.c_str() );
	  FILE* file = _wfopen( wFileName, L"r" );
#else
	  FILE* file = fopen( fileName.c_str(), "r" );
#endif
	  char* content = NULL;
	  int count = 0;

	  if( file != NULL )
	  {
		fseek( file, 0, SEEK_END );
		count = ftell( file );
		rewind( file );

		if( count > 0 )
		{
		  content = ( char* )malloc( sizeof( char ) * ( count + 1 ) );
		  count = (int)fread( content, sizeof( char ), count, file ); //!< TODO: conversion from size_t to int
		  content[ count ] = '\0';
		}
		fclose( file );
	  }

	  return content;
	}
}
// ============================================================================
// function : CreateShaderProgram
// purpose  :
// ============================================================================
#ifdef __APPLE__
bool VTKViewer_OpenGLHelper::CreateShaderProgram (const std::string&  theFilePath,
                                                  VTKViewer_GLhandle& theProgram,
                                                  VTKViewer_GLhandle& theVertexShader,
                                                  VTKViewer_GLhandle& theFragmentShader) const
#else
bool VTKViewer_OpenGLHelper::CreateShaderProgram (const std::string& theFilePath,
                                                  GLhandleARB&       theProgram,
                                                  GLhandleARB&       theVertexShader,
                                                  GLhandleARB&       theFragmentShader) const
#endif
{
#ifdef VTK_OPENGL2
  // Create program.
  theProgram = vglCreateProgramObjectARB();
  if (theProgram == 0)
  {
    std::cerr << "Can't create opengl program." << std::endl;
    return false;
  }

  std::string fileName;
  char*       shaderContent;
  GLint       linked, compileStatus, validateStatus;

  // Create vertex shader.
  fileName = theFilePath + ".vs.glsl";

  shaderContent = GUI_OPENGL::readFromFile (fileName);

  theVertexShader = vglCreateShaderObjectARB (GL_VERTEX_SHADER_ARB);
  vglShaderSourceARB (theVertexShader, 1, (const GLcharARB**)&shaderContent, NULL);
  vglCompileShaderARB (theVertexShader);

  free( shaderContent );

  vglGetShaderivARB (theVertexShader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus != GL_TRUE)
  {
    GLint size;
    GLcharARB info[1024];

    vglGetShaderInfoLogARB (theVertexShader, 1024, &size, info);
    std::cerr << "Can't compile vertex shader." << std::endl;
    std::cerr << info << std::endl;

    return false;
  }

  // Create fragment shader.
  fileName = theFilePath + ".fs.glsl";

  shaderContent = GUI_OPENGL::readFromFile (fileName);

  theFragmentShader = vglCreateShaderObjectARB (GL_FRAGMENT_SHADER_ARB);
  vglShaderSourceARB (theFragmentShader, 1, (const GLcharARB**)&shaderContent, NULL);
  vglCompileShaderARB (theFragmentShader);

  free (shaderContent);

  vglGetShaderivARB (theFragmentShader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus != GL_TRUE)
  {
    GLint size;
    GLcharARB info[1024];

    vglGetShaderInfoLogARB (theVertexShader, 1024, &size, info);
    std::cerr << "Can't compile fragment shader." << std::endl;
    std::cerr << info << std::endl;
    return false;
  }

  // Attach shaders.
  vglAttachObjectARB (theProgram, theVertexShader);
  vglAttachObjectARB (theProgram, theFragmentShader);
  vglLinkProgramARB  (theProgram);

  vglGetProgramivARB (theProgram, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    std::cerr << "Can't link program." << std::endl;
    return false;
  }

  vglValidateProgramARB (theProgram);
  vglGetProgramivARB (theProgram, GL_VALIDATE_STATUS, &validateStatus);

  if (validateStatus != GL_TRUE)
  {
    std::cerr << "Shader program is not validate." << std::endl;
    return false;
  }

  return true;
#else
  return false;
#endif
}

// ============================================================================
// function : DestroyShaderProgram
// purpose  :
// ============================================================================
#ifdef __APPLE__
void VTKViewer_OpenGLHelper::DestroyShaderProgram (VTKViewer_GLhandle theProgram,
                                                   VTKViewer_GLhandle theVertexShader,
                                                   VTKViewer_GLhandle theFragmentShader) const
#else
void VTKViewer_OpenGLHelper::DestroyShaderProgram (GLhandleARB theProgram,
                                                   GLhandleARB theVertexShader,
                                                   GLhandleARB theFragmentShader) const
#endif
{
#ifdef VTK_OPENGL2
  vglDetachObjectARB (theProgram, theVertexShader);
  vglDetachObjectARB (theProgram, theFragmentShader);

  vglDeleteObjectARB (theVertexShader);
  vglDeleteObjectARB (theFragmentShader);
#endif
}

// ============================================================================
// function : SetUniformMatrix
// purpose  :
// ============================================================================
#ifdef VTK_OPENGL2
void VTKViewer_OpenGLHelper::SetUniformMatrix (const GLint         theLocation,
                                               const vtkMatrix4x4* theMatrix) const
{
  float data[16];
  for (int i = 0; i < 16; ++i)
  {
    data[i] = theMatrix->Element[i / 4][i % 4];
  }

  this->vglUniformMatrix4fvARB (theLocation, 1, GL_FALSE, data);
}
#endif
