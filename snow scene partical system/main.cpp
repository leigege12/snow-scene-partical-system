#define GLUT_DISABLE_ATEXIT_HACK
#define FRAMETIME 0.033f
#define SNOWFLAKE_NUM 20000
#define SMOKE_NUM 2500
#define LIGHT_NUM 200
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

bool snow_stop = false;
bool smoke_stop = false;



GLfloat light_constant = 0.01f;
GLfloat light_linear = 0.005f;
GLfloat light_quadratic = 0.0001f;
GLfloat ldirect[3] = { 0.0f,-0.2f,-1.0f };
GLfloat lcutoff =22.5f;
GLfloat lexponent = 5.0f;
GLfloat lpos[4] = { 0.0f, -1.0f*skySizeY / 2 + 20.0f, 0.0f , 1.0f };
GLfloat lAmb[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat lDif[4] = { 0.7f, 0.7f, 0.0f, 1.0f };
GLfloat lSpe[4] = { 0.9f, 0.9f, 0.0f, 1.0f };
GLfloat emission[] = { 0.9f, 0.9f, 0.0f, 1.0f };



GLfloat temp[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
GLfloat diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat shininess = 10.0f;


GLuint texture[10];


GLint houseList = 0;
GLint baseList = 0;


float whRatio;
int wHeight = 0;
int wWidth = 0;


float center[] = { 0, -1.0f*skySizeY / 2 + 20.0f, 0 };
float eye[] = { 0, -1.0f*skySizeY/2+20.0f, 50 };
GLfloat dx = 0, dy = 0, dz = 0;
GLfloat ax = 0, ay = 0, az = 0;
float ratio = 180;



Emitter *snowflower_emitter;
Emitter *smoke_emitter;
Emitter *light_emitter;


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
	float acc[] = { 1.0f*(rand() % 3 - 1) / 2000,-0.2f*((rand() % 5)+1) / 2000 -4.9f/2000 ,1.0f*(rand() % 3 - 1) / 2000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec3(size, size, size), vec3(speed), vec3(acc),
		vec3(angle), rand() % 100, texture[2]);
	return p;
}
Particle* init_smoke_partical()
{
	float size = rand() % 90 * 0.02f;
	float speed[] = { float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 800, float(rand() % 10 - 4) / 1600 };
	float acc[] = { 1.0f*(rand() % 3 - 1) / 5000, 0.2f *((rand() % 5)+1)/ 2000+4.9f/2000 ,1.0f*(rand() % 3 - 1) / 5000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec3(size, size, size), vec3(speed), vec3(acc),
		vec3(angle), rand() % 50 + 10, texture[3]);
	return p;
}


Particle* init_light_partical()
{
	float size = rand() % 90 * 0.02f;
	float speed[] = { float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 1600, float(rand() % 10 - 4) / 1600 };
	float acc[] = { 1.0f*(rand() % 3 - 1) / 5000, 1.0f*(rand() % 3 - 1) / 5000 , 1.0f*(rand() % 3 - 1) / 5000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec3(size, size, size), vec3(speed), vec3(acc),
		vec3(angle), rand() % 50 + 10, texture[3]);
	return p;
}


bool snowflower_dead(Particle* p)
{
	const vec3& place = p->getPlace();
	if (place.y < -1.0f*skySizeY / 2) {
		return true;
	}
	return false;
}

bool smoke_dead(Particle* p)
{
	const vec3& place = p->getPlace();
	if (place.y > 1.0f*skySizeY) {
		return true;
	}
	return false;
}

bool light_dead(Particle* p)
{
	const vec3& place = p->getPlace();
	if (abs(place.x-lpos[0])>0.5f || abs(place.y - lpos[1])>0.5f || abs(place.z - lpos[2])>0.5f) {
		return true;
	}
	return false;
}




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
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, temp);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	int dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };
	glFrontFace(GL_CW);
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
	glFrontFace(GL_CCW);

}




int a = 0;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();         
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	glEnable(GL_LIGHTING);
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
	glDisable(GL_LIGHT0);

	glPushMatrix();


	glTranslatef(dx, dy, dz);
	glRotatef(ax, 1.0f, 0.0f, 0.0f);
	glRotatef(ay, 0.0f, 1.0f, 0.0f);
	
	drawLine();

	
	

	glPushMatrix();
	glTranslatef(-17.0f, -1.0f*skySizeY / 2.0f+6.0f, -1.0f*skySizeZ / 2.0f + 76.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(10, 11, 1);
	drawRect(texture[0]);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(-30.0f, -1.0f*skySizeY / 2.0f+8.5f, -1.0f*skySizeZ / 2.0f + 76.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(7, 7, 1);
	drawRect(texture[1]);
	glPopMatrix();




	glPushMatrix();
	glTranslatef(0.0f, -1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawRect(texture[4]);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0f, 1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawRect(texture[5]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*skySizeZ / 2.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[6]);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*skySizeZ / 2.0f);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[7]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[8]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[9]);
	glPopMatrix();
	

	glCallList(houseList);
	glCallList(baseList);

	glEnable(GL_BLEND);
	glPushMatrix();
	snowflower_emitter->show();
	smoke_emitter->show();
	light_emitter->show();

	glPopMatrix();

	glDisable(GL_BLEND);
	glPopMatrix();
	glutSwapBuffers();

}

void timeFunc(int value) {
	a += 10;
	if (snow_stop)
		snowflower_emitter->update();
	else
		snowflower_emitter->infinite_update();
	if (smoke_stop)
		smoke_emitter->update();
	else
		smoke_emitter->infinite_update();
	light_emitter->infinite_update();
	glutPostRedisplay();
	glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);

}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();            
	whRatio = (GLfloat)width / (GLfloat)height;             
	gluPerspective(45, whRatio, 1, 400); 
	glMatrixMode(GL_MODELVIEW);   
}


void reshape(int width, int height) {
	if (height == 0) { 
		height = 1;           
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth);      
}


void myKeyboard(unsigned char key, int x, int y) {
	switch (key){
			case 'q': {
			exit(0); 
			break; 
		} 

		case 'a': { 
			move(eye, center, direction::left);
			break;
		}
		case 'd': { 
			move(eye, center, direction::right);
			break;
		}
		case 'w': { 
			move(eye, center, direction::front);
			break;
		}
		case 's': {  
			move(eye, center, direction::back);
			break;
		}

		case 'j': {
			rotate(eye, center, direction::left);
			break;
		}
		case 'l': {  
			rotate(eye, center, direction::right);
			break;
		}
		case 'i': { 
		center[1] += 2.0f;
			break;
		}
		case 'k': {
			center[1] -= 2.0f;
			break;
		}
		case 't' :{
			lpos[2] -= 2.0f;
			light_emitter->changeLoc(lpos[0], lpos[0], lpos[1], lpos[1], lpos[2], lpos[2]);
			break;
		}
		case 'g':{
			lpos[2] += 2.0f;
			light_emitter->changeLoc(lpos[0], lpos[0], lpos[1], lpos[1], lpos[2], lpos[2]);
			break;
		}
		case 'f':{
			lpos[0] -= 2.0f;
			light_emitter->changeLoc(lpos[0], lpos[0], lpos[1], lpos[1], lpos[2], lpos[2]);
			break;
		}
		case 'h': {
			lpos[0] += 2.0f;
			light_emitter->changeLoc(lpos[0], lpos[0], lpos[1], lpos[1], lpos[2], lpos[2]);
			break;
		}
		case ',':{
			for (int i = 0; i < SNOWFLAKE_NUM; i++) {
				vec3 temp_acc = snowflower_emitter->p[i]->acc;
				snowflower_emitter->p[i]->acc.set(temp_acc.x - 1.0f / 10000, temp_acc.y, temp_acc.z);
			}
			break;
		}
		case '.': {
			for (int i = 0; i < SNOWFLAKE_NUM; i++) {
				vec3 temp_acc = snowflower_emitter->p[i]->acc;
				snowflower_emitter->p[i]->acc.set(temp_acc.x + 1.0f / 10000, temp_acc.y, temp_acc.z);
			}
			break;
		}
		case '-': {
			light_quadratic += 0.00005f;
			break;
		}
		case '=': {
			if (light_quadratic>0)
				light_quadratic -= 0.00005f;
			break;
		}
	}
	outCollisionTest(eye, center, -skySizeX / 2, skySizeX / 2, -skySizeZ / 2, skySizeZ / 2);
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

	

	snowflower_emitter = new Emitter(SNOWFLAKE_NUM, -1.0f*skySizeX / 2.0f, 1.0f*skySizeX / 2.0f, 1.0f*skySizeY / 2.0f, 1.0f*skySizeY / 2.0f, -1.0f*skySizeZ / 2.0f, 1.0f*skySizeZ / 2.0f);
	snowflower_emitter->emit(init_snowflower_partical, snowflower_dead);
	smoke_emitter = new Emitter(SMOKE_NUM, -31.0f, -28.0f, -1.0f*skySizeY / 2.0f + 28.0f, -1.0f*skySizeY / 2.0f + 28.0f, -1.0f*skySizeZ / 2.0f + 67.5f, -1.0f*skySizeZ / 2.0f + 69.5f);
	smoke_emitter->emit(init_smoke_partical, smoke_dead);
	light_emitter = new Emitter(LIGHT_NUM, lpos[0],lpos[0], lpos[1], lpos[1], lpos[2], lpos[2]);
	light_emitter->emit(init_light_partical, light_dead);
}


void ProcessMenu(int value)
{
	switch (value)
	{
	case 0:
		smoke_stop = true;
		break;
	case 1:
		smoke_stop = false;
		break;

	case 2:
		snow_stop = false;
		break;
	case 3:
		snow_stop = false;
		break;
	case 4:
		for (int i = 0; i < SNOWFLAKE_NUM; i++)
			snowflower_emitter->p[i]->setTexture(texture[3]);
		break;
	case 5:
		for (int i = 0; i < SNOWFLAKE_NUM; i++)
			snowflower_emitter->p[i]->setTexture(texture[2]);
		break;
	}

	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);        
	windowHandler = glutCreateWindow("粒子系统漫游");              
	glutDisplayFunc(renderScene);            
	glutReshapeFunc(reshape);  
	glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);
	glutKeyboardFunc(myKeyboard);
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("停止烟雾效果", 0);
	glutAddMenuEntry("开始烟雾效果", 1);
	glutAddMenuEntry("停止下雪", 2);
	glutAddMenuEntry("开始下雪", 3);
	glutAddMenuEntry("下雨", 4);
	glutAddMenuEntry("下雪", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	setupRC();
	glutMainLoop();
	system("pause");
	return 0;
}