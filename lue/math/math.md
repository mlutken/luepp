/** \dir
!!! GMath geometry math library

GMath is a C++ templated math library mainly for geometry applications like
2D, 3D vector graphics and computergames. It's the ambition to be part of the boost libraries
at some point.
You can browse the preliminary documentation on these pages. Use the navigation on the right.
Project is hosted at sourceforge under CPAF:
https://cpaf.svn.sourceforge.net/svnroot/cpaf/trunk/cpaf/cpaf_libs/math/ 


!! Features /Ambitions
- 2D, 3D, 4D vectors and matrices
- quaternion and special 4x3 matrix that acts as a 4x4
- Geomtry classes for lines, boxes, triangles, spheres etc. in 2D and 3D
- Intersection functions for all the geometry classes
- Good documentation
- Possiblity to implement platform specific versions of the critical functions.
- Perhaps use expression templates to speed up expressions like: va+vb + ma*vd + mb*ve ...

! Storage policies for primitives
For primitives like eg. a triangle which is defined by 3 points, we want it 
possible to select between different storage policies so that the points can be stored
by value, or by reference or pointers to the actual points. This is both to be able
to preserve space avoiding copies of allready existing points and also refering by 
pointers or reference should make the constructor faster. 


!! Status (january 2006)
Generally all of the classes and API needs review by others than me.

Right now a status saying Beta means that the class is working with at least 
allmost all functions implemented ... but more functions might be added.
Also naming, platform optimizations, expression templates for 
#va+vb + ma*vd + mb*ve ...# like expressions are not implemented.

I have no religious opinions about the names chosen other that they should 
conform as much as possible with existing STD/STL/Boost naming conventions.

Also there is yet not a scheme to make platform independent optimizations, allthough I 
have given the subject some thoughts. I would be happy to discuss solutions!

! Primitives
The work on the primitives and associated functions for intersections etc. 
are at a relatively early stage, but 2D triangle, rectangle, box, cirle are more 
or less working including storage policies. i believe we should concentrate on 
building the 2D primitives and functions first before moving to 3D. One because 
it is a bit simpler, two because we want the class and function names to be consistent 
and three because we might need some of the 2D primitives in the functions operating 
on 3D primitives.

||Name : Description||Status||Personnel
| #Base# | | 
| v2 : 2D vector | Beta | Martin Lutken
| m22 : 2D matrix | Beta | Martin Lutken
| g33 : 3D matrix bottom row = 0,0,1. For 2D transforms.| Beta | Martin Lutken
| v3 : 3D vector | Beta | Martin Lutken
| m33 : 3D matrix | Beta | Martin Lutken
| g44 : 4D matrix bottom row = 0,0,0,1. For 3D transforms.| Beta | Martin Lutken
| v4 : 4D vector | 50% | Martin Lutken
| m44 : 4D matrix | 50%| Martin Lutken
| quat : Quaternion (merge boost quaternion ??)| 0% | ?
| oct : Octonion (merge boost octonion ??)| 0% | ?
| #Primitives 2D# | | 
| line_2p : Line (2D) defined by 2 points. | 5% | ?
| line_pv : Line (2D) defined by a point and a (direction) vector. | 5% | ?
| rectangle : Rectangle (2D) defined by two points. | 70% | ML, ?
| triangle :  | % | ML, ?
| box :  | % | ML, ?
| box_origo :  | % | ML, ?
| circle :  | % | ML, ?
| circle_origo :  | % | ML, ?
| ellipse :  | % | ?
| ellipse_origo :  | % | ?
| #Primitives 3D# | | 
| line_2p : Line (3D) defined by 2 points. | ... | ?



January 2006!

*/



