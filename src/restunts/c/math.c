#include "externs.h"
#include "shape3d.h"

extern struct RECTANGLE select_rect_rc;
extern struct MATRIX mat_z_rot;
extern struct MATRIX mat_x_rot;
extern struct MATRIX mat_y_rot;
extern struct MATRIX mat_rot_temp;
extern unsigned mat_y_rot_angle;
extern long sin80, cos80;

short sintab[] = {
	0, 101, 201, 302, 402, 503, 603, 704, 804, 904, 1005, 1105, 1205, 1306, 1406, 1506, 1606, 1706, 1806, 1906, 2006, 2105, 2205, 2305, 2404, 2503, 2603, 2702, 2801, 2900, 2999, 3098, 3196, 3295, 3393, 3492, 3590, 3688, 3786, 3883, 3981, 4078, 4176, 4273, 4370, 4467, 4563, 4660, 4756, 4852, 4948, 5044, 5139, 5235, 5330, 5425, 5520, 5614, 5708, 5803, 5897, 5990, 6084, 6177, 6270, 6363, 6455, 6547, 6639, 6731, 6823, 6914, 7005, 7096, 7186, 7276, 7366, 7456, 7545, 7635, 7723, 7812, 7900, 7988, 8076, 8163, 8250, 8337, 8423, 8509, 8595, 8680, 8765, 8850, 8935, 9019, 9102, 9186, 9269, 9352, 9434, 9516, 9598, 9679, 9760, 9841, 9921, 10001, 10080, 10159, 10238, 10316, 10394, 10471, 10549, 10625, 10702, 10778, 10853, 10928, 11003, 11077, 11151, 11224, 11297, 11370, 11442, 11514, 11585, 11656, 11727, 11797, 11866, 11935, 12004, 12072, 12140, 12207, 12274, 12340, 12406, 12472, 12537, 12601, 12665, 12729, 12792, 12854, 12916, 12978, 13039, 13100, 13160, 13219, 13279, 13337, 13395, 13453, 13510, 13567, 13623, 13678, 13733, 13788, 13842, 13896, 13949, 14001, 14053, 14104, 14155, 14206, 14256, 14305, 14354, 14402, 14449, 14497, 14543, 14589, 14635, 14680, 14724, 14768, 14811, 14854, 14896, 14937, 14978, 15019, 15059, 15098, 15137, 15175, 15213, 15250, 15286, 15322, 15357, 15392, 15426, 15460, 15493, 15525, 15557, 15588, 15619, 15649, 15679, 15707, 15736, 15763, 15791, 15817, 15843, 15868, 15893, 15917, 15941, 15964, 15986, 16008, 16029, 16049, 16069, 16088, 16107, 16125, 16143, 16160, 16176, 16192, 16207, 16221, 16235, 16248, 16261, 16273, 16284, 16295, 16305, 16315, 16324, 16332, 16340, 16347, 16353, 16359, 16364, 16369, 16373, 16376, 16379, 16381, 16383, 16384, 16384
};

extern unsigned char atantable[];

short sin_fast(unsigned short s) {
	unsigned char c = s & 0xFF;
	switch ((s >> 8) & 3) {
		case 0:
			return sintab[c];
		case 1:
			return sintab[0x100 - c];
		case 2:
			return -sintab[c];
		case 3:
			return -sintab[0x100 - c];
	}
}

short cos_fast(unsigned short s) {
	return sin_fast(s + 0x100);
}

int polarAngle(int z, int y) {
	
	unsigned flag;
	int temp, result;
	unsigned long index;
	
	flag = 0;
	
	if (z < 0) {
		flag |= 4;
		z = -z;
	}
	
	if (y < 0) {
		flag |= 2;
		y = -y;
	}
	
	if (z == y) {
		if (z == 0) return result; // orig code has undefined return value here as well!
		result = 0x80;
	} else {
		if (z > y) {
			temp = z;
			z = y;
			y = temp;
			flag |= 1;
		}
		index = (((unsigned long)z << 16) / y);
		if ((index & 0xFF) >= 0x80) // round upwards
			index += 0x100;
		result = atantable[index >> 8];
	}
	
	switch (flag) {
		case 0:
			return result;
		case 1:
			return -result + 0x100;
		case 2:
			return -result + 0x200;
		case 3:
			return result + 0x100;
		case 4:
			return -result;
		case 5:
			return result - 0x100;
		case 6:
			return result - 0x200;
		case 7:
			return -(result + 0x100);
	}
	
}

int polarRadius2D(int z, int y) {
	long result;
	
	result = polarAngle(z, y);
	
	if (result < 0) {
		result = -result;
	}
	
	if (result >= 0x100) {
		result = -(result - 0x200);
	}
	
	if (result <= 0x80) {
		result = cos_fast(result);
		if (y < 0)
			y = -y;
		return (((unsigned long)y) << 14) / result;
	} else {
		result = sin_fast(result);
		if (z < 0)
			z = -z;
		return (((unsigned long)z) << 14) / result;
	}
}

int polarRadius3D(struct VECTOR* vec) {
	return polarRadius2D( polarRadius2D(vec->x, vec->y), vec->z );
}

unsigned rect_compare_point(struct POINT2D* pt) {
	char flag;
	if (pt->py < select_rect_rc.x2)
		flag = 1;
	else if (pt->py > select_rect_rc.y2)
		flag = 2; 
	else
		flag = 0;
	
	if (pt->px < select_rect_rc.x1)
		flag |= 4;
	else if (pt->px > select_rect_rc.y1)
		flag |= 8;
	return flag;
}

void mat_mul_vector(struct VECTOR* invec, struct MATRIX* mat, struct VECTOR* outvec) {

	if (mat->m._11 != 0 && invec->x != 0)
		outvec->x = ((long)mat->m._11 * invec->x) >> 14;
	else
		outvec->x = 0;

	if (mat->m._12 != 0 && invec->y != 0)
		outvec->x += ((long)mat->m._12 * invec->y) >> 14;

	if (mat->m._13 != 0 && invec->z != 0)
		outvec->x += ((long)mat->m._13 * invec->z) >> 14;


	if (mat->m._21 != 0 && invec->x != 0)
		outvec->y = ((long)mat->m._21 * invec->x) >> 14;
	else
		outvec->y = 0;

	if (mat->m._22 != 0 && invec->y != 0)
		outvec->y += ((long)mat->m._22 * invec->y) >> 14;

	if (mat->m._23 != 0 && invec->z != 0)
		outvec->y += ((long)mat->m._23 * invec->z) >> 14;


	if (mat->m._31 != 0 && invec->x != 0)
		outvec->z = ((long)mat->m._31 * invec->x) >> 14;
	else
		outvec->z = 0;

	if (mat->m._32 != 0 && invec->y != 0)
		outvec->z += ((long)mat->m._32 * invec->y) >> 14;

	if (mat->m._33 != 0 && invec->z != 0)
		outvec->z += ((long)mat->m._33 * invec->z) >> 14;

}

void mat_multiply(struct MATRIX* rmat, struct MATRIX* lmat, struct MATRIX* outmat) {
	int counter;
	int* rmatvals = rmat->vals;
	int* lmatvals = lmat->vals;
	int* outmatvals = outmat->vals;
	
	counter = 9;
	while (counter > 0) {
		if (rmatvals[0] != 0 && lmatvals[0] != 0)
			outmatvals[0] = ((long)rmatvals[0] * lmatvals[0]) >> 14; else
			outmatvals[0] = 0;

		if (rmatvals[1] != 0 && lmatvals[3] != 0)
			outmatvals[0] += ((long)rmatvals[1] * lmatvals[3]) >> 14;

		if (rmatvals[2] != 0 && lmatvals[6] != 0)
			outmatvals[0] += ((long)rmatvals[2] * lmatvals[6]) >> 14;
		
		outmatvals++;
		if (counter != 7 && counter != 4) {
			lmatvals++;
		} else {
			lmatvals -= 2;
			rmatvals += 3;
		}
		counter--;
	}
	
}

void mat_invert(struct MATRIX* inmat, struct MATRIX* outmat) {
	int temp;
	if (inmat == outmat) {
		temp = outmat->m._21;
		outmat->m._21 = outmat->m._12;
		outmat->m._12 = temp;

		temp = outmat->m._31;
		outmat->m._31 = outmat->m._13;
		outmat->m._13 = temp;

		temp = outmat->m._32;
		outmat->m._32 = outmat->m._23;
		outmat->m._23 = temp;
	} else {
		outmat->m._11 = inmat->m._11;
		outmat->m._12 = inmat->m._21;
		outmat->m._13 = inmat->m._31;

		outmat->m._21 = inmat->m._12;
		outmat->m._22 = inmat->m._22;
		outmat->m._23 = inmat->m._32;

		outmat->m._31 = inmat->m._13;
		outmat->m._32 = inmat->m._23;
		outmat->m._33 = inmat->m._33;
	}
}


void mat_rot_x(struct MATRIX* outmat, int angle) {
	int c, s;
	
	c = cos_fast(angle);
	s = sin_fast(angle);
	outmat->m._11 = 0x4000;
	outmat->m._21 = 0;
	outmat->m._31 = 0;
	outmat->m._12 = 0;
	outmat->m._22 = c;
	outmat->m._32 = s;
	outmat->m._13 = 0;
	outmat->m._23 = -s;
	outmat->m._33 = c;
}

void mat_rot_y(struct MATRIX* outmat, int angle) {
	int c, s;
	
	c = cos_fast(angle);
	s = sin_fast(angle);
	outmat->m._11 = c;
	outmat->m._21 = 0;
	outmat->m._31 = -s;
	outmat->m._12 = 0;
	outmat->m._22 = 0x4000;
	outmat->m._32 = 0;
	outmat->m._13 = s;
	outmat->m._23 = 0;
	outmat->m._33 = c;
}

void mat_rot_z(struct MATRIX* outmat, int angle) {
	int c, s;
	
	c = cos_fast(angle);
	s = sin_fast(angle);
	outmat->m._11 = c;
	outmat->m._21 = s;
	outmat->m._31 = 0;
	outmat->m._12 = -s;
	outmat->m._22 = c;
	outmat->m._32 = 0;
	outmat->m._13 = 0;
	outmat->m._23 = 0;
	outmat->m._33 = 0x4000;
}

// mat_rot_zxy was originally optimized, using pre-calced y-matrices and only 
// multiplying the non-zero axes. currently not optimized except for the y cache:

struct MATRIX* mat_rot_zxy(int z, int x, int y, int unk) {
	int flag;
	
	mat_rot_z(&mat_z_rot, z);
	mat_rot_x(&mat_x_rot, x);
	
	// y rotation matrix cache
	/*if (mat_y_rot_angle != y) {
		mat_rot_y(&mat_y_rot, y);
		mat_y_rot_angle = y;
	}*/
	mat_y_rot_angle = y; // dont forget this!!
	mat_rot_y(&mat_y_rot, y);
	
	if ((unk & 1) != 0) {
		mat_multiply(&mat_y_rot, &mat_x_rot, &mat_rot_temp);
		mat_multiply(&mat_rot_temp, &mat_z_rot, &mat_x_rot);
		return &mat_x_rot;
	} else {
		mat_multiply(&mat_z_rot, &mat_x_rot, &mat_rot_temp);
		mat_multiply(&mat_rot_temp, &mat_y_rot, &mat_z_rot);
		return &mat_z_rot;
	}
}

void rect_adjust_from_point(struct POINT2D* pt, struct RECTANGLE* rc) {
	int temp;
	
	if (rc->x1 > pt->px) {
		rc->x1 = pt->px;
	}
	
	temp = pt->px + 1;
	if (rc->y1 < temp) {
		rc->y1 = temp;
	}
	
	if (rc->x2 > pt->py) {
		rc->x2 = pt->py;
	}
	
	temp = pt->py + 1;
	if (rc->y2 < temp) {
		rc->y2 = temp;
	}
}

int vector_op_unk2(struct VECTOR* vec) {
	long y;
	long temp;
	int flag;
	int result;
	long angle;
	
	y = abs(vec->y);
	
	temp = polarRadius2D(abs(vec->x), abs(vec->z));
	
	if (sin80 != cos80) {
		//fatal_error("sin80 != cos80 - not observed");
		y = y * sin80;
		temp = temp * cos80;
	} 

	if (temp >= y) {
		flag = 0;
	} else {
		flag = 1;
	}
	
	if (vec->y < 0) {
		if (flag != 0) return 0x1E;
	} else
	if (vec->y > 0) {
		if (flag != 0) return 0x1F;
	}

	if (vec->y > 0) {
		result = 0x0F;
	} else {
		result = 0;
	}
	
	angle = -polarAngle(vec->z, -vec->x);
	if (angle < 0) {
		angle += 0x400;
	}
	
	result += (((angle << 4) - angle) >> 10);
	
	return result;
}

extern int projectiondata5, projectiondata8, projectiondata9, projectiondata10;


void vector_to_point(struct VECTOR* vec, struct POINT2D* outpt) {

	// the original code checks for several overflows (in a strange way) - this code does not, but seems to do well anyway

	long proj;
	long comp;
	
	if (vec->z <= 0) {
		outpt->px = 0x8000;
		outpt->py = 0x8000;
		return;
	}
	
	if (vec->x < 0) {
		proj = (long)-vec->x * projectiondata9;
		comp = (proj >> 16) << 1;
		if (proj & 0xFFFF == 0) {
			fatal_error("%li  %i", proj, comp);
			comp++;
		}

		if (vec->z > comp) { 
			outpt->px = -(proj / vec->z) + projectiondata5;
		} else
			outpt->px = -0x7D00;
	} else {
		proj = (long)vec->x * projectiondata9;
		comp = (proj >> 16) << 1;
		if (proj & 0xFFFF == 0) {
			fatal_error("%l  %i", proj, comp);
			comp++;
		}

		if (vec->z > comp) 
			outpt->px = (proj / vec->z) + projectiondata5;
		else
			outpt->px = 0x7D00;
	}

	if (vec->y < 0) {
		proj = (long)-vec->y * projectiondata10;
		comp = (proj >> 16) << 1;
		if (proj & 0xFFFF == 0) {
			fatal_error("%l  %i", proj, comp);
			comp++;
		}

		if (vec->z > comp) 
			outpt->py = (proj / vec->z) + projectiondata8;
		else
			outpt->py = 0x7D00;
	} else {
		proj = (long)vec->y * projectiondata10;
		comp = (proj >> 16) << 1;
		if (proj & 0xFFFF == 0) {
			fatal_error("%l  %i", proj, comp);
			comp++;
		}

		if (vec->z > comp) 
			outpt->py = -(proj / vec->z) + projectiondata8;
		else
			outpt->py = -0x7D00;
	}
}

void vector_op_unk(struct VECTOR* vec1, struct VECTOR* vec2, struct VECTOR* outvec, int i) {
	
	long var_4, var_2;
	
	outvec->z = i;

	var_4 = outvec->z - vec2->z;
	var_2 = vec1->z - vec2->z;
	if (var_2 < 0) {
		var_4 = var_4 >> 1;
		var_2 = var_2 >> 1;
	}
	
	outvec->x = (vec1->x - vec2->x) * var_4 / var_2 + vec2->x;
	outvec->y = (vec1->y - vec2->y) * var_4 / var_2 + vec2->y;
}
