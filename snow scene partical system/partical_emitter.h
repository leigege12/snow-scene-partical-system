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
	bool has_tex;//纹理或颜色 

	vec place;//位置
	vec size;//大小
	vec speed;// 速度
	vec acc;//加速度 
	vec angle;//角度
	vec color;//颜色

	unsigned int texture;//纹理  
	float life;//生命 
	friend class Emitter;
	//有纹理绘制单个粒子
	void draw(GLuint texture);
	//无纹理绘制单个粒子
	void draw();
	//根据位置，角度绘制粒子
	void show();
	//更新粒子速度，位置，并重绘粒子
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
	float x1, y1, x2, y2, z1, z2; //发射器位置  
	int speed;//发射速率  
	bool isInfinite;
	//int num;
	Particle **p;//发射粒子  
	bool *dead;
	//初始化粒子的函数指针，设置粒子运动速度，方向，加速度
	Particle* (*initFunc)();
	bool(*deadFunc)(Particle*);

public:
	//设置粒子死亡函数，无限个数发射
	void emit(Particle* (init)(), bool(*deadF)(Particle*));
	//有限个数发射
	void emit(Particle* (init)());
	void show();
	//遍历粒子，若没有死亡，则显示粒子（有限个数发射）
	void update();
	//遍历粒子，更新粒子，若死亡，删去粒子，重新初始化新的粒子（无限个数发射）
	void infinite_update();
	//所有粒子都初始化
	void reset();
	
	Emitter(int _speed, float _x1,
		float _x2, float _y1, float _y2, float _z1, float _z2);
};


