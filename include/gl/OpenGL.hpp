/*
 * =====================================================================================
 *
 *       Filename:  OpenGL.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 04:18:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef OPENGL_HPP_
#define OPENGL_HPP_

#ifdef __APPLE__
	#include <OpenGL/glew.h>
#else
	#ifdef __MINGW32__
		#include <GL/glew.h>
	#else
		#include <GL/gl.h>
	#endif
#endif

#endif // OPENGL_HPP_