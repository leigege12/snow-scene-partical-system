#pragma once
#include "GL/glut.h"
#include<stdio.h>  
struct TextureTga            // ����һ���ṹ��  
{
	GLubyte *imageData;           // ͼ������ (���32bit)  
	GLuint bpp;            // ÿһ���ص�ͼ����ɫ���  
	GLuint width;            // ͼ����  
	GLuint height;            // ͼ��߶�  
	GLuint texID;            // ����ID  
};

// ����BMP,JPG,GIF���ļ�  
bool buildTexture(char *szPathName, GLuint &texid);

// ����TGA�ļ�  
bool buildTexture(char *filename, TextureTga *texture);
