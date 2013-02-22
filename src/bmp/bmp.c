#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_ERROR_H 

/* 这个结构体用于存储bmp文件的文件头的信息 */
typedef struct bmp_head {
	short int BMPsyg;
	short int nic[8];
	short int ix;
	short int nic2;
	short int iy;
	short int nic3[2];
	short int depth;
	short int rle;
	short int nic4[11];
} bmp_head;

int load_bmp(const char *filepath, LCUI_Graph *out)
/* 打开并载入BMP图片文件内的图形数据 */
{
	FILE *fp;
	bmp_head bmp;
	int size, m, n, x, y, temp, tempi, rle,pocz, omin;
	unsigned char rozp;
	unsigned char **bak_rgba;
	
	fp = fopen(filepath,"r");
	if(fp == NULL) {
		return FILE_ERROR_OPEN_ERROR;
	}
	
	/* 检测是否为bmp图片 */ 
	temp = fread(&bmp, 1, sizeof(bmp_head),fp);
	if (temp < sizeof(bmp_head) || bmp.BMPsyg != 19778) {
		return FILE_ERROR_UNKNOWN_FORMAT; 
	}
	
	pocz = bmp.nic[4];
	out->bit_depth = bmp.depth;
	rle = bmp.rle;
	if ((out->bit_depth != 32) && (out->bit_depth != 24) 
		&& (out->bit_depth != 8) && (out->bit_depth !=4)) { 
		printf("错误(bmp):位深 %i 不支持!\n",out->bit_depth);
		return  FILE_ERROR_UNKNOWN_FORMAT;
	}
	out->have_alpha = FALSE;     /* 没有透明效果 */
	temp = Graph_Create(out, bmp.ix, bmp.iy);
	if(temp != 0) {
		printf("错误(bmp):无法分配足够的内存供存储数据!\n");
		return 1;
	}
	
	size = out->width * out->height * sizeof(unsigned char);
	bak_rgba = (unsigned char**)malloc(3 * sizeof(unsigned char*));
	bak_rgba[0] = (unsigned char*)malloc(size);
	bak_rgba[1] = (unsigned char*)malloc(size);
	bak_rgba[2] = (unsigned char*)malloc(size);
	
	if(!bak_rgba || !bak_rgba[0] || !bak_rgba[1] || !bak_rgba[2]){
		printf("错误(bmp):无法分配足够的内存供存储数据!\n");
		return 1;
	}
	rozp = ftell(fp);
	fseek(fp,0,SEEK_END);
	omin = ftell(fp);
	omin = omin-pocz;
	omin = omin-((out->width*out->height)*(out->bit_depth/8));
	omin = omin/(out->height);
	fseek(fp,pocz,SEEK_SET);
	switch(out->bit_depth){ 
	case 32:
		for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				out->rgba[2][n]=fgetc(fp);
				out->rgba[1][n]=fgetc(fp);
				out->rgba[0][n]=fgetc(fp);
				tempi=fgetc(fp);
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) fgetc(fp);
		}
	break;
		 
	case 24:
		for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				out->rgba[2][n]=fgetc(fp);
				out->rgba[1][n]=fgetc(fp);
				out->rgba[0][n]=fgetc(fp);
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) fgetc(fp); 
		}
	break;
		
	case 8: 
		fseek(fp,-256*4,SEEK_CUR);
		for (tempi=0;tempi<256;tempi++) {
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		/* nonRLE */	
		if (rle==0) for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				if ((tempi=fgetc(fp))==-1) return 0;
				out->rgba[0][n]=bak_rgba[0][tempi];
				out->rgba[1][n]=bak_rgba[1][tempi];
				out->rgba[2][n]=bak_rgba[2][tempi];
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) 
				getc(fp); 
		}   /* end nonRLE */
		else for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				if ((out->bit_depth=fgetc(fp))==-1) 
					return 0;
				n = m+x+rle;
				tempi=fgetc(fp);
				if (out->bit_depth==0) {
					for (rle=0;rle<tempi;rle++){
						out->bit_depth=fgetc(fp);
						out->rgba[0][n]=bak_rgba[0][out->bit_depth];
						out->rgba[1][n]=bak_rgba[1][out->bit_depth];
						out->rgba[2][n]=bak_rgba[2][out->bit_depth];
					}
					if (tempi!=((tempi/2)*2)) tempi=fgetc(fp);
				} else for (rle=0;rle<out->bit_depth;rle++){
					out->rgba[0][n]=bak_rgba[0][tempi];
					out->rgba[1][n]=bak_rgba[1][tempi];
					out->rgba[2][n]=bak_rgba[2][tempi];
				} 
				x+=rle-1;
			}	
		}
		/* end RLE  */  
		break;

	case 4: 
		fseek(fp,-16*4,SEEK_CUR);
		for (tempi=0;tempi<16;tempi++) {
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		/* nonRLE */
		if (rle==0) for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = m+x;
				tempi=fgetc(fp);
				rozp=tempi;
				rozp=(rozp & 0xf0)>>4;
				out->rgba[0][n]=bak_rgba[0][rozp];
				out->rgba[1][n]=bak_rgba[1][rozp];
				out->rgba[2][n]=bak_rgba[2][rozp];
				x++;
				rozp=tempi;
				rozp=(rozp & 0x0f);
				out->rgba[0][n]=bak_rgba[0][rozp];
				out->rgba[1][n]=bak_rgba[1][rozp];
				out->rgba[2][n]=bak_rgba[2][rozp];
			} 
		} /* end nonRLE */
		else { /* RLE */
			
		} /* end RLE */ 
	break;
	}
	out->type = TYPE_BMP;     /* 图片类型为bmp */
	free(bak_rgba[0]);
	free(bak_rgba[1]);
	free(bak_rgba[2]);
	free(bak_rgba);
	fclose(fp);
	return 0;
}
