#include "partical_emitter.h"  



Particle::Particle() {}

Particle::Particle(vec _size, vec _speed, vec _acc, vec _angle,
	float _life, unsigned int _texture)
{
	size = _size;
	speed = _speed;
	acc = _acc;
	life = _life;
	texture = _texture;
	angle = _angle;
	has_tex = true;
}

Particle::Particle(vec _size, vec _speed, vec _acc, vec _angle,
	float _life, vec _color)
{
	size = _size;
	speed = _speed;
	acc = _acc;
	life = _life;
	color = _color;
	angle = _angle;
	has_tex = false;
}


//有纹理绘制单个粒子
void Particle::draw(GLuint texture)
{
	//没有mtl文件使用的材质
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
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
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//无纹理绘制单个粒子
void Particle::draw()
{
	//没有mtl文件使用的材质
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 0.7f };
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	for (int i = 0; i < 4; i++) {
		glVertex2fv(point[i]);
	}
	glEnd();
}

//根据位置，角度绘制粒子
void Particle::show()
{
	//深度缓冲区只读，粒子半透明
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(place.x, place.y, place.z);
	glRotatef(angle.x, 1, 0, 0);
	glRotatef(angle.y, 0, 1, 0);
	glRotatef(angle.z, 0, 0, 1);
	glScalef(size.x, size.y, size.z);
	if (has_tex)
		draw(texture);
	else
		draw();
	glPopMatrix();
	glDepthMask(GL_TRUE);
}

//更新粒子速度，位置
void Particle::update()
{
		place.y += speed.y;
		place.x += speed.x;
		place.z += speed.z;

		speed.y += acc.y;
		speed.x += acc.x;
		speed.z += acc.z;
		life -= 0.2f;
}


const vec& Particle::getPlace()
{
	return place;
}

const vec& Particle::getSize()
{
	return size;
}

const vec& Particle::getAngle()
{
	return angle;
}


Emitter::Emitter(int _speed, float _x1,
	float _x2, float _y1, float _y2, float _z1, float _z2) {
	speed = _speed;
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
	z1 = _z1;
	z2 = _z2;
	p = new Particle*[speed];
	dead = new bool[speed];
	for (int i = 0; i < speed; i++) {
		dead[i] = false;
	}
	//num = speed;
}

//设置粒子死亡函数，无限个数发射
void Emitter::emit(Particle* (init)(), bool(*deadF)(Particle*)) {
	isInfinite = true;
	for (int i = 0; i < speed; i++) {
		initFunc = init;
		deadFunc = deadF;
		p[i] = initFunc();
		int place_x = rand() % speed;
		int place_y = rand() % speed;
		int place_z = rand() % speed;
		p[i]->place.x = 1.0f*place_x / speed*(x2 - x1) + x1;
		p[i]->place.y = 1.0f*place_y / speed*(y2 - y1) + y1;
		p[i]->place.z = 1.0f*place_z / speed*(z2 - z1) + z1;
	}
}
//有限个数发射
void Emitter::emit(Particle* (init)())
{
	isInfinite = false;
	for (int i = 0; i < speed; i++) {
		initFunc  = init;
		p[i] = initFunc();
		int place_x = rand() % speed;
		int place_y = rand() % speed;
		int place_z = rand() % speed;
		//线性插值，随机决定粒子初始位置
		p[i]->place.x = 1.0f*place_x / speed*(x2 - x1) + x1;
		p[i]->place.y = 1.0f*place_y / speed*(y2 - y1) + y1;
		p[i]->place.z = 1.0f*place_z / speed*(z2 - z1) + z1;
	}
}

//遍历粒子，若没有死亡，则显示粒子（有限个数发射）
void Emitter::update()
{
	for (int i = 0; i < speed; i++) {
		if (!dead[i]) {
			p[i]->update();
			//p[i]->show();
		}
	}
}
//遍历粒子,显示粒子
void Emitter::show()
{
	for (int i = 0; i < speed; i++) {
			p[i]->show();
	}
}
//遍历粒子，更新粒子，若死亡，删去粒子，重新初始化新的粒子（无限个数发射）
void Emitter::infinite_update() {
	for (int i = 0; i < speed; i++) {
		p[i]->update();
		//p[i]->show();
		if (p[i]->life < 0 && isInfinite || deadFunc(p[i])) {
			delete p[i];
			p[i] = initFunc();
			int place_x = rand() % speed;
			int place_y = rand() % speed;
			int place_z = rand() % speed;
			p[i]->place.x = 1.0f*place_x / speed*(x2 - x1) + x1;
			p[i]->place.y = 1.0f*place_y / speed*(y2 - y1) + y1;
			p[i]->place.z = 1.0f*place_z / speed*(z2 - z1) + z1;
		}
	}
}

//所有粒子都初始化
void Emitter::reset()
{
	for (int i = 0; i < speed; i++) {
		dead[i] = false;
	}
	for (int i = 0; i < speed; i++) {
		p[i] = initFunc();
		int place_x = rand() % speed;
		int place_y = rand() % speed;
		int place_z = rand() % speed;
		p[i]->place.x = 1.0f*place_x / speed*(x2 - x1) + x1;
		p[i]->place.y = 1.0f*place_y / speed*(y2 - y1) + y1;
		p[i]->place.z = 1.0f*place_z / speed*(z2 - z1) + z1;
	}
	//num = speed;
}



