// primitives2d.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "../rectangle.hpp"

using namespace std;
using namespace tmath;
using namespace tmath::p2d;

typedef v2<float>	v2f;
typedef v2<int>	v2i;
//typedef rectangle<float, false>	rectanglefVAL;
//typedef rectangle<float, true>	rectanglefREF;
typedef rectangle<float, PRectangleStoreByValue>	rectanglef;
typedef rectangle<int, PRectangleStoreByValue>	rectanglei;

typedef rectangle<float, PRectangleStoreByPointer>	rectanglefPointer;
typedef rectangle<float, PRectangleStoreByReference>	rectanglefReference;

void rectangle_test()
{
	v2f va0(0,10), va1(10,0);
	v2f vb0, vb1;


	rectanglef t0(va0, va1);
	rectanglei ti0(v2i(0,10), v2i(10,0));
	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
	cout<<"t0 center: "<<t0.center().str(2)<<endl<<endl;
	cout<<"t0 widht, height: "<<t0.w()<<", "<<t0.h()<<endl<<endl;
	
//	cout<<ti0.str(2)<<" Area: "<<ti0.area()<<endl<<endl;
//	cout<<"ti0 center: "<<ti0.center().str(2)<<endl<<endl;
	
	v2f vf0(-2,3);
	cout<<vf0.str(2)<<" inside: "<<t0.point_inside(vf0)<<endl<<endl;
	v2f vf1(10,10.01);
	cout<<vf1.str(2)<<" inside: "<<t0.point_inside(vf1)<<endl<<endl;

	
	
	
//	rectanglefReference t2Ref;		// Should yield error about default constructor not supported
//	rectanglefPointer t3Ref(t0Ref);	// Should yield error about copy constructor not supported
	
	
//	cout<<" Area t1: "<<t1.area()<<endl<<endl;
//	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
//	char c;
//	cin>>c;
}

