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


//��������Ƶ�������
void Particle::draw(GLuint texture)
{
	//û��mtl�ļ�ʹ�õĲ���
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
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
//��������Ƶ�������
void Particle::draw()
{
	//û��mtl�ļ�ʹ�õĲ���
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

//����λ�ã��ǶȻ�������
void Particle::show()
{
	//��Ȼ�����ֻ�������Ӱ�͸��
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

//���������ٶȣ�λ��
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

//���������������������޸�������
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
//���޸�������
void Emitter::emit(Particle* (init)())
{
	isInfinite = false;
	for (int i = 0; i < speed; i++) {
		initFunc  = init;
		p[i] = initFunc();
		int place_x = rand() % speed;
		int place_y = rand() % speed;
		int place_z = rand() % speed;
		//���Բ�ֵ������������ӳ�ʼλ��
		p[i]->place.x = 1.0f*place_x / speed*(x2 - x1) + x1;
		p[i]->place.y = 1.0f*place_y / speed*(y2 - y1) + y1;
		p[i]->place.z = 1.0f*place_z / speed*(z2 - z1) + z1;
	}
}

//�������ӣ���û������������ʾ���ӣ����޸������䣩
void Emitter::update()
{
	for (int i = 0; i < speed; i++) {
		if (!dead[i]) {
			p[i]->update();
			//p[i]->show();
		}
	}
}
//��������,��ʾ����
void Emitter::show()
{
	for (int i = 0; i < speed; i++) {
			p[i]->show();
	}
}
//�������ӣ��������ӣ���������ɾȥ���ӣ����³�ʼ���µ����ӣ����޸������䣩
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

//�������Ӷ���ʼ��
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



