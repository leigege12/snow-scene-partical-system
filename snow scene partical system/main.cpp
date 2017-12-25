#define GLUT_DISABLE_ATEXIT_HACK
#define FRAMETIME 0.033f
#include <cmath>
#include <map>
#include <set>
#include <ctime>
#include <GL/glut.h>
#include "obj_mat.h"
#include "texture.h"
#include "move.h"
#include "partical_emitter.h"

GLint windowHandler;
const int skySizeX = 200;
const int skySizeY = 100;
const int skySizeZ = 200;
//光源
GLfloat light_constant = 0.01f;
GLfloat light_linear = 0.001f;
GLfloat light_quadratic = 0.0001f;
GLfloat ldirect[3] = { 0.0f,-0.2f,-1.0f };
GLfloat lcutoff =22.5f;
GLfloat lexponent = 5.0f;
GLfloat lpos[4] = { 0.0f, -1.0f*skySizeY / 2 + 20.0f, 0.0f , 1.0f };
GLfloat lAmb[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat lDif[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat lSpe[4] = { 0.9f, 0.9f, 0.0f, 1.0f };
GLfloat emission[] = { 0.9f, 0.9f, 0.0f, 1.0f };
//没有mtl文件使用的材质
GLfloat temp[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat shininess = 10.0f;


GLuint texture[10];
// 房子 显示列表
GLint houseList = 0;
// 地面 显示列表
GLint baseList = 0;
//视区      
float whRatio;
int wHeight = 0;
int wWidth = 0;


//视点      
float center[] = { 0, -1.0f*skySizeY / 2 + 20.0f, 0 };
float eye[] = { 0, -1.0f*skySizeY/2+20.0f, 50 };
GLfloat dx = 0, dy = 0, dz = 0;
GLfloat ax = 0, ay = 0, az = 0;
GLint mx = 0, my = 0;
GLint MouseDown = 0;
float ratio = 180;

//int col = 4, num = 12;


Emitter *snowflower_emitter;
Emitter *smoke_emitter;


//加载obj
std::map<std::string, Object> housemap;
std::set<std::string> housename;
std::string house;
std::map<std::string, Material> house_matname;

std::map<std::string, Object> basemap;
std::set<std::string> basename;
std::string base;
std::map<std::string, Material> base_matname;

Particle* init_snowflower_partical()
{
	float size = rand() % 20 * 0.03f;
	unsigned char color[] = { 1.0f,0.0f,0.0f };
	float speed[] = { float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 400, float(rand() % 10 - 4) / 800 };
	float acc[] = { 1.0f*(rand() % 3 - 1) / 2000,-4.9 / 4000 ,1.0f*(rand() % 3 - 1) / 2000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec(size, size, size), vec(speed), vec(acc),
		vec(angle), rand() % 100, texture[2]);
	return p;
}
Particle* init_smoke_partical()
{
	float size = rand() % 90 * 0.02f;
	float speed[] = { float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600 };
	float acc[] = { 1.0f*(rand() % 3 - 1) / 5000,4.9 / 2000 ,1.0f*(rand() % 3 - 1) / 5000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec(size, size, size), vec(speed), vec(acc),
		vec(angle), rand() % 50 + 10, texture[3]);
	return p;
}

bool snowflower_dead(Particle* p)
{
	const vec& place = p->getPlace();
	if (place.y < -1.0f*skySizeY / 2) {
		//s->add(place.x, place.z, p->getSize(), p->getAngle());
		return true;
	}
	return false;
}

bool smoke_dead(Particle* p)
{
	const vec& place = p->getPlace();
	if (place.y > 1.0f*skySizeY) {
		//s->add(place.x, place.z, p->getSize(), p->getAngle());
		return true;
	}
	return false;
}


/*
void drawTop(GLuint texture)
{
//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
//glMaterialf(GL_FRONT, GL_SHININESS, shininess);
//glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]
int i = 0, j = 0;

const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
const GLfloat x0[] = { 0,x,2 * x,3 * x,4 * x };
const GLfloat x1 = -0.5f, x2 = 0.5f;
const GLfloat y1 = -0.5f, y2 = 0.5f;
const GLfloat point[12][2] = {
{ x2,y1 },{ 0,0 },{ x1,y1 },
{ x1,y1 },{ 0,0 },{ x1,y2 },
{ x1,y2 },{ 0,0 },{ x2,y2 },
{ x2,y2 },{ 0,0 },{ x2,y1 },
};
const GLfloat dir[12][2] = { { x0[0],1 - y },{ x0[2],1 },{ x0[1],1 - y },
{ x0[1],1 - y },{ x0[2],1 },{ x0[2],1 - y },
{ x0[2],1 - y },{ x0[2],1 },{ x0[3],1 - y },
{ x0[3],1 - y },{ x0[2],1 },{ x0[4],1 - y } };

glBegin(GL_TRIANGLES);

for (int k = 0; k < 12; k++) {
glTexCoord2fv(dir[k]);
glVertex2fv(point[k]);
}
glEnd();


glDisable(GL_TEXTURE_2D);
}

void drawBottom(GLuint texture)
{
//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
//glMaterialf(GL_FRONT, GL_SHININESS, shininess);
//glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]
int i = 0, j = 0;

const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
const GLfloat x0[] = { 0,x,2 * x,3 * x,4 * x };
const GLfloat x1 = -0.5f, x2 = 0.5f;
const GLfloat y1 = -0.5f, y2 = 0.5f;
const GLfloat point[12][2] = {
{ x2,y1 },{ 0,0 },{ x1,y1 },
{ x1,y1 },{ 0,0 },{ x1,y2 },
{ x1,y2 },{ 0,0 },{ x2,y2 },
{ x2,y2 },{ 0,0 },{ x2,y1 },
};
const GLfloat dir[12][2] = { { x0[0],y },{ x0[2],0 },{ x0[1],y },
{ x0[1],y },{ x0[2],0 },{ x0[2],y },
{ x0[2],y },{ x0[2],0 },{ x0[3],y },
{ x0[3],y },{ x0[2],0 },{ x0[4],y } };

glBegin(GL_TRIANGLES);

for (int k = 0; k < 12; k++) {
glTexCoord2fv(dir[k]);
glVertex2fv(point[k]);
}
glEnd();


glDisable(GL_TEXTURE_2D);
}

*/

void drawLine() {
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(0.0f, -1.0f*skySizeY/2, 0.0f);
	glVertex3f(0.0f, 1.0f*skySizeY / 2, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
	
	glPopMatrix();
}

void drawRect(GLuint texture)
{
	/*
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	*/
	glMaterialfv(GL_FRONT, GL_AMBIENT, temp);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, temp);
	glMaterialfv(GL_FRONT, GL_SPECULAR, temp);
	glMaterialfv(GL_FRONT, GL_EMISSION, temp);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]       
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	int dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
}


/*
void drawRect(GLuint texture, int i, int j)
{
glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
glMaterialf(GL_FRONT, GL_SHININESS, shininess);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]

const GLfloat x1 = -0.5, x2 = 0.5;
const GLfloat y1 = -0.5, y2 = 0.5;
const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
//const GLfloat dir[4][2] = { { j*x,1 - (i + 1)*y },{ (j + 1)*x,1 - (i + 1)*y },{ (j + 1)*x ,1 - i*y },{ j*x,1 - i*y } };
const GLfloat dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };

glBegin(GL_QUADS);

for (int k = 0; k < 4; k++) {
glTexCoord2fv(dir[k]);
glVertex2fv(point[k]);
}
glEnd();

glDisable(GL_TEXTURE_2D);
}

*/
/*
void drawRect( int i, int j)
{
glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
glMaterialf(GL_FRONT, GL_SHININESS, shininess);

const GLfloat x1 = -0.5, x2 = 0.5;
const GLfloat y1 = -0.5, y2 = 0.5;
const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
const GLfloat dir[4][2] = { { j*x,1 - (i + 1)*y },{ (j + 1)*x,1 - (i + 1)*y },{ (j + 1)*x ,1 - i*y },{ j*x,1 - i*y } };
glBegin(GL_QUADS);

for (int k = 0; k < 4; k++) {
glVertex2fv(point[k]);
}
glEnd();

}

*/

int a = 0;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存     
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //初始化矩阵为单位矩阵          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	glEnable(GL_LIGHTING);
	//Set up light
	glLightfv(GL_LIGHT1, GL_POSITION, lpos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lSpe);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, ldirect);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lcutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lexponent);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, light_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, light_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, light_quadratic);
	glEnable(GL_LIGHT1);

	glPushMatrix();


	glTranslatef(dx, dy, dz);
	glRotatef(ax, 1.0f, 0.0f, 0.0f);
	glRotatef(ay, 0.0f, 1.0f, 0.0f);
	glInitNames();//初始化名字  
	glPushName(0);//初始化名字栈  

	drawLine();

	//画一个光源
	glPushMatrix();
	glTranslatef(lpos[0], lpos[1], lpos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glutSolidSphere(1, 20, 20);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);

	glPopMatrix();

	//画房子
	glCallList(houseList);
	//画烟囱
	glCallList(baseList);

	//门
	/*
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	
	*/
	

	glLoadName(1);//加载名字  
	glPushMatrix();
	glTranslatef(-17.0f, -1.0f*skySizeY / 2.0f+6.0f, -1.0f*skySizeZ / 2.0f + 76.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(10, 11, 1);
	drawRect(texture[0]);
	glPopMatrix();
	/*
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
	*/
	
	//窗
	glLoadName(2);//加载名字  
	glPushMatrix();
	glTranslatef(-30.0f, -1.0f*skySizeY / 2.0f+8.5f, -1.0f*skySizeZ / 2.0f + 76.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(7, 7, 1);
	drawRect(texture[1]);
	glPopMatrix();


	glPopName();

	//地板

	glPushMatrix();
	glTranslatef(0.0f, -1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	//drawBottom(texture[4]);
	drawRect(texture[4]);
	glPopMatrix();



	//天花板
	glPushMatrix();
	glTranslatef(0.0f, 1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	//drawTop(texture[5]);
	drawRect(texture[5]);
	glPopMatrix();

	
	//墙壁（前）
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*skySizeZ / 2.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[6]);
	glPopMatrix();
	
	//墙壁（后）
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*skySizeZ / 2.0f);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[7]);
	glPopMatrix();

	//墙壁（左）
	glPushMatrix();
	glTranslatef(-1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[8]);
	glPopMatrix();

	//墙壁（右）
	glPushMatrix();
	glTranslatef(1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[9]);
	glPopMatrix();
	

	glEnable(GL_BLEND);
	glPushMatrix();
	snowflower_emitter->show();
	smoke_emitter->show();

	glPopMatrix();

	glDisable(GL_BLEND);
	glPopMatrix();
	glutSwapBuffers();//交换缓冲区      

}

void timeFunc(int value) {
	a += 10;
	snowflower_emitter->infinite_update();
	smoke_emitter->infinite_update();
	//printf("frame time\n" );
	glutPostRedisplay();
	glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);

}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影           
	glLoadIdentity();   //初始化矩阵为单位矩阵              
	whRatio = (GLfloat)width / (GLfloat)height;  //设置显示比例             
	gluPerspective(45, whRatio, 1, 400); //透视投影  
	glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型        
}


void reshape(int width, int height) {
	if (height == 0) { //如果高度为0     
		height = 1;   //让高度为1（避免出现分母为0的现象）          
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth); //更新视角        
}


void myMouse(int button, int state, int x, int y) {

}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q': {exit(0); break; } //退出  

	case 'a': { //左移  
		move(eye, center, direction::left);
		break;
	}
	case 'd': { //右移  
		move(eye, center, direction::right);
		break;
	}
	case 'w': { //前移  
		move(eye, center, direction::front);
		break;
	}
	case 's': {  //后移  
		move(eye, center, direction::back);
		break;
	}

	case 'j': {//视角左移  
		rotate(eye, center, direction::left);
		break;
	}
	case 'l': {//视角右移  
		rotate(eye, center, direction::right);
		break;
	}
	case 'i': {//视角上移  
		center[1] += 2.0f;
		break;
	}
	case 'k': {//视角上移  
		center[1] -= 2.0f;
		break;
	}
	}
}



GLint GenTableList()
{
	GLint lid = glGenLists(1);

	glNewList(lid, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, -1.0f*skySizeY / 2.0f - 2.0f, -1.0f*skySizeZ / 2.0f + 60.0f);
	glScalef(3.0f, 3.0f, 3.0f);
	glRotatef(90, 0, 1, 0);
	loadObj(housename, housemap, house_matname);
	glPopMatrix();

	glNewList(lid, GL_COMPILE);
	glPushMatrix();
	//glTranslatef(-5.0f, -1.0f*skySizeY / 2.0f, -23.0f);
	glTranslatef(-25.0f, -1.0f*skySizeY / 2.0f + 24.0f, -1.0f*skySizeZ / 2.0f + 64.0f);

	glScalef(1.2f, 0.5f, 1.2f);
	loadObj(basename, basemap, base_matname);
	glPopMatrix();

	glEndList();
	return lid;
}

void setupRC() {
	srand(unsigned(time(NULL)));
	glClearColor(1.0, 0.0, 0.0, 0.0);

	//平滑过渡效果
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	readObj(house, "obj\\house.obj", housemap, housename, house_matname);
	readObj(house, "obj\\base.obj", basemap, basename, base_matname);
	houseList = GenTableList();
	baseList = houseList + 1;

	texture[0] = readTexture("door.bmp");
	texture[1] = readTexture("window.bmp");
	texture[2] = readTexture("snowflower.bmp");
	texture[3] = readTexture("flame.bmp");
	texture[4] = readTexture("roof.bmp");
	texture[5] = readTexture("up.bmp");
	texture[6] = readTexture("front.bmp");
	texture[7] = readTexture("back.bmp");
	texture[8] = readTexture("left.bmp");
	texture[9] = readTexture("right.bmp");

	/*
	buildTexture("door.jpg", texture[0]);
	buildTexture("window.jpg", texture[1]);
	buildTexture("snowflower.jpg", texture[2]);
	buildTexture("flame.bmp", texture[3]);
	buildTexture("roof.bmp", texture[4]);
	buildTexture("up.bmp", texture[5]);
	buildTexture("front.bmp", texture[6]);
	buildTexture("back.bmp", texture[7]);
	buildTexture("left.bmp", texture[8]);
	buildTexture("right.bmp", texture[9]);
	*/
	
	//BuildTexture("Butterfly2.bmp", texture[6]);
	//BuildTexture("Butterfly3.bmp", texture[7]);

	//s = new snow(texture[0]);

	snowflower_emitter = new Emitter(15000, -1.0f*skySizeX / 2.0f, 1.0f*skySizeX / 2.0f, 1.0f*skySizeY / 2.0f, 1.0f*skySizeY / 2.0f, -1.0f*skySizeZ / 2.0f, 1.0f*skySizeZ / 2.0f);
	snowflower_emitter->emit(init_snowflower_partical, snowflower_dead);
	smoke_emitter = new Emitter(2500, -31.0f, -28.0f, -1.0f*skySizeY / 2.0f + 28.0f, -1.0f*skySizeY / 2.0f + 28.0f, -1.0f*skySizeZ / 2.0f + 67.5f, -1.0f*skySizeZ / 2.0f + 69.5f);
	smoke_emitter->emit(init_smoke_partical, smoke_dead);
	//e2 = new emitter(400, 1.5f, 2.5f, -8.0f, -7.5f, -19.5f, -20.5f);
	//e2->emit(init_flame);

	//int* tex = new int[3];
	//tex[0] = texture[5];
	//tex[1] = texture[6];
	//tex[2] = texture[7];

	//butt = new butterflyManager(tex);
	//delete tex;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//对glut的初始化             
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//初始化显示模式:RGB颜色模型，深度测试，双缓冲               
	glutInitWindowSize(800, 600);//设置窗口大小             
	windowHandler = glutCreateWindow("Partical System Display");//设置窗口标题               
	glutDisplayFunc(renderScene); //注册绘制回调函数             
	glutReshapeFunc(reshape);   //注册重绘回调函数    
	glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	//glutIdleFunc(idleFunc);//注册全局回调函数：空闲时调用           
	setupRC();
	glutMainLoop();  // glut事件处理循环           
	system("pause");
	return 0;
}