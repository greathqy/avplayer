/*
    Copyright (C) 2012  microcai <microcai@fedoraproject.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#define UINT64_C(c)     c ## ULL

#include <avplay.h>
#include <SDL/SDL.h>

#include "sdl_render.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXPORT_API int sdl_init_video(void *ctx, int w, int h, int pix_fmt)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = NULL;
	vo->video_dev = (void*)(sdl = new sdl_render);
	return sdl->init_render(vo->user_data, w, h, pix_fmt) ? 0 : -1;
}

EXPORT_API int sdl_render_one_frame(void *ctx, AVFrame* data, int pix_fmt, double pts)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = (sdl_render*)vo->video_dev;
	return sdl->render_one_frame(data, pix_fmt) ? 0 : -1;
}

EXPORT_API void sdl_re_size(void *ctx, int width, int height)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = (sdl_render*)vo->video_dev;
	sdl->re_size(width, height);
}

EXPORT_API void sdl_aspect_ratio(void *ctx, int srcw, int srch, int enable_aspect)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = (sdl_render*)vo->video_dev;
	sdl->aspect_ratio(srcw, srch, enable_aspect);
}

EXPORT_API int sdl_use_overlay(void *ctx)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = (sdl_render*)vo->video_dev;
	return sdl->use_overlay() ? 0 : -1;
}

EXPORT_API void sdl_destory_render(void *ctx)
{
	vo_context *vo = (vo_context*)ctx;
	sdl_render *sdl = (sdl_render*)vo->video_dev;
	if (sdl)
	{
		sdl->destory_render();
		delete sdl;
		vo->video_dev = NULL;
	}
}

#ifdef  __cplusplus
}
#endif


bool sdl_render::init_render(void* ctx, int w, int h, int pix_fmt)
{
	sfc = SDL_SetVideoMode(w,h,32,0);
	SDL_LockSurface(sfc);
	this->m_yuv = SDL_CreateYUVOverlay(w,h,SDL_IYUV_OVERLAY,sfc);
	SDL_UnlockSurface(sfc);
	logger("%s is called %p \n",__func__, sfc);
	return m_yuv;
}

bool sdl_render::render_one_frame(AVFrame* data, int pix_fmt)
{
	SDL_Rect rect;

	uint8_t** px = m_yuv->pixels;

	SDL_LockYUVOverlay(m_yuv);

	m_yuv->pixels = data->data;
	//memcpy(m_yuv->pixels[0],data->data[0],data->linesize[0]*m_yuv->h);
// // 	memcpy(m_yuv->pixels[1],data->data[1],data->linesize[1]*m_yuv->h);
	//memcpy(m_yuv->pixels[2],data->data[2],data->linesize[2]*m_yuv->h);
	
	SDL_UnlockYUVOverlay(m_yuv);
	rect.x = 0;
	rect.y = 0;
	rect.w =  m_yuv->w;
	rect.h = m_yuv->h;
	SDL_DisplayYUVOverlay(m_yuv, &rect);
	m_yuv->pixels =	px;
	return true;
}

void sdl_render::re_size(int width, int height)
{

}

void sdl_render::aspect_ratio(int srcw, int srch, bool enable_aspect)
{

}

void sdl_render::destory_render()
{

}
