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
	bool has_tex;//纹理或颜色 

	vec3 place;//位置
	vec3 size;//大小
	vec3 speed;// 速度
	vec3 acc;//加速度 
	vec3 angle;//角度
	vec3 color;//颜色

	unsigned int texture;//纹理  
	float life;//生命 
	friend class Emitter;

	Particle();

	Particle(vec3 size, vec3 speed, vec3 acc, vec3 angle, float _life, unsigned int _texture);

	const vec3& getPlace();

	//有纹理绘制单个粒子
	void draw(GLuint texture);
	//无纹理绘制单个粒子
	void draw();
	//根据位置，角度绘制粒子
	void show();
	//更新粒子速度，位置，并重绘粒子
	void update();
	void setTexture(GLuint texture);

	

};

class Emitter {
	public:
	float x1, y1, x2, y2, z1, z2; //发射器位置  
	int speed;//发射速率  
	bool isInfinite;
	//int num;
	Particle **p;//发射粒子  
	//初始化粒子的函数指针，设置粒子运动速度，方向，加速度
	Particle* (*initFunc)();
	bool(*deadFunc)(Particle*);

	Emitter(int _speed, float _x1, float _x2, float _y1, float _y2, float _z1, float _z2);

	void changeLoc(float _x1, float _x2, float _y1, float _y2, float _z1, float _z2);

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
	
};


