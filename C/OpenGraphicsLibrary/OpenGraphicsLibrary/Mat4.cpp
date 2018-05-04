#include "Mat4.h"
//column type matrix....

mat4::mat4()
{
	//matrix[row + column*4]
	matrix[0 + 0 * 4] = 1;
	matrix[1 + 1 * 4] = 1;
	matrix[2 + 2 * 4] = 1;
	matrix[3 + 3 * 4] = 1;
}

mat4::mat4(float f)
{
	//matrix[row + column*4]
	matrix[0 + 0 * 4] = f;
	matrix[1 + 1 * 4] = f;
	matrix[2 + 2 * 4] = f;
	matrix[3 + 3 * 4] = f;
}