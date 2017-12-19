#pragma once
#include <cstdlib>
#include <GL/glut.h>

struct vec
{
	float x;
	float y;
	float z;
	vec::vec() { }
	vec::vec(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) { }
	vec::vec(float* v)
	{
		x = v[0], y = v[1], z = v[2];
	}
};

class Particle {
	bool has_tex;//�������ɫ 

	vec place;//λ��
	vec size;//��С
	vec speed;// �ٶ�
	vec acc;//���ٶ� 
	vec angle;//�Ƕ�
	vec color;//��ɫ

	unsigned int texture;//����  
	float life;//���� 
	friend class Emitter;
	//��������Ƶ�������
	void draw(GLuint texture);
	//��������Ƶ�������
	void draw();
	//����λ�ã��ǶȻ�������
	void show();
	//���������ٶȣ�λ�ã����ػ�����
	void update();
public:
	Particle();
	const vec& getPlace();
	const vec& getSize();
	const vec& getAngle();
	Particle(vec size, vec speed, vec acc, vec angle,
		float _life, unsigned int _texture);

	Particle(vec size, vec speed, vec acc, vec angle,
		float _life, vec _color);

};

class Emitter {
	float x1, y1, x2, y2, z1, z2; //������λ��  
	int speed;//��������  
	bool isInfinite;
	//int num;
	Particle **p;//��������  
	bool *dead;
	//��ʼ�����ӵĺ���ָ�룬���������˶��ٶȣ����򣬼��ٶ�
	Particle* (*initFunc)();
	bool(*deadFunc)(Particle*);

public:
	//���������������������޸�������
	void emit(Particle* (init)(), bool(*deadF)(Particle*));
	//���޸�������
	void emit(Particle* (init)());
	void show();
	//�������ӣ���û������������ʾ���ӣ����޸������䣩
	void update();
	//�������ӣ��������ӣ���������ɾȥ���ӣ����³�ʼ���µ����ӣ����޸������䣩
	void infinite_update();
	//�������Ӷ���ʼ��
	void reset();
	
	Emitter(int _speed, float _x1,
		float _x2, float _y1, float _y2, float _z1, float _z2);
};


