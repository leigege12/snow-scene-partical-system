#pragma once
#include <cstdlib>
#include <GL/glut.h>

struct vec3
{
	float x;
	float y;
	float z;
	vec3::vec3() { }
	vec3::vec3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) { }
	vec3::vec3(float* v)
	{
		x = v[0], y = v[1], z = v[2];
	}
	void vec3::set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

class Particle {
public:
	bool has_tex;//�������ɫ 

	vec3 place;//λ��
	vec3 size;//��С
	vec3 speed;// �ٶ�
	vec3 acc;//���ٶ� 
	vec3 angle;//�Ƕ�
	vec3 color;//��ɫ

	unsigned int texture;//����  
	float life;//���� 
	friend class Emitter;

	Particle();

	Particle(vec3 size, vec3 speed, vec3 acc, vec3 angle, float _life, unsigned int _texture);

	const vec3& getPlace();

	//��������Ƶ�������
	void draw(GLuint texture);
	//��������Ƶ�������
	void draw();
	//����λ�ã��ǶȻ�������
	void show();
	//���������ٶȣ�λ�ã����ػ�����
	void update();
	void setTexture(GLuint texture);

	

};

class Emitter {
	public:
	float x1, y1, x2, y2, z1, z2; //������λ��  
	int speed;//��������  
	bool isInfinite;
	//int num;
	Particle **p;//��������  
	//��ʼ�����ӵĺ���ָ�룬���������˶��ٶȣ����򣬼��ٶ�
	Particle* (*initFunc)();
	bool(*deadFunc)(Particle*);

	Emitter(int _speed, float _x1, float _x2, float _y1, float _y2, float _z1, float _z2);

	void changeLoc(float _x1, float _x2, float _y1, float _y2, float _z1, float _z2);

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
	
};


