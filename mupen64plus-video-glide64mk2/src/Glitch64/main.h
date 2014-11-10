/*
* Glide64 - Glide video plugin for Nintendo 64 emulators.
* Copyright (c) 2002  Dave2001
* Copyright (c) 2003-2009  Sergey 'Gonetz' Lipski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MAIN_H
#define MAIN_H

#include <m64p_types.h>

#define LOG(...) WriteLog(M64MSG_VERBOSE, __VA_ARGS__)
#define LOGINFO(...) WriteLog(M64MSG_VERBOSE, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
void WriteLog(m64p_msg_level level, const char *msg, ...);
#ifdef __cplusplus
}
#endif

#ifdef VPDEBUG
#include "glitchdebug.h"
#endif

#define zscale 1.0f

typedef struct _wrapper_config
{
  int res;
  int fbo;
  int anisofilter;
  int vram_size;
} wrapper_config;
extern wrapper_config config;


// VP added this utility function
// returns the bytes per pixel of a given GR texture format
int grTexFormatSize(int fmt);

extern int packed_pixels_support;
extern int ati_sucks;
extern float largest_supported_anisotropy;

extern int default_texture; // the infamous "32*1024*1024" is now configurable
extern int depth_texture;
void set_depth_shader();
void set_bw_shader();
extern float invtex[2];
extern int buffer_cleared; // mark that the buffer has been cleared, used to check if we need to reload the texture buffer content

#ifndef HAVE_WGL
#include <stdio.h>
//#define printf(...)
#define GL_GLEXT_PROTOTYPES
#ifdef USE_GLES
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#endif // HAVE_WINGL
#include "glide.h"

void display_warning(const unsigned char *text, ...);
void display_warning(const char *text, ...);
void init_geometry();
void init_textures();
void init_combiner();
void free_textures();
void updateCombiner(int i);
void updateCombinera(int i);
void remove_tex(unsigned int idmin, unsigned int idmax);
void add_tex(unsigned int id);

void check_compile(GLuint shader);
void check_link(GLuint program);
void vbo_enable();
void vbo_disable();

//Vertex Attribute Locations
#define POSITION_ATTR 0
#define COLOUR_ATTR 1
#define TEXCOORD_0_ATTR 2
#define TEXCOORD_1_ATTR 3
#define FOG_ATTR 4

extern int w_buffer_mode;
extern int nbTextureUnits;
extern int width, height, widtho, heighto;
extern int tex0_width, tex0_height, tex1_width, tex1_height;
extern int tex_exactWidth[2], tex_exactHeight[2];
extern int tex0_filter, tex1_filter;
extern float texture_env_color[4];
extern int fog_enabled;
extern float lambda;
extern int need_lambda[2];
extern float lambda_color[2][4];
extern int inverted_culling;
extern int culling_mode;
extern int render_to_texture;
extern int lfb_color_fmt;
extern int need_to_compile;
extern int blackandwhite0;
extern int blackandwhite1;
extern int TMU_SIZE;

extern int blend_func_separate_support;
extern int fog_coord_support;
//extern int pbuffer_support;
extern int glsl_support;
extern unsigned int pBufferAddress;
extern int viewport_width, viewport_height, viewport_offset, nvidia_viewport_hack;
extern int UMAmode;

void grChromaRangeExt(GrColor_t color0, GrColor_t color1, FxU32 mode);
void grChromaRangeModeExt(GrChromakeyMode_t mode);
void grTexChromaRangeExt(GrChipID_t tmu, GrColor_t color0, GrColor_t color1, GrTexChromakeyMode_t mode);
void grTexChromaModeExt(GrChipID_t tmu, GrChromakeyMode_t mode);
void updateTexture();
void reloadTexture();
void free_combiners();
void compile_shader();
void set_lambda();
void set_copy_shader();
void disable_textureSizes();

// config functions

//FX_ENTRY void FX_CALL grConfigWrapperExt(HINSTANCE instance, HWND hwnd);
FX_ENTRY void FX_CALL grConfigWrapperExt(FxI32, FxI32, FxBool, FxBool);
FX_ENTRY GrScreenResolution_t FX_CALL grWrapperFullScreenResolutionExt(FxU32*, FxU32*);
FX_ENTRY char ** FX_CALL grQueryResolutionsExt(FxI32*);
FX_ENTRY FxBool FX_CALL grKeyPressedExt(FxU32 key);
FX_ENTRY void FX_CALL grGetGammaTableExt(FxU32, FxU32*, FxU32*, FxU32*);

#ifndef GrPixelFormat_t
#define GrPixelFormat_t int
#endif

FX_ENTRY GrContext_t FX_CALL
grSstWinOpenExt(
                HWND                 hWnd,
                GrScreenResolution_t screen_resolution,
                GrScreenRefresh_t    refresh_rate,
                GrColorFormat_t      color_format,
                GrOriginLocation_t   origin_location,
                GrPixelFormat_t      pixelformat,
                int                  nColBuffers,
                int                  nAuxBuffers);

int getFullScreenWidth();
int getFullScreenHeight();

void vbo_init();
void vbo_free();
void vbo_bind();
void vbo_unbind();
void vbo_buffer_data(void *data, size_t data_sizeof);

// ZIGGY framebuffer copy extension
// allow to copy the depth or color buffer from back/front to front/back
#define GR_FBCOPY_MODE_DEPTH 0
#define GR_FBCOPY_MODE_COLOR 1
#define GR_FBCOPY_BUFFER_BACK 0
#define GR_FBCOPY_BUFFER_FRONT 1
FX_ENTRY void FX_CALL grFramebufferCopyExt(int x, int y, int w, int h,
                                           int buffer_from, int buffer_to, int mode);


// COMBINE extension

typedef FxU32 GrCCUColor_t;
typedef FxU32 GrACUColor_t;
typedef FxU32 GrTCCUColor_t;
typedef FxU32 GrTACUColor_t;

typedef FxU32 GrCombineMode_t;
#define GR_FUNC_MODE_ZERO                 0x00
#define GR_FUNC_MODE_X                    0x01
#define GR_FUNC_MODE_ONE_MINUS_X          0x02
#define GR_FUNC_MODE_NEGATIVE_X           0x03
#define GR_FUNC_MODE_X_MINUS_HALF         0x04

#define GR_CMBX_ZERO                      0x00
#define GR_CMBX_TEXTURE_ALPHA             0x01
#define GR_CMBX_ALOCAL                    0x02
#define GR_CMBX_AOTHER                    0x03
#define GR_CMBX_B                         0x04
#define GR_CMBX_CONSTANT_ALPHA            0x05
#define GR_CMBX_CONSTANT_COLOR            0x06
#define GR_CMBX_DETAIL_FACTOR             0x07
#define GR_CMBX_ITALPHA                   0x08
#define GR_CMBX_ITRGB                     0x09
#define GR_CMBX_LOCAL_TEXTURE_ALPHA       0x0a
#define GR_CMBX_LOCAL_TEXTURE_RGB         0x0b
#define GR_CMBX_LOD_FRAC                  0x0c
#define GR_CMBX_OTHER_TEXTURE_ALPHA       0x0d
#define GR_CMBX_OTHER_TEXTURE_RGB         0x0e
#define GR_CMBX_TEXTURE_RGB               0x0f
#define GR_CMBX_TMU_CALPHA                0x10
#define GR_CMBX_TMU_CCOLOR                0x11


FX_ENTRY void FX_CALL
grColorCombineExt(GrCCUColor_t a, GrCombineMode_t a_mode,
				  GrCCUColor_t b, GrCombineMode_t b_mode,
                  GrCCUColor_t c, FxBool c_invert,
				  GrCCUColor_t d, FxBool d_invert,
				  FxU32 shift, FxBool invert);

FX_ENTRY void FX_CALL
grAlphaCombineExt(GrACUColor_t a, GrCombineMode_t a_mode,
				  GrACUColor_t b, GrCombineMode_t b_mode,
				  GrACUColor_t c, FxBool c_invert,
				  GrACUColor_t d, FxBool d_invert,
				  FxU32 shift, FxBool invert);

FX_ENTRY void FX_CALL
grTexColorCombineExt(GrChipID_t       tmu,
                     GrTCCUColor_t a, GrCombineMode_t a_mode,
                     GrTCCUColor_t b, GrCombineMode_t b_mode,
                     GrTCCUColor_t c, FxBool c_invert,
                     GrTCCUColor_t d, FxBool d_invert,
                     FxU32 shift, FxBool invert);

FX_ENTRY void FX_CALL
grTexAlphaCombineExt(GrChipID_t       tmu,
                     GrTACUColor_t a, GrCombineMode_t a_mode,
                     GrTACUColor_t b, GrCombineMode_t b_mode,
                     GrTACUColor_t c, FxBool c_invert,
                     GrTACUColor_t d, FxBool d_invert,
                     FxU32 shift, FxBool invert);

FX_ENTRY void FX_CALL
grTextureAuxBufferExt( GrChipID_t tmu,
                      FxU32      startAddress,
                      GrLOD_t    thisLOD,
                      GrLOD_t    largeLOD,
                      GrAspectRatio_t aspectRatio,
                      GrTextureFormat_t format,
                      FxU32      odd_even_mask );

FX_ENTRY void FX_CALL
grConstantColorValueExt(GrChipID_t    tmu,
                        GrColor_t     value);

int isExtensionSupported(const char *extension);

#ifdef USE_GLES
#define CHECK_FRAMEBUFFER_STATUS() \
{\
 GLenum status; \
 status = glCheckFramebufferStatus(GL_FRAMEBUFFER); \
 /*display_warning("%x\n", status);*/\
 switch(status) { \
 case GL_FRAMEBUFFER_COMPLETE: \
   /*display_warning("framebuffer complete!\n");*/\
   break; \
 case GL_FRAMEBUFFER_UNSUPPORTED: \
   display_warning("framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");\
    /* you gotta choose different formats */ \
   /*assert(0);*/ \
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: \
   display_warning("framebuffer INCOMPLETE_ATTACHMENT\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: \
   display_warning("framebuffer FRAMEBUFFER_MISSING_ATTACHMENT\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: \
   display_warning("framebuffer FRAMEBUFFER_DIMENSIONS\n");\
   break; \
 default: \
   break; \
   /* programming error; will fail on all hardware */ \
   /*assert(0);*/ \
 }\
}
#else
#define CHECK_FRAMEBUFFER_STATUS() \
{\
 GLenum status; \
 status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); \
 /*display_warning("%x\n", status);*/\
 switch(status) { \
 case GL_FRAMEBUFFER_COMPLETE_EXT: \
   /*display_warning("framebuffer complete!\n");*/\
   break; \
 case GL_FRAMEBUFFER_UNSUPPORTED_EXT: \
   display_warning("framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");\
    /* you gotta choose different formats */ \
   /*assert(0);*/ \
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: \
   display_warning("framebuffer INCOMPLETE_ATTACHMENT\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: \
   display_warning("framebuffer FRAMEBUFFER_MISSING_ATTACHMENT\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: \
   display_warning("framebuffer FRAMEBUFFER_DIMENSIONS\n");\
   break; \
 /*case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT: \
   display_warning("framebuffer INCOMPLETE_DUPLICATE_ATTACHMENT\n");\
   break;*/ \
 case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: \
   display_warning("framebuffer INCOMPLETE_FORMATS\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: \
   display_warning("framebuffer INCOMPLETE_DRAW_BUFFER\n");\
   break; \
 case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: \
   display_warning("framebuffer INCOMPLETE_READ_BUFFER\n");\
   break; \
 case GL_FRAMEBUFFER_BINDING_EXT: \
   display_warning("framebuffer BINDING_EXT\n");\
   break; \
 default: \
   break; \
   /* programming error; will fail on all hardware */ \
   /*assert(0);*/ \
 }\
}
#endif

#ifdef VPDEBUG
#define LOGGING
#endif

#ifdef LOGGING
void OPEN_LOG();
void CLOSE_LOG();
//void LOG(const char *text, ...);
#else // LOGGING
#define OPEN_LOG()
#define CLOSE_LOG()
//#define LOG
#endif // LOGGING

#endif
