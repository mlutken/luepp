#include "../v3.hpp"
#include "../m33.hpp"

#include <iostream>
#include <time.h>

#include "../lue_math.h"
#include "../utils.hpp"
#include "../stdmath.hpp"
#include "../c.hpp"

using namespace gmath;
using namespace std;


typedef gmath::v3<float> vec3;
typedef gmath::m33<float> mat33;

typedef gmath::v3<double> vec3d;
typedef gmath::m33<double> mat33d;


void test_v3()
{
	// casting stuff
	vec3 vf1(2,2,2), vf2;
	vec3d vd1(3,3,3), vd2;
	vf2 = vf1 + vd1;
	vf2 = vf1 + static_cast<vec3>(vd1);
	bool b1 = (vf1 < static_cast<vec3>(vd1));
	bool b2 = (vf1 < vd1);
	// casting stuff, end
	
	vec3 v0(2,2,2), v1(3,3,3), v2;

	printf("v0: %s\n", v0.str(0).c_str());
	printf("v1: %s\n", v1.str(0).c_str());
	v2 = vec3::V_X + vec3::V_Y - vec3::V_Y*3;
	printf("v2: %s\n", v2.str(0).c_str());
	v2 = -v2; printf("v2 = -v2: %s\n", v2.str(0).c_str());
	v2 += v0; printf("v2 += v0: %s\n", v2.str(0).c_str());
	v2 -= vec3::V_Y; printf("v2 -= V_Y: %s\n", v2.str(0).c_str());
	v2 *= 3; printf("v2 *= 3: %s\n", v2.str(0).c_str());
	v2 /= 3; printf("v2 /= 3: %s\n", v2.str(0).c_str());
	printf("v0*v1: %f\n", v0*v1);
	printf("v0 - vec3::V_Y: %s\n", (v0 - vec3::V_Y).str().c_str());
	printf("vec3::V_Y - v0: %s\n", (vec3::V_Y - v0).str().c_str());
	printf("vec3::V_Y - vec3::V_X: %s\n", (vec3::V_Y - vec3::V_X).str().c_str());
	printf("v0==v1: %d\n", v0==v1);
	printf("v0!=v1: %d\n", v0!=v1);
	printf("v0==vec3(2,2,2): %d\n", v0<=vec3(2,2,2));
	printf("v0<=vec3(2,2,2): %d\n", v0==vec3(2,2,2));
	printf("v0<v1: %d\n", v0<v1);
	printf("v0>v1: %d\n", v0>v1);
	printf("v1.equal_exact(vec3::V_Y): %d\n", v1.equal_exact(vec3::V_Y));
	printf("v1==vec3::V_Y: %d\n", v1==vec3::V_Y);
	printf("vec3::V_X==vec3::V_Y: %d\n", vec3::V_X==vec3::V_Y);
	printf("v1.len2(): %.1f\n", v1.len2());
	printf("v1.len(): %.1f\n", v1.len());
	printf("v0.dist(v1): %.1f\n", v0.dist(v1));
	printf("v0.dist2(v1): %.1f\n", v0.dist2(v1));
	printf("v0.angle(v1): %.1f\n", v0.angle(v1)*C<float>::TODEG());
	printf("V_X.angle(V_Y): %.1f\n", vec3::V_X.angle(vec3::V_Y)*C<float>::TODEG());
	printf("v0.normalize(): %.1f\n", v0.normalize());
	printf("v0 normalized: %s\n", v0.str().c_str());
	printf("v0.len(): %.1f\n", v0.len());
	printf("v1.set_len(4): %.1f\n", v1.set_len(4));
	printf("v1 set_len 4: %s\n", v1.str().c_str());
	printf("v1.len(): %.1f\n", v1.len());
	
	v2.set(5,5,5);
	v2.x() = 3.0f;
	v2.y() = 3.0f;

	printf("v2: %s\n", v2.str(2).c_str());
	printf("v2.x(): %.2f,  v2.y(): %.2f\n", v2.x(), v2.y());

	v1.set(3.1f, 3.0f, 3.0f);
	printf("v1: %s\n", v1.str(2).c_str());
	printf("v2: %s\n", v2.str(2).c_str());

//	printf("v1 == v2\n");
	printf("3.0f == v2.x()\n");
	if ( 3.0f == v2.x() )
	{
		printf("Equal\n");
	}
	else
	{
		printf("NOT Equal\n");
	}

	// Scale vector with scalar
	v1 = v0*3.f;
	v1 = 3.4f*v0;
	printf("v1.dist(v0): %.1f\n", v1.dist(v0));


	// equal epsilon test
	v0.set(2.f, 2.f, 2.0001f);
	v1.set(2.f, 2.f, 2.f);
	printf("v0 == v1: %d\n", v0 == v1);
	printf("v0.equal_exact(v1): %d\n", v0.equal_exact(v1));
	printf("v0.equal_epsilon(v1, 0.0): %d\n", v0.equal_epsilon(v1, 0.0f));
	printf("v0.equal_epsilon(v1, 0.1): %d\n", v0.equal_epsilon(v1, 0.1f));
};


void test_m33()
{
	// casting stuff
	mat33 mf1(2,2,3,4,2,2,3,4,7), mf2;
	mat33d md1(3,3,3,5,3,3,3,5,8), md2;
	mf2 = mf1 + md1;
	mf2 = mf1 + md1;
	bool b1 = (mf1 == static_cast<mat33>(md1));
	bool b2 = (mf1 == md1);
	// casting stuff, end

	mat33 m0=mat33::M_I, m1, m2;
	vec3 v0(2,3,4);


	m1 = 3.f*m2;

	m1.set(1,-2,-3, 2,1,-2, 2,3,1);
	printf("m0:\n%s\n", m0.str(0).c_str());
	printf("m1:\n%s\n", m1.str(0).c_str());
	printf("m0+m1:\n%s\n", (m0+m1).str(0).c_str());
	printf("m0*m1:\n%s\n", (m0*m1).str(0).c_str());
	printf("M_I*m0*m1:\n%s\n", (mat33::M_I*m0*m1).str(0).c_str());
	m1.transpose();
	printf("m1 transposed:\n%s\n", m1.str(0).c_str());
	printf("2.f*(m0+m1)-10*m0*2.f:\n%s\n", (2.f*(m0+m1)-10.f*m0*2.f).str(0).c_str());
	printf("m1.xcol():%s\n", m1.xcol().str(0).c_str());
	printf("m1.ycol():%s\n", m1.ycol().str(0).c_str());
	printf("m1[Y][Y]: %.1f\n", m1[Y][Y]);
	
	m2.set(vec3::V_X, vec3::V_X, vec3::V_X);
	printf("m2:\n%s\n", m2.str(0).c_str());
//	m2 *= m0;
	printf("m2:\n%s\n", m2.str(0).c_str());

	printf("\n-----------------\n");
	m1.transpose();
	printf("m1:\n%s\n", m1.str(0).c_str());
	printf("v0:%s\n", v0.str(0).c_str());

	printf("m1*v0:%s\n", (m1*v0).str(0).c_str());
	printf("v0*m1:%s\n", (v0*m1).str(0).c_str());
	printf("m1.det():%.2f\n", m1.det());
	printf("m1.get_inverse():\n%s\n", m1.get_inverse().str(2).c_str());

	m1.set_xrow(vec3(3,4,5));
	m1.set_yrow(vec3(30,40,50));
	printf("m1:\n%s\n", m1.str(0).c_str());
	printf("m1.get_xrow():%s\n", m1.get_xrow().str(0).c_str());

	printf("M_I:\n%s\n", (mat33::M_I+mat33::M_ZERO).str(0).c_str());
	cout<<m1<<endl;

	m0.set(3,7,2,4,3,7,2,4,9);
	mat33 m3(m0);
	v0.set(3,5,7);
	printf("m0*v0:\n%s\n", (m0*v0).str(0).c_str());
	printf("v0*m0:\n%s\n", (v0*m0).str(0).c_str());

	printf("m0:\n%s\n", m0.str(0).c_str());
	printf("m0.get_inverse():\n%s\n", m0.get_inverse().str(2).c_str());
	printf("m0.adjoint():\n%s\n", m0.adjoint().str(2).c_str());
	printf("m0.get_transpose():\n%s\n", m0.get_transpose().str(0).c_str());
	m3.transpose();
	printf("m3.transpose():\n%s\n", m3.str(0).c_str());

	// acces elements
	printf("m3.xcol().y(): %f\n", m3.xcol().y());
	m3.xcol().y() = 10.f;
	printf("m3.xcol().y(): %f\n", m3.xcol().y());

	printf("m3[Y][X]: %f\n", m3[Y][X]);
	m3[Y][X] = 99.f;
	printf("m3[Y][X]: %f\n", m3[Y][X]);


};

