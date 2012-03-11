/***************************************************************************
 * export.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <zlib.h>
#include <stdarg.h>
#ifndef NO_PNG
#include <png.h>
#endif
#ifdef HAVE_JPEG
extern "C" {
#include <jpeglib.h>
}
#endif

#include "mgl/canvas.h"
#include "mgl/canvas_cf.h"
//-----------------------------------------------------------------------------
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p)
{
#ifndef NO_PNG
	FILE *fp = fopen(fname, "wb");
	if (!fp)	return 1;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (!png_ptr)	{	fclose(fp);	return 1;	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_write_struct(&png_ptr,0);	fclose(fp);	return 1;	}

	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	png_set_IHDR(png_ptr, info_ptr, w, h, 8,
				PNG_COLOR_TYPE_RGB_ALPHA,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
	png_set_rows(png_ptr, info_ptr, p);
	png_write_png(png_ptr, info_ptr,  PNG_TRANSFORM_IDENTITY, 0);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
#else
	return 1;
#endif
}
//-----------------------------------------------------------------------------
int mgl_png_save(const char *fname, int w, int h, unsigned char **p)
{
#ifndef NO_PNG
	FILE *fp = fopen(fname, "wb");
	if (!fp)	return 1;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (!png_ptr)	{	fclose(fp);	return 1;	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_write_struct(&png_ptr,0);	fclose(fp);	return 1;	}

	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	png_set_IHDR(png_ptr, info_ptr, w, h, 8,
				PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
	png_set_rows(png_ptr, info_ptr, p);
	png_write_png(png_ptr, info_ptr,  PNG_TRANSFORM_IDENTITY, 0);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
#else
	return 1;
#endif
}
//-----------------------------------------------------------------------------
int mgl_bmp_save(const char *fname, int w, int h, unsigned char **p)
{
	FILE *fp = fopen(fname,"wb");
	if (!fp)	return 1;

	char z[4] = {0,0,0,0};
	unsigned u = w*h*3 + 54;
	// BITMAPFILEHEADER
	fwrite("BM",2,1,fp);	fwrite(&u,4,1,fp);
	fwrite(z,4,1,fp);	u=54;	fwrite(&u,4,1,fp);
	// BITMAPINFOHEADER
	u=40;	fwrite(&u,4,1,fp);	fwrite(&w,4,1,fp);	fwrite(&h,4,1,fp);
	unsigned short pp=1;
	fwrite(&pp,2,1,fp);	pp=24;	fwrite(&pp,2,1,fp);	u = w*h*3;
	fwrite(z,4,1,fp);	fwrite(&u,4,1,fp);
	fwrite(z,4,1,fp);	fwrite(z,4,1,fp);
	fwrite(z,4,1,fp);	fwrite(z,4,1,fp);
	// image
	const unsigned char *q;
	register int i,j;
	for(i=h-1;i>=0;i--)	for(j=0;j<w;j++)
	{
		q = p[i]+3*j;
		fwrite(q+2,1,1,fp);
		fwrite(q+1,1,1,fp);
		fwrite(q+0,1,1,fp);
	}
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int mgl_tga_save(const char *fname, int w, int h, unsigned char **p)
{
	FILE *fp = fopen(fname,"wb");
	if (!fp)	return 1;
	// header
	char head[14]={0,0,2, 0,0,0,0,0, 0,0,0,0, 32,0};
	fwrite(head,12,1,fp);
	fwrite(&w,2,1,fp);	fwrite(&h,2,1,fp);
	fwrite(head+12,2,1,fp);
	// image
	register int i,j;
	const unsigned char *q;
	for(i=h-1;i>=0;i--)	for(j=0;j<w;j++)
	{
		q = p[i]+4*j;
		fwrite(q+2,1,1,fp);
		fwrite(q+1,1,1,fp);
		fwrite(q+0,1,1,fp);
		fwrite(q+3,1,1,fp);
	}
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int mgl_jpeg_save(const char *fname, int w, int h, unsigned char **p)
{
#ifdef HAVE_JPEG
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if((outfile = fopen(fname, "wb")) == 0)	return 1;
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = w;
	cinfo.image_height = h;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);
	jpeg_write_scanlines(&cinfo, p, h);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return 0;
#else
	return 1;
#endif
}
//-----------------------------------------------------------------------------
void mgl_printf(void *fp, bool gz, const char *str, ...)
{
	char buf[512];
	va_list lst;
	va_start(lst,str);
	vsprintf(buf,str,lst);
	va_end(lst);
	if(gz)	gzprintf((gzFile)fp, "%s", buf);
	else	fprintf((FILE *)fp, "%s", buf);
}
//---------------------------------------------------------------------------
int mgl_bps_save(const char *fname, int w, int h, unsigned char **p)
{
	time_t now;	time(&now);
	register long i,j;
	bool gz = fname[strlen(fname)-1]=='z';

	void *fp = gz ? (void*)gzopen(fname,"wt") : (void*)fopen(fname,"wt");
	mgl_printf(fp, gz, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n",w,h);
	mgl_printf(fp, gz, "%%%%Created by MathGL library\n%%%%Title: %s\n", fname);
	mgl_printf(fp, gz, "%%%%CreationDate: %s\n",ctime(&now));
	mgl_printf(fp, gz, "%d %d 8 [1 0 0 1 0 0] {currentfile %d string readhexstring pop} false 3 colorimage\n",
			w,h,1+w*h/40);
	for(j=h-1;j>=0;j--)	for(i=0;i<w;i++)
	{
		if((i+w*(h-j-1))%40==0 && i+j>0)	mgl_printf(fp, gz, "\n");
		mgl_printf(fp, gz, "%02x%02x%02x",p[j][3*i],p[j][3*i+1],p[j][3*i+2]);
	}
	mgl_printf(fp, gz, "\n\nshowpage\n%%%%EOF\n");
	if(gz)	gzclose((gzFile)fp);	else	fclose((FILE *)fp);
	return 0;
}
//-----------------------------------------------------------------------------
int mgl_gif_save(const char *fname, int w, int h, unsigned char **l)
{
#ifdef HAVE_GIF
	GifFileType *fg = EGifOpenFileName(fname, 0);
	// define colormap
	GifColorType col[256];
	memset(col,0,256*sizeof(GifColorType));
	register long m;
	register int i,j,k,ii;
	for(i=0;i<6;i++)	for(j=0;j<6;j++)	for(k=0;k<6;k++)
	{
		m = i+6*(j+6*k);		// part 1
		col[m].Red = 51*i;
		col[m].Green=51*j;
		col[m].Blue =51*k;
	}
	// write header
	ColorMapObject *gmap = MakeMapObject(256, col);
	EGifPutScreenDesc(fg, w, h, 256,0,gmap);
	FreeMapObject(gmap);
	// write frame
	EGifPutImageDesc(fg, 0, 0, w, h, 0, 0);
	GifPixelType *line = new GifPixelType[w*h];
	for(m=0;m<w*h;m++)
	{
		ii = 3*(m%w);	k = m/w;
		i = (l[k][ii]+25)/51;
		j = (l[k][ii+1]+25)/51;
		k = (l[k][ii+2]+25)/51;
		line[m] = i+6*(j+6*k);
	}
	EGifPutLine(fg, line, w*h);
	EGifCloseFile(fg);
	delete []line;	return 0;
#else
	return 1;
#endif
}
//-----------------------------------------------------------------------------
//
//		Save animation
//
//-----------------------------------------------------------------------------
void mglCanvas::StartGIF(const char *fname, int ms)
{
#ifdef HAVE_GIF
	if(gif)	EGifCloseFile(gif);
	EGifSetGifVersion("89a");
	std::string fn=fname;
	if(fn.empty())	{	fn=PlotId+".gif";	fname = fn.c_str();	}
	gif = EGifOpenFileName(fname, 0);
	// get picture sizes
	// NOTE: you shouldn't call SetSize() after StartGIF() !!!
	long width, height;
	unsigned char *f=0;
	GetRGBLines(width, height, f);
	if(f)	free(f);
	// define colormap
	GifColorType col[256];
	memset(col,0,256*sizeof(GifColorType));
	register int i,j,k,m;
	for(i=0;i<6;i++)	for(j=0;j<6;j++)	for(k=0;k<6;k++)
	{
		m = i+6*(j+6*k);		// part 1
		col[m].Red = 51*i;
		col[m].Green=51*j;
		col[m].Blue =51*k;
	}
	// write header
	ColorMapObject *gmap = MakeMapObject(256, col);
	EGifPutScreenDesc(gif, width, height, 256,0,gmap);
	FreeMapObject(gmap);
	// put animation parameters
	ms /= 10;
	unsigned char ext1[11] = {0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30}, ext3[3] = {0x01, 0xff, 0xff}, ext2[9] = {0x08, ms%256, ms/256, 0xff};
	EGifPutExtensionFirst(gif,0xff,11,ext1);
	EGifPutExtensionLast(gif,0xff,3,ext3);
	EGifPutExtension(gif,0xf9,4,ext2);
#endif
}
//-----------------------------------------------------------------------------
void mglCanvas::CloseGIF()
{
#ifdef HAVE_GIF
	if(gif)	EGifCloseFile(gif);
#endif
	gif = 0;
}
//-----------------------------------------------------------------------------
int mglCanvas::NewFrame()
{
	Clf();	Identity();	CurFrameId++;
	return CurFrameId-1;
}
//-----------------------------------------------------------------------------
void mglCanvas::EndFrame()
{
#ifdef HAVE_GIF
	long width, height, n;
	unsigned char *f=0, **l=0;
	l = GetRGBLines(width, height, f);
	n = width*height;
	if(!l || !gif)	return;
	EGifPutImageDesc(gif, 0, 0, width, height, 0, 0);
	GifPixelType *line = new GifPixelType[n];
	register long m;
	register int i,j,k,ii;
	for(m=0;m<n;m++)
	{
		ii = 3*(m%width);	k = m/width;
		i = (l[k][ii]+25)/51;
		j = (l[k][ii+1]+25)/51;
		k = (l[k][ii+2]+25)/51;
		line[m] = i+6*(j+6*k);
	}
	EGifPutLine(gif, line, n);
	delete []line;	free(l);
	if(f)	free(f);
#else
	if(get(MGL_USEDRWDAT))	PushDrwDat();
	Finish();
#endif
}
//-----------------------------------------------------------------------------
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
//-----------------------------------------------------------------------------
void mgl_write_png(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f,true);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".png";	fname = fn.c_str();	}
		if(mgl_pnga_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_png_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_png(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".png";	fname = fn.c_str();	}
		if(mgl_png_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_png_solid_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_png_solid(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_jpg(HMGL gr, const char *fname,const char *)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".jpg";	fname = fn.c_str();	}
		if(mgl_jpeg_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_jpg_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_jpg(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_tga(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f,true);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".tga";	fname = fn.c_str();	}
		if(mgl_tga_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_tga_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_tga(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_bmp(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".bmp";	fname = fn.c_str();	}
		if(mgl_bmp_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_bmp_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_bmp(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_bps(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".eps";	fname = fn.c_str();	}
		if(mgl_bps_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_bps_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_bps(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_write_gif(HMGL gr, const char *fname,const char *descr)
{
	long w,h;	unsigned char *f=0, **p=0;
	p =_Gr_->GetRGBLines(w,h,f);
	if(p)
	{
		std::string fn=fname;
		if(fn.empty())	{	fn=gr->PlotId+".gif";	fname = fn.c_str();	}
		if(mgl_gif_save(fname,w,h,p))	gr->SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
void mgl_write_gif_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_gif(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_start_gif(HMGL gr, const char *fname,int ms)
{	_Gr_->StartGIF(fname,ms);	}
void mgl_start_gif_(uintptr_t *gr, const char *fname,int *ms,int l)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	mgl_start_gif(_GR_,s,*ms);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_close_gif(HMGL gr)			{	_Gr_->CloseGIF();	}
void mgl_close_gif_(uintptr_t *gr)	{	mgl_close_gif(_GR_);	}
//-----------------------------------------------------------------------------
void mgl_write_frame(HMGL gr, const char *fname,const char *descr)
{
	char buf[64];
	if(!fname || !fname[0])
	{	sprintf(buf,"%s%04d.jpg",_Gr_->PlotId.c_str(),_Gr_->GetNumFrame());	fname = buf;	}
	int len=strlen(fname);
	if(!strcmp(fname+len-4,".jpg"))	mgl_write_jpg(gr,fname,descr);
	if(!strcmp(fname+len-5,".jpeg"))mgl_write_jpg(gr,fname,descr);
	if(!strcmp(fname+len-5,".idtf"))mgl_write_idtf(gr,fname,descr);
	if(!strcmp(fname+len-4,".png"))	mgl_write_png(gr,fname,descr);
	if(!strcmp(fname+len-4,".eps"))	mgl_write_eps(gr,fname,descr);
	if(!strcmp(fname+len-4,".svg"))	mgl_write_svg(gr,fname,descr);
	if(!strcmp(fname+len-4,".gif"))	mgl_write_gif(gr,fname,descr);
	if(!strcmp(fname+len-4,".bmp"))	mgl_write_bmp(gr,fname,descr);
	if(!strcmp(fname+len-4,".tga"))	mgl_write_tga(gr,fname,descr);
}
void mgl_write_frame_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_frame(_GR_,s,f);	delete []s;		delete []f;}
//-----------------------------------------------------------------------------
void mgl_show_image(HMGL gr, const char *viewer, int keep)
{
	char fname[128], *cmd = new char [128];
	sprintf(fname,"%s.png", tmpnam(NULL));
	mgl_write_png_solid(gr,fname,"MathGL ShowImage file");
	if(!viewer || !viewer[0])
		viewer = MGL_DEF_VIEWER;
	#ifdef WIN32
		if(keep)
		{
			sprintf(cmd,"%s %s &", viewer,fname);
			if(system(cmd)==-1)	printf("Error to call external viewer\n");
			//		sleep(2);
			sprintf(cmd,"del %s", fname);
		}
		else	sprintf(cmd,"%s %s; del %s", viewer,fname,fname);
		#else
		if(keep)
		{
			sprintf(cmd,"%s %s &", viewer,fname);
			if(system(cmd)==-1)	printf("Error to call external viewer\n");
			sleep(2);
			sprintf(cmd,"rm %s", fname);
		}
		else	sprintf(cmd,"%s %s; rm %s", viewer,fname,fname);
		#endif
		if(system(cmd)==-1)	printf("Error to call external viewer\n");
		delete []cmd;
}
void mgl_show_image_(uintptr_t *gr, const char *viewer, int *keep, int l)
{	char *s=new char[l+1];	memcpy(s,viewer,l);	s[l]=0;
	mgl_show_image(_GR_,s,*keep);	delete []s;	}
//-----------------------------------------------------------------------------
