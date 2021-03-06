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

//****************************************************************
//
// Glide64 - Glide Plugin for Nintendo 64 emulators
// Project started on December 29th, 2001
//
// Authors:
// Dave2001, original author, founded the project in 2001, left it in 2002
// Gugaman, joined the project in 2002, left it in 2002
// Sergey 'Gonetz' Lipski, joined the project in 2002, main author since fall of 2002
// Hiroshi 'KoolSmoky' Morii, joined the project in 2007
//
//****************************************************************
//
// To modify Glide64:
// * Write your name and (optional)email, commented by your work, so I know who did it, and so that you can find which parts you modified when it comes time to send it to me.
// * Do NOT send me the whole project or file that you modified.  Take out your modified code sections, and tell me where to put them.  If people sent the whole thing, I would have many different versions, but no idea how to combine them all.
//
//****************************************************************

static inline void mirror16bS(uint8_t *tex, uint8_t *start, int width, int height, int mask, int line, int full, int count)
{
  uint16_t *v8;
  int v9;
  int v10;

  v8 = (uint16_t *)start;
  v9 = height;
  do
  {
    v10 = 0;
    do
    {
      if ( width & (v10 + width) )
      {
        *v8 = *(uint16_t *)(&tex[mask] - (mask & 2 * v10));
        ++v8;
      }
      else
      {
        *v8 = *(uint16_t *)&tex[mask & 2 * v10];
        ++v8;
      }
      ++v10;
    }
    while ( v10 != count );
    v8 = (uint16_t *)((int8_t*)v8 + line);
    tex += full;
    --v9;
  }
  while ( v9 );
}

static inline void wrap16bS(uint8_t *tex, uint8_t *start, int height, int mask, int line, int full, int count)
{
  uint32_t *v7;
  int v8;
  int v9;

  v7 = (uint32_t *)start;
  v8 = height;
  do
  {
    v9 = 0;
    do
    {
      *v7 = *(uint32_t *)&tex[4 * (mask & v9)];
      ++v7;
      ++v9;
    }
    while ( v9 != count );
    v7 = (uint32_t *)((int8_t*)v7 + line);
    tex += full;
    --v8;
  }
  while ( v8 );
}

static inline void clamp16bS(uint8_t *tex, uint8_t *constant, int height, int line, int full, int count)
{
  uint16_t *v6;
  uint16_t *v7;
  int v8;
  uint16_t v9;
  int v10;

  v6 = (uint16_t *)constant;
  v7 = (uint16_t *)tex;
  v8 = height;
  do
  {
    v9 = *v6;
    v10 = count;
    do
    {
      *v7 = v9;
      ++v7;
      --v10;
    }
    while ( v10 );
    v6 = (uint16_t *)((int8_t*)v6 + full);
    v7 = (uint16_t *)((int8_t*)v7 + line);
    --v8;
  }
  while ( v8 );
}

//****************************************************************
// 16-bit Horizontal Mirror
#include <stdint.h>
#include <string.h>
typedef uint32_t wxUint32;

void Mirror16bS (uint8_t * tex, wxUint32 mask, wxUint32 max_width, wxUint32 real_width, wxUint32 height)
{
  if (mask == 0) return;

  wxUint32 mask_width = (1 << mask);
  wxUint32 mask_mask = (mask_width-1) << 1;
  if (mask_width >= max_width) return;
  int count = max_width - mask_width;
  if (count <= 0) return;
  int line_full = real_width << 1;
  int line = line_full - (count << 1);
  if (line < 0) return;
  uint8_t *start = tex + (mask_width << 1);
  mirror16bS (tex, start, mask_width, height, mask_mask, line, line_full, count);
}

//****************************************************************
// 16-bit Horizontal Wrap (like mirror)

void Wrap16bS (uint8_t * tex, wxUint32 mask, wxUint32 max_width, wxUint32 real_width, wxUint32 height)
{
  if (mask == 0) return;

  wxUint32 mask_width = (1 << mask);
  wxUint32 mask_mask = (mask_width-1) >> 1;
  if (mask_width >= max_width) return;
  int count = (max_width - mask_width) >> 1;
  if (count <= 0) return;
  int line_full = real_width << 1;
  int line = line_full - (count << 2);
  if (line < 0) return;
  uint8_t * start = tex + (mask_width << 1);
  wrap16bS (tex, start, height, mask_mask, line, line_full, count);
}

//****************************************************************
// 16-bit Horizontal Clamp

void Clamp16bS (uint8_t * tex, wxUint32 width, wxUint32 clamp_to, wxUint32 real_width, wxUint32 real_height)
{
  if (real_width <= width) return;

  uint8_t * dest = tex + (width << 1);
  uint8_t * constant = dest-2;
  int count = clamp_to - width;

  int line_full = real_width << 1;
  int line = width << 1;

  clamp16bS (dest, constant, real_height, line, line_full, count);
}

//****************************************************************
// 16-bit Vertical Mirror

void Mirror16bT (uint8_t * tex, wxUint32 mask, wxUint32 max_height, wxUint32 real_width)
{
  if (mask == 0) return;

  wxUint32 mask_height = (1 << mask);
  wxUint32 mask_mask = mask_height-1;
  if (max_height <= mask_height) return;
  int line_full = real_width << 1;

  uint8_t * dst = tex + mask_height * line_full;

  for (wxUint32 y=mask_height; y<max_height; y++)
  {
     void *src = (y & mask_height) ? 
        (void*)(tex + (mask_mask - (y & mask_mask)) * line_full) : /* mirrored */
        (void*)(tex + (y & mask_mask) * line_full);                /* not mirrored */

     memcpy ((void*)dst, src, line_full);
    dst += line_full;
  }
}

//****************************************************************
// 16-bit Vertical Wrap

void Wrap16bT (uint8_t * tex, wxUint32 mask, wxUint32 max_height, wxUint32 real_width)
{
  if (mask == 0) return;

  wxUint32 mask_height = (1 << mask);
  wxUint32 mask_mask = mask_height-1;
  if (max_height <= mask_height) return;
  int line_full = real_width << 1;

  uint8_t * dst = tex + mask_height * line_full;

  for (wxUint32 y=mask_height; y<max_height; y++)
  {
    // not mirrored
    memcpy ((void*)dst, (void*)(tex + (y & mask_mask) * line_full), line_full);

    dst += line_full;
  }
}

//****************************************************************
// 16-bit Vertical Clamp

void Clamp16bT (uint8_t * tex, wxUint32 height, wxUint32 real_width, wxUint32 clamp_to)
{
  int line_full = real_width << 1;
  uint8_t * dst = tex + height * line_full;
  uint8_t * const_line = dst - line_full;

  for (wxUint32 y=height; y<clamp_to; y++)
  {
    memcpy ((void*)dst, (void*)const_line, line_full);
    dst += line_full;
  }
}
