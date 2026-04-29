// primitives2d.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "../triangle.hpp"

using namespace std;
using namespace tmath;
using namespace tmath::p2d;

typedef v2<float>	v2f;
//typedef triangle<float, false>	trianglefVAL;
//typedef triangle<float, true>	trianglefREF;
typedef triangle<float, PTriangleStoreByValue>	trianglef;
typedef triangle<int, PTriangleStoreByValue>	trianglei;

typedef triangle<float, PTriangleStoreByPointer>	trianglefPointer;
typedef triangle<float, PTriangleStoreByReference>	trianglefReference;

void triangle_test()
{
	v2f va0(0,0), va1(5,0), va2(0,5);
	v2f vb0, vb1, vb2;
	cout<<va1.str(2)<<endl<<endl;

	trianglef tri;
	
	trianglefPointer t0Ptr(&va0, &va1, &va2);
	trianglefReference t0Ref(va0, va1, va2);
	trianglefReference t1Ref(vb0, vb1, vb2);

	trianglef	t0(va0, va1, va2);
	trianglef	t1(vb0, vb1, vb2);
//	trianglef	t0(&va0, &va1, &va2);
//	trianglef	t1(&vb0, &vb1, &vb2);
	
//	cout<<t0Ref.str(2)<<" Area: "<<t0Ref.area()<<endl<<endl;
	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
	t1 = t0 + v2f(2,2);
	cout<<t1.str(2)<<" Area: "<<t1.area()<<endl<<endl;

	t0 = t0 + v2f(-2,-2);
	t0 += v2f(2,2);
	t1 = t0;
	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
	cout<<t1.str(2)<<" Area: "<<t1.area()<<endl<<endl;
	trianglef	t2(t1);
	cout<<t2.str(2)<<" Area: "<<t2.area()<<endl<<endl;
	va2.y(10);
	cout<<t0Ptr.str(2)<<" Area: "<<t0Ptr.area()<<endl<<endl;
	t0Ptr = t0 + v2f(0,0);
	cout<<t0Ptr.str(2)<<" Area: "<<t0Ptr.area()<<endl<<endl;
	trianglefPointer t1Ptr(&vb0, &vb1, &vb2);
	t1Ptr = t0Ptr;
	t0Ptr = t0Ptr + v2f(0,0);
	cout<<t0Ptr.str(2)<<" Area: "<<t0Ptr.area()<<endl<<endl;
	cout<<t0Ptr<<endl<<endl;
	cout<<t1Ptr.str(2)<<" Area: "<<t1Ptr.area()<<endl<<endl;
	cout<<"Equal (t0Ptr == t1Ptr): "<<(t0Ptr == t1Ptr)<<endl<<endl;
	cout<<t0Ref.str(2)<<" Area: "<<t0Ref.area()<<endl<<endl;
	t0Ref = (triangle<float>)(t0);
	t0Ref = t0;
	cout<<t0Ref.str(2)<<" Area: "<<t0Ref.area()<<endl<<endl;
	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
	
	cout<<t0Ref.str(2)<<" Area: "<<t0Ref.area()<<endl<<endl;
	va2.y(20.567f);
	cout<<t0Ref.str(2)<<" Area: "<<t0Ref.area()<<endl<<endl;

	trianglei	ti;
	ti = t0Ref;		// assigning from float-triangle to int-triangle
	cout<<ti.str(2)<<" Area: "<<ti.area()<<endl<<endl;
	
	t0.v0().x() = 235.345;
	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
	trianglei	ti1(t0);
	cout<<ti1.str(2)<<" Area: "<<ti1.area()<<endl<<endl;
			
//	trianglefReference t2Ref;		// Should yield error about default constructor not supported
//	trianglefPointer t3Ref(t0Ref);	// Should yield error about copy constructor not supported
	
	
//	cout<<" Area t1: "<<t1.area()<<endl<<endl;
//	cout<<t0.str(2)<<" Area: "<<t0.area()<<endl<<endl;
//	char c;
//	cin>>c;
}

