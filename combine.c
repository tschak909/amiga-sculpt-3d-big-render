/**
 * @brief   Convert 24-bit RAW image data to Sun Raster file format
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE
 */

/* compile with: cc -ocombine combine.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_OLD 0
#define NO_COLORMAP 0
#define DEPTH_BITS 24
#define DEPTH_BYTES (DEPTH_BITS/8)
#define RAS_MAGIC_NUMBER 0x59a66a95

int big_endian_int(int n)
{
	return ((n & 0x000000ff) << 24u) |
		(( n & 0x0000ff00) << 8u) | 
		(( n & 0x00ff0000) >> 8u) |
		(( n & 0xff000000) >> 24u);
}

int main(int argc, char **argv)
{
	struct rasterfile 
	{
		int ras_magic;
		int ras_width;
		int ras_height;
		int ras_depth;
		int ras_length;
		int ras_type;
		int ras_maptype;
		int ras_maplength;	
	} r;

	int w = atoi(argv[5]);
	int h = atoi(argv[6]);

	if (argc<6)
	{
		printf("%s <r.raw> <b.raw> <g.raw> <c.ras> <width> <height>\n",argv[0]);
		return 1;
	}

	r.ras_magic = big_endian_int(RAS_MAGIC_NUMBER);
	r.ras_width = big_endian_int(w);
	r.ras_height = big_endian_int(h);
	r.ras_depth = big_endian_int(DEPTH_BITS);
	r.ras_length = ((w * h)/DEPTH_BYTES) * DEPTH_BYTES;
	r.ras_type = TYPE_OLD;
	r.ras_maptype = NO_COLORMAP;
	r.ras_maplength = 0;

	FILE *fr = fopen(argv[1],"rb");
	FILE *fg = fopen(argv[2],"rb");
	FILE *fb = fopen(argv[3],"rb");
	FILE *fc = fopen(argv[4],"wb");

	/* No, there is no error handling. This is for demonstrative purposes only. */

	fwrite(&r,sizeof(struct rasterfile),1,fc);

	/* Bytes in body are in BGR order */

	while(!feof(fr))
	{
		fputc(fgetc(fb),fc);
		fputc(fgetc(fg),fc);
		fputc(fgetc(fr),fc);
	}

	fclose(fr);
	fclose(fg);
	fclose(fb);
	fclose(fc);

	return 0;
}

