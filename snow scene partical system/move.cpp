#include "move.h"

#define PI 3.1416f


void outCollisionTest(float* eye, float* center, float x1, float x2, float z1, float z2)
{

	if (x1 < 0)x1 += 2;
	else x1 -= 2;

	if (x2 < 0)x2 += 2;
	else x2 -= 2;

	if (z1 < 0)z1 += 2;
	else z1 -= 2;

	if (z2 < 0)z2 += 2;
	else z2 -= 2;

	if (center[0] < x1) {
		center[0] = x1;
	}

	if (center[0] > x2) {
		center[0] = x2;
	}
	if (center[2] < z1) {
		center[2] = z1;
	}
	if (center[2] > z2) {
		center[2] = z2;
	}

	float distance = sqrt((eye[0] - center[0])*(eye[0] - center[0]) +
		(eye[2] - center[2])*(eye[2] - center[2]));

	if (distance <= 2.0f) {
		eye[0] = 2.0f*(eye[0] - center[0]) / distance + center[0];
		eye[2] = 2.0f*(eye[2] - center[2]) / distance + center[2];
	}
	bool flag = false;
	if (eye[0] < x1) {
		flag = true;
		eye[0] = x1;
	}
	if (eye[0] > x2) {
		flag = true;
		eye[0] = x2;
	}
	if (eye[2] < z1) {
		flag = true;
		eye[2] = z1;
	}
	if (eye[2] > z2) {
		flag = true;
		eye[2] = z2;
	}
	if (flag) {
		distance = sqrt((eye[0] - center[0])*(eye[0] - center[0]) +
			(eye[2] - center[2])*(eye[2] - center[2]));

		if (distance <= 2.0f) {
			center[0] = 2.0f*(center[0] - eye[0]) / distance + eye[0];
			center[2] = 2.0f*(center[2] - eye[2]) / distance + eye[2];
		}
	}
	return;
}


void move(float* eye, float* center, direction dir)
{
	const float d = 1.0f;
	float x1, x2, y1, y2;
	x1 = eye[0], y1 = eye[2], x2 = center[0], y2 = center[2];
	float len = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	if (x2 - x1<0.1f&&x2 - x1>-0.1f)x2 = x1;
	if (y2 - y1 < 0.1f&&y2 - y1>-0.1f)y2 = y1;
	switch (dir) {
	case front: {
		eye[0] = d*(x2 - x1) / len + x1;
		eye[2] = d*(y2 - y1) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case back: {
		eye[0] = d*(x1 - x2) / len + x1;
		eye[2] = d*(y1 - y2) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case left: {
		eye[0] = d*(y2 - y1) / len + x1;
		eye[2] = d*(x1 - x2) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;
		break;
	}
	case right: {
		eye[0] = d*(y1 - y2) / len + x1;
		eye[2] = d*(x2 - x1) / len + y1;
		center[0] = eye[0] + x2 - x1;
		center[2] = eye[2] + y2 - y1;

		break;
	}
	default:break;
	}
	return;
}


void rotate(float* eye, float* center, direction dir)
{
	const float alpha = 1.0f / (10 * PI);
	float x1, x2, y1, y2;
	x1 = eye[0], y1 = eye[2], x2 = center[0], y2 = center[2];
	switch (dir) {
	case left: {
		center[0] = x1 + (x2 - x1)*cos(alpha) + (y2 - y1)*sin(alpha);
		center[2] = y1 + (y2 - y1)*cos(alpha) - (x2 - x1)*sin(alpha);
		break;
	}
	case right: {
		center[0] = x1 + (x2 - x1)*cos(alpha) - (y2 - y1)*sin(alpha);
		center[2] = y1 + (y2 - y1)*cos(alpha) + (x2 - x1)*sin(alpha);
		break;
	}
	default:break;
	}
}