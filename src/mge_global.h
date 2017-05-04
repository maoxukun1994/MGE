#ifndef MGE_GLOBAL_H
#define MGE_GLOBAL_H

/*
 * this file include gl funcionts loaders headers and glutil tools headers,as well as engine
 * related definitiones in global scope
*/


//std c/c++ headers
//stdc
#include <cmath>
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cassert>
//stdc++
#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>


//gl functions loader,need to be included BEFORE sfml or other gl tools
//glad headers
#include "glad/glad.h"


//gl util tools,need to be included AFTER gl functions loader headers
//sfml headers
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

//do not change this value
#define MGE_MAX_TUV_SET_PER_BATCH 4
//buffer size for shader error message
#define MGE_SHADER_INFO_BUFFERSIZE 1024
//the initial size of vector in mge_glprogrammanager,to avoid reallocate of vector
#define MGE_SHADER_CONTAINER_INITSIZE 64


//vertex attributes definitions
namespace MGE_CORE
{

//vertex attributes (used in shader layout location)
enum MGE_VATTRIBS
{
    MGE_VATTRIB_POSITION = 0,
    MGE_VATTRIB_NORMAL, //1
    MGE_VATTRIB_COLOR, //2
    MGE_VATTRIB_TUV_0, //3
    MGE_VATTRIB_TUV_1, //4
    MGE_VATTRIB_TUV_2, //5
    MGE_VATTRIB_TUV_3, //6

    MGE_VATTRIB_LAST //attribute last for control in loop etc.
};
//enum MGE_VATTRIBS

//texture filter type
enum TextureFilterType
{
    MIN_FILTER = 0,
    MAG_FILTER
};

//texture filter method
enum TextureFilterMethod
{
    NEAREST = 0,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR
};



//error code list
enum MGE_ERRORCODE
{
    NoError = 0,
    GLLoadError,
    GLUnInitialized,
    ShaderCompileVertex,
    ShaderCompileFragment,
    ShaderCompileCompute,
    ShaderLink,
    ShaderFileRead,
    TextureLoadFileRead,
};
//enum MGE_ERRORCODE

}
//in namespace MGE_CORE

#endif // MGE_GLOBAL_H
