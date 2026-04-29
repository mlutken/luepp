// primitives2d.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "../circle_origo.hpp"

using namespace std;
using namespace tmath;
using namespace tmath::p2d;

typedef v2<float>	v2f;
typedef v2<int>		v2i;
typedef circle_origo<float>	circle_origof;
typedef circle_origo<int>	circle_origoi;


void circle_origo_test()
{
	v2f va0(0,10), va1(10,0);
	v2f vb0, vb1;

	cout<<"circle_origo test !!!"<<endl<<endl;
//	circle_origof t0(va0, 3.0f);
	circle_origof t0( 3.0f );
//	circlei ti0(v2i(0,10), 3);
	cout<<t0.str(2)<<endl<<endl;
	
//	cout<<ti0.str(2)<<" Area: "<<ti0.area()<<endl<<endl;
//	cout<<"ti0 center: "<<ti0.center().str(2)<<endl<<endl;
	
	v2f vf0(1,2.5f);
	cout<<vf0.str(2)<<" inside: "<<t0.point_inside(vf0)<<endl<<endl;
	v2f vf1(1,3);
	cout<<vf1.str(2)<<" inside: "<<t0.point_inside(vf1)<<endl<<endl;

	
	
	
//	circlefReference t2Ref;		// Should yield error about default constructor not supported
//	circlefPointer t3Ref(t0Ref);	// Should yield error about copy constructor not supported
	
	
//	cout<<" Area t1: "<<t1.area()<<endl<<endl;
//	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
//	char c;
//	cin>>c;
}

