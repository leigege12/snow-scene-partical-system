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
const int skySizeY = 30;
const int skySizeZ = 200;
GLfloat lpos[4] = { 17.0f, 10.0f, -23.5f, 1.0f };
GLfloat lAmb[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lDif[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat white_light[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lSpe[4] = { 0.0f, 0.0f, 1.0f, 1.0f };



GLuint texture[10];
// ���� ��ʾ�б�
GLint houseList = 0;
// ���� ��ʾ�б�
GLint baseList = 0;
//����      
float whRatio;
int wHeight = 0;
int wWidth = 0;


//�ӵ�      
float center[] = { 0, 0, 0 };
float eye[] = { 0, 0, 50 };
GLfloat dx = 0, dy = 0, dz = 0;
GLfloat ax = 0, ay = 0, az = 0;
GLint mx = 0, my = 0;
GLint MouseDown = 0;
float ratio = 180;

Emitter *snowflower_emitter;

//����obj
std::map<std::string, Object> housemap;
std:: set<std::string> housename;
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
	float acc[] = { 1.0f*(rand() % 3 - 1) / 2000,-4.9 / 4000 ,1.0f*(rand() % 3 - 1) / 90000 };
	float angle[] = { rand() % 360, rand() % 360 ,rand() % 360 };
	Particle* p = new Particle(vec(size, size, size), vec(speed), vec(acc),
		vec(angle), rand() % 100, texture[2]);
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

void drawRect(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //ѡ������texture[status]       
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	int dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

int a = 0;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����Ȼ���     
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //��ʼ������Ϊ��λ����          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	glTranslatef(dx, dy, dz);
	glRotatef(ax, 1.0f, 0.0f, 0.0f);
	glRotatef(ay, 0.0f, 1.0f, 0.0f);
	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	//glRotatef(30, 0.0f, 1.0f, 0.0f);
	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	glInitNames();//��ʼ������  
	glPushName(0);//��ʼ������ջ  
	glPushMatrix();
	glTranslatef(17.0f, 10.0f, -23.5f);
	glRotatef(a, 1.0f, 1.0f, 0.0f);
	//glColor3f(0.5f, 0.0f, 0.0f);//Red
	glutSolidSphere(1,20,20);

	glPopMatrix();

	
	glCallList(houseList);

	glCallList(baseList);
				  
	//��
	glLoadName(1);//��������  
	glPushMatrix();
	glTranslatef(-17.0f, -9.0f, -23.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(10, 11, 1);
	drawRect(texture[0]);
	glPopMatrix();
	//��
	glLoadName(2);//��������  
	glPushMatrix();
	glTranslatef(-30.0f, -6.0f, -23.5f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(7, 7, 1);
	drawRect(texture[1]);
	glPopMatrix();
	

	glPopName();

	//�ذ�
	/*
	glPushMatrix();
	glTranslatef(0.0f, -1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawBottom(texture[1]);
	glPopMatrix();
	*/
	

	//�컨��
	/*
	glPushMatrix();
	glTranslatef(0.0f, 1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawTop(texture[1]);
	glPopMatrix();
	*/
	
	/*
	//ǽ�ڣ�ǰ��
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*skySizeZ / 2.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[1], 1, 2);
	glPopMatrix();

	//ǽ�ڣ���
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*skySizeZ / 2.0f);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[1], 1, 0);
	glPopMatrix();

	//ǽ�ڣ���
	glPushMatrix();
	glTranslatef(-1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[1], 1, 1);
	glPopMatrix();

	//ǽ�ڣ��ң�
	glPushMatrix();
	glTranslatef(1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[1], 1, 3);
	glPopMatrix();
	*/
	
	//	int t = timer->GetTime();
	glEnable(GL_BLEND);
	//��ʼ��������
	/*
	if (isSnow) {
		snowflag = true;
		t++;
		if (t % 10 == 0 && t != last_t) {
			e1->update();
			last_t = t;
			if (t == 100) {
				t = 0;
				last_t = 0;
			}
		}
		else {
			e1->show();
		}
	}
	else if (snowflag) {
		if (t >= 10) {
			e1->fade();
			s->remove();
			t = 0;
		}
		e1->show();
		t++;
	}
	s->show();
	*/
	glPushMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, lAmb);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	snowflower_emitter->show();
	glPopMatrix();
	
	glDisable(GL_BLEND);	
	glPopMatrix();
	glutSwapBuffers();//����������      

}

void timeFunc(int value) {
		a += 10;
		snowflower_emitter->infinite_update();
		//printf("frame time\n" );
		glutPostRedisplay();
		glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);

}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰ           
	glLoadIdentity();   //��ʼ������Ϊ��λ����              
	whRatio = (GLfloat)width / (GLfloat)height;  //������ʾ����             
	gluPerspective(45, whRatio, 1, 400); //͸��ͶӰ  
	glMatrixMode(GL_MODELVIEW);  //���þ���ģʽΪģ��        
}


void reshape(int width,int height) {
	if (height == 0) { //����߶�Ϊ0     
		height = 1;   //�ø߶�Ϊ1��������ַ�ĸΪ0������          
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth); //�����ӽ�        
}


void myMouse(int button, int state, int x, int y) {

}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q': {exit(0); break; } //�˳�  

	case 'a': { //����  
		move(eye, center, direction::left);
		break;
	}
	case 'd': { //����  
		move(eye, center, direction::right);
		break;
	}
	case 'w': { //ǰ��  
		move(eye, center, direction::front);
		break;
	}
	case 's': {  //����  
		move(eye, center, direction::back);
		break;
	}

	case 'j': {//�ӽ�����  
		rotate(eye, center, direction::left);
		break;
	}
	case 'l': {//�ӽ�����  
		rotate(eye, center, direction::right);
		break;
	}
	case 'i': {//�ӽ�����  
		center[1] += 0.5f;
		break;
	}
	case 'k': {//�ӽ�����  
		center[1] -= 0.5f;
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
	glTranslatef(-25.0f, -1.0f*skySizeY / 2.0f+28.0f, -1.0f*skySizeZ / 2.0f + 58.0f);

	glScalef(1.2f, 0.5f, 0.5f);
	loadObj(basename, basemap, base_matname);
	glPopMatrix();

	glEndList();
	return lid;
}

void setupRC() {
	srand(unsigned(time(NULL)));
	glClearColor(1.0, 0.0, 0.0, 0.0);
	//ƽ������Ч��
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_LIGHTING);
	//Set up light
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lSpe);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.08);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.003);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	readObj(house, "obj\\house.obj", housemap, housename, house_matname);
	readObj(house, "obj\\base.obj", basemap, basename, base_matname);
	houseList = GenTableList();
	baseList = houseList + 1;
	buildTexture("door.jpg", texture[0]);
	buildTexture("window.jpg", texture[1]);
	buildTexture("snowflower.jpg", texture[2]);
	//BuildTexture("background.jpg", texture[1]);
	//BuildTexture("spark.bmp", texture[2]);
	//BuildTexture("packet.jpg", texture[3]);
	//BuildTexture("door.jpg", texture[4]);
	//BuildTexture("Butterfly1.bmp", texture[5]);
	//BuildTexture("Butterfly2.bmp", texture[6]);
	//BuildTexture("Butterfly3.bmp", texture[7]);

	//s = new snow(texture[0]);

	snowflower_emitter = new Emitter(7000, -1.0f*skySizeX/2.0f, 1.0f*skySizeX / 2.0f, 15, 15, -1.0f*skySizeZ / 2.0f, 1.0f*skySizeZ / 2.0f);
	snowflower_emitter->emit(init_snowflower_partical, snowflower_dead);

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
	glutInit(&argc, argv);//��glut�ĳ�ʼ��             
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//��ʼ����ʾģʽ:RGB��ɫģ�ͣ���Ȳ��ԣ�˫����               
	glutInitWindowSize(800, 600);//���ô��ڴ�С             
	windowHandler = glutCreateWindow("Partical System Display");//���ô��ڱ���               
	glutDisplayFunc(renderScene); //ע����ƻص�����             
	glutReshapeFunc(reshape);   //ע���ػ�ص�����    
	glutTimerFunc(FRAMETIME * 1000, timeFunc, 1);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	//glutIdleFunc(idleFunc);//ע��ȫ�ֻص�����������ʱ����           
	setupRC();
	glutMainLoop();  // glut�¼�����ѭ��           
	system("pause");
	return 0;
}