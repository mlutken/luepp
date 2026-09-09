#pragma once

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <math/base/lue_math.h>
#include <math/base/utils.hpp>
#include <math/base/stdmath.hpp>

#include <math/base/v2.hpp>
#include <math/base/v3.hpp>
#include <math/base/m22.hpp>


namespace lue::math {



/**
Specialized 3x3 matrix with bottom row allways 0,0,1.
Used in computer 2D graphics XX more XX.
The g33 matrix is internally represented as a m22 matrix and a v2 vector.
As the bottom row of the 3x3 matrix is allways  0 0 1
Matrix layout:
\pre
    column, row

    m0,0	m1,0	m2,0	=	mx,x	my,x	v,x
    m0,1	m1,1	m2,1	=	mx,y	my,y	v,y
     0		 0		 1		=	 0		 0		 1
\endpre
*/
template <typename T>
class g33
{
public:

    // ----------------
    // --- Typedefs ---
    // ----------------
    typedef g33<T>					value_type;
    typedef value_type&				reference;
    typedef const value_type&		const_reference;
    typedef value_type*				pointer;
    typedef const value_type*		const_pointer;
    typedef size_t			index_type;

    // --- vector typedefs ---
    typedef v3<T>					v3_value_type;
    typedef v3_value_type&			v3_reference;
    typedef const v3_value_type&	v3_const_reference;

    // --- vector typedefs ---
    typedef math::v2<T>			v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    // --- m22 typedefs ---
    typedef math::m22<T>			m22_value_type;
    typedef m22_value_type&			m22_reference;
    typedef const m22_value_type&	m22_const_reference;

    // ----------------------------
    // --- Const Static Members ---
    // ----------------------------
    static const value_type M_ZERO;		///< Constant zero matrix.
    static const value_type M_I;		///< Identity matrix.



public:

    // --------------------------------------------
    // --- Constructors / Set values functions. ---
    // --------------------------------------------

    /** Default constructor with no initialization. */
    g33() {}

    /** Copy constructor. Initializing with a general m33<T>.*/
    template <typename T1>
    g33(const g33<T1>& m)
        : 	m_m22(static_cast<m22_value_type>(m.m_m22)),
            m_v2(static_cast<v2_value_type>(m.m_v2)) {}

    /** Constructor with single value initialization. */
    explicit g33(const T fVal) 			///< [in] All elements are set to this value.
        :	m_m22(fVal),
            m_v2(fVal) {}


    /** Constructor with m22 and v2 initialization. */
    g33(m22_const_reference m,		///< [in] m2  matrix
        v2_const_reference v)		///< [in] v2 vector
        : 	m_m22(m),
            m_v2(v) {}


    /** Constructor with 6 values initialization. */
    g33	(	const T fxx,			///< [in] Column-X, X component (m22[x,x])
            const T fxy,			///< [in] Column-X, Y component (m22[x,y])
            const T fyx,			///< [in] Column-Y, X component (m22[y,x])
            const T fyy,			///< [in] Column-Y, Y component (m22[y,y])
            const T fzx,			///< [in] Column-Z, X component (v2[x])
            const T fzy)			///< [in] Column-Z, Y component (v2[y])
        :	m_m22(fxx, fxy, fyx, fyy),
            m_v2(fzx, fzy) {}

    // ----------------------------
    // --- Set values functions ---
    // ----------------------------

    /** Set all fields to same value. */
    void				set(const T fVal)				///< [in] All elements are set to this value.
    {
        m_m22.set(fVal);
        m_v2.set(fVal);
    }

    /** Set values with m22 and v2 . */
    void				set(m22_const_reference m,		///< [in] m2  matrix
                            v2_const_reference v)		///< [in] v2 vector
    {
        m_m22 	= m;
        m_v2 	= v;
    }

    /** Set values with 6 numbers. */
    void				set(const T fxx,	///< [in] Column-X, X component (m22[x,x])
                            const T fxy,	///< [in] Column-X, Y component (m22[x,y])
                            const T fyx,	///< [in] Column-Y, X component (m22[y,x])
                            const T fyy,	///< [in] Column-Y, Y component (m22[y,y])
                            const T fzx,	///< [in] Column-Z, X component (v2[x])
                            const T fzy)	///< [in] Column-Z, Y component (v2[y])
    {
        m_m22.set(fxx, fxy, fyx, fyy);
        m_v2.set(fzx, fzy);
    }

    // -----------------------
    // --- Access elements ---
    // -----------------------

    /** [] Operator. Acces individual columns of the matrix.*/
    v2_reference		operator[](index_type i)	///< [in] Index value. Should be in range 0..2 ~ X, Y, Z.
    {
        if 	 ( i > Y) 	return m_v2;
        else 			return m_m22[i];
    }

    v2_const_reference	operator[](index_type i) const
    {
        if 	 ( i > Y) 	return m_v2;
        else 			return m_m22[i];
    }

    /** Access column X. Use this function to get acces to the X-column of the matrix.
        \return Reference to coloumn X as a vector. */
    v2_reference		xcol()			{ return m_m22[X]; }
    v2_const_reference	xcol()	const	{ return m_m22[X]; }

    /** Access column Y. Use this function to get acces to the Y-column of the matrix.
        \return Reference to coloumn Y as a vector. */
    v2_reference		ycol()			{ return m_m22[Y]; }
    v2_const_reference	ycol()	const	{ return m_m22[Y]; }

    /** Access column Z. Use this function to get acces to the Z-column of the matrix.
        \return Reference to coloumn Z as a vector. */
    v2_reference		zcol()			{ return m_v2; }
    v2_const_reference	zcol()	const	{ return m_v2; }

    /** Access column Z. Use this function to get acces to the Z-column of the matrix.
        \return Reference to coloumn Z as a vector. */
    m22_reference		m22()			{ return m_m22; }
    m22_const_reference	m22()	const	{ return m_m22; }


    // ------------------------------------
    // --- Simple Set And Get Functions ---
    // ------------------------------------

    /** Get copy of X-row. Use this function to get a copy of the X-row of the matrix.
        \return Copy of row X as a vector. */
    v3_value_type		get_xrow()	const
    {
        return v3_value_type(xcol().x(), ycol().x(), zcol().x());
    }

    /** Get copy of Y-row. Use this function to get a copy of the Y-row of the matrix.
        \return Copy of row Y as a vector. */
    v3_value_type		get_yrow()	const
    {
        return v3_value_type(xcol().y(), ycol().y(), zcol().y());
    }

    /** Get copy of Z-row. Use this function to get a copy of the Z-row of the matrix.
        \note This function allways return [0 0 1].
        \return Copy of row Z as a vector. Allways [0 0 1]. */
    v3_value_type		get_zrow()	const
    {
        return v3_value_type(T(0), T(0), T(1));
    }

    /// Set X-row to vector. Assigns input vector to X-row of matrix.
    void				set_xrow(v3_const_reference v)		///< [in] Vector to assign to X-row
    {
        xcol().x() = v.x();
        ycol().x() = v.y();
        zcol().x() = v.z();
    }

    /// Set Y-row to vector. Assigns input vector to Y-row of matrix.
    void				set_yrow(v3_const_reference v)		///< [in] Vector to assign to Y-row
    {
        xcol().y() = v.x();
        ycol().y() = v.y();
        zcol().y() = v.z();
    }

    /// Set Z-row to vector. Assigns input vector to Z-row of matrix.
    /// This function is a no-op since the buttom row is fixed to [0 0 1]
    void				set_zrow(v3_const_reference v)		///< [in] Vector to assign to Z-row
    { }

    // ----------------------------
    // --- Assignment operators ---
    // ----------------------------

    /** '=' operator, Assignment from another g33.*/
    reference			operator =(const_reference m)		///< [in] Right hand side operand.
    {
        m_m22 = m.m_m22;
        m_v2  = m.m_v2;
        return *this;
    }

    // ----------------------------
    // --- Misc. math operators ---
    // ----------------------------

    /** Unary '-' operator, Negate matrix.
        \return Matrix holding the result of the negation. */
    value_type		operator -() const
    {
        return value_type(-m_m22, -m_v2);
    }

    /** '/=' operator, Divides the elements of the vector with a scalar.
        \return Reference to "this".*/
    reference			operator /=(const T fVal)		///< [in] Right hand side operand. Dividend used to scale the vector.
    {
        T fV = T(1) / fVal;
        m_m22 *= fV;
        m_v2  *= fV;
        return *this;
    }

    // ------------------------------------
    // --- Add/Substract math operators ---
    // ------------------------------------

    /** '+=' operator, Add matrix with another.
        \return Reference to "this".*/
    reference			operator +=(const_reference m)		///< [in] Right hand side operand.
    {
        m_m22 += m.m_m22;
        m_v2  += m.m_v2;
        return *this;
    }


    /** '-=' operator, Substract a matrix from this matrix.
        \return Reference to "this".*/
    reference			operator -=(const_reference m)		///< [in] Right hand side operand.
    {
        m_m22 -= m.m_m22;
        m_v2  -= m.m_v2;
        return *this;
    }

    /** '+' operator, Matrix - Matrix addition. g33 + g33.
        \return Matrix holding the result of the addition. */
    value_type		operator +(	const_reference m		///< [in] Right hand side operand (g33).
                              )const
    {
        return value_type(
            m_m22 + m.m_m22,
            m_v2  + m.m_v2 );
    }

    /** '-' operator, Matrix - Matrix substraction. g33 - g33.
        \return Matrix holding the result of the substraction. */
    value_type		operator -(	const_reference m		///< [in] Right hand side operand (g33).
                              )const
    {
        return value_type(
            m_m22 - m.m_m22,
            m_v2  - m.m_v2 );
    }


    // -------------------------------------
    // --- Multiplication math operators ---
    // -------------------------------------

    /** '*=' operator, Scales the matrix with a scalar. (I.e. multiplying the
        elements with a scalar.)
        \return Reference to "this".*/
    reference			operator *=(const T fVal)		///< [in] Right hand side operand. Scalar used to scale the vector.
    {
        m_m22 *= fVal;
        m_v2  *= fVal;
        return *this;
    }

    /** '*' operator, Matrix - Scalar multiplication.
        Scales matrix with a scalar, like this: matrix * number.
        \return Matrix holding the result of the scaling. */
    value_type		operator *(const T fVal			///< [in] Right hand side operand scalar (T).
                              )const
    {
        return value_type( m_m22*fVal, m_v2*fVal );
    }


    /** '*' operator. Matrix - Vector multiplication. Performs g33 * v2.

        #About doing the opposite transformation when no scaling:#\n
        Remember that for an orthonormal matrix (pure rotation in upper 2x2)
        the transposed matrix is identical to the inverse. So in cases where
        the 2x2 upper matrix is pure rotation we can do the reverse translation
        of $ M * V $ as $ M.get_tranpose() * V $ which is faster than the general
        $ M.get_inverse() * V $. To further optimize this case a special member
        function transpose_mult() is provided which does the same as:
        $ M.get_inverse() * V$. So in short (when upper 2x2 is pure rotation):
        \code
            M.get_inverse() * V == M.get_transpose() * V == M.transpose_mult(V)
        \endcode
        \see transpose_mult()
        \return A 2D vector holding the result. */
    v2_value_type	operator *(	v2_const_reference v)	///< [in] Right hand side operand (v2).
    {
        return v2_value_type(	xcol().x()*v.x() + ycol().x()*v.y() + zcol().x(),
                                xcol().y()*v.x() + ycol().y()*v.y() + zcol().y() );
    }


    /** Matrix tranposed multiplied with vector. Faster way of doing
        $ M.get_transpose() * V $.

        Remember that for an orthonormal matrix (pure rotation in upper 2x2)
        the transposed matrix is identical to the inverse. So in cases where
        the 2x2 upper matrix is pure rotation we can do the reverse translation
        of $ M * V $ as $ M.get_tranpose() * V $ which is faster than the general
        $ M.get_inverse() * V $. To further optimize this case a special member
        function transpose_mult() is provided whic does the same as:
        $ M.get_inverse() * V$. So in short (when upper 2x2 is pure rotation):
        \code
            M.get_inverse() * V == M.get_transpose() * V == M.transpose_mult(V)
        \endcode
        \return A 2D vector holding the result. */
    v2_value_type	transpose_mult(	v2_const_reference v)	///< [in] Right hand side operand (v2).
    {
        return v2_value_type(	xcol().x()*v.x() + xcol().y()*v.y() - zcol().x(),
                                ycol().x()*v.x() + ycol().y()*v.y() - zcol().y() );
    }



    /** '*' operator. Matrix - Matrix multiplication. g33 * g33. */
    value_type		operator *(const_reference m 	///< [in] Right hand side operand (g33).
                              ) const
    {
        return value_type(
            xcol().x() * m.xcol().x() + ycol().x() * m.xcol().y(),
            xcol().y() * m.xcol().x() + ycol().y() * m.xcol().y(),

            xcol().x() * m.ycol().x() + ycol().x() * m.ycol().y(),
            xcol().y() * m.ycol().x() + ycol().y() * m.ycol().y(),

            xcol().x() * m.zcol().x() + ycol().x() * m.zcol().y() + zcol().x(),
            xcol().y() * m.zcol().x() + ycol().y() * m.zcol().y() + zcol().y()
            );
    }


    // ----------------------
    // --- Util functions ---
    // ----------------------

    /// Sets all elements to zero.
    void			reset()
    {
        *this = M_ZERO;
    }


    /// Sets the matrix to the identity matrix.
    void			load_identity()
    {
        *this = M_I;
    }

    /** Sets the matrix to a diagonal matrix using the elements specified.
        The rest of the elements are set to zero. */
    void			load_diagonal(	const T xx,		///< [in] xx component
                                    const T yy)		///< [in] yy component
    {
        set(xx, T(0), T(0), yy, T(0), T(0));
    }


    /** Sets the matrix to a diagonal matrix using the vector specified.
        The rest of the elements are set to zero. */
    void			load_diagonal(v2_const_reference v)		///< [in] Vector to assign to diagonal
    {
        set(v.x(), T(0), T(0), v.y(), T(0), T(0));
    }

    /** Transposes the matrix. Matrix is transposed in a special way since.
        this matrix is intended for use with homogeneous coordinates.
        We transpose the matrix by transposing the 2x2 matrix and negating
        the 2D vector representing the translation.

        Remember that for an
        orthonormal matrix (ie. a matrix where all coloumns have lenght 1 and
        perpendicular to each other) the transposed matrix is identical to the
        inverse. This is a much faster way to calculate the inverse and the
        preferred way in computer graphics where matrices are often pure rotation/
        orientation matrices, which are orthonormal. So as long as the 2x2 matrix
        is a pure rotation matrix with no scaling, then we can use the tranposed
        instead of the inverse.
        \see get_transpose(), inverse(), get_inverse(). */
    void				transpose()
    {
        m_m22.transpose();
        m_v2 = -m_v2;
    }

    /** Returns the transpose of the matrix. Matrix is transposed in a special way since.
        this matrix is intended for use with homogeneous coordinates.
        We transpose the matrix by transposing the 2x2 matrix and negating
        the 2D vector representing the translation.

        Remember that for an
        orthonormal matrix (ie. a matrix where all coloumns have lenght 1 and
        perpendicular to each other) the transposed matrix is identical to the
        inverse. This is a much faster way to calculate the inverse and the
        preferred wat in computer graphics where matrices are often pure rotation/
        orientation matrices which are orthonormal. So as long as the 2x2 matrix
        is a pure rotation matrix with no scaling, then we can use the tranposed
        instead of the inverse.
        \see transpose(), inverse(), get_inverse()
        \return The transposed 2x2 matrix and the negated translation 2D vector (column Z). */
    value_type			get_transpose() const
    {
        const auto m22T = m_m22.get_transpose();
        return value_type(m22T, -m22T*m_v2);
    }



    /** Return the inverse matrix  The invers matrix is returned. In 2D graphics
        use the inverse matrix if the original matrix (upper 2x2) is not a pure rotation matrix
        (orthonormal).  Ie. if the original matrix (upper 2x2) contains scaling. The
        2D vector representing the translation (coloumn Z) is negated.
        \see inverse(), get_transpose(), transpose().
        \return The inverse 2x2 matrix and the negated translation 2D vector (column Z). */
    value_type			get_inverse() const
    {
        return value_type(m_m22.get_transpose(), -m_v2);
    }


    // -----------------------------
    // --- Load matrix functions ---
    // -----------------------------


    /** Look at direction and place at position. Creates an ortho-normal matrix
        using the input angle as direction. The matrix here refers only to the upper 2x2.
        The 2D vector part of the g33 matrix is set to the position requested.*/
    void				create(	T fAngle,					///< [in] Angle in degrees to rotate (Counter clockwise is positive).
                                v2_const_reference vPos)	///< [in] Position to use for the 2D (column Z) part.
    {
        m_m22.create_rot(fAngle);
        m_v2	= vPos;
    }


    /** Look at direction and place at position. Creates an ortho-normal matrix
        using the a 2D vector as direction. Vector does not need to be normalized.
        The matrix here refers only to the upper 2x2.
        The 2D vector part of the g33 matrix is set to the position requested.*/
    void				create(	v2_const_reference vDir,	///< [in] Direction to set the 2x2 rotation matrix to. Vector does not need to be normalized.
                                v2_const_reference vPos)	///< [in] Position to use for the 2D (column Z) part.
    {
        m_m22.create_rot(vDir);
        m_v2	= vPos;
    }



    // -------------------------------------
    // --- Determinant, Adjoint, Inverse ---
    // -------------------------------------

    /** Finds the determinant of the matrix.
        \return Determinant of matrix. */
    T					det() const
    {
        const T det_x = ycol().y()*zcol().z();
        const T det_y = xcol().y()*zcol().z();
        return xcol().x()*det_x - ycol().x()*det_y;
    }


    // -------------------------
    // --- Compare operators ---
    // -------------------------

    /** '==' operator. Tests two matrices for equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if all corresponding elements of the two matrices is no further,
        than EPSILON<T>() apart (absolute value), false otherwise. */
    bool	operator ==(const_reference m			///< [in] Right operand.
                       ) const
    {
        return	(m_m22 == m.m_m22) &&
                (m_v2  == m.m_v2);
    }

    /** '!=' operator. Tests two matrices for in-equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if just one corresponding pair of elements of the two
        matrices is further than EPSILON<T>() apart (absolute value),
        false otherwise. */
    bool	operator !=(const_reference m			///< [in] Right operand.
                       ) const
    {
        return	(m_m22 != m.m_m22) ||
                (m_v2  != m.m_v2);
    }


    // -------------------------
    // --- Compare functions ---
    // -------------------------


    /** Equal with exact comparision. Tests two matrices for equality.
        No epsilon value is used. Use with care when comparing floating point
        type values with this function.
        \return True if all corresponding elements of the two matrices are exactly
        equal, false otherwise. */
    bool		equal_exact(const_reference m)		///< [in] Matrix to compare with
    {
        return	m_m22.equal_exact(m.m_m22) &&
                m_v2.equal_exact(m.m_v2);
    }


    /** Equal with custom epsilon comparision. Tests two vectors for equality.
        Supplied epsilon value is used.
        \return True if all corresponding elements of the two vectors is no further,
        than epsilon apart (absolute value), false otherwise.. */
    bool		equal_epsilon(const_reference m,	///< [in] Matrix to compare with
                              const T epsilon		///< [in] Epsilon value to use when comparing
                             ) const
    {
        return	m_m22.equal_epsilon(m.m_m22, epsilon) &&
                m_v2.equal_epsilon(m.m_v2, epsilon);
    }

    // ----------------------------------
    // --- 2D Homogeneous coordinates ---
    // ----------------------------------



    // -----------------------
    // --- Debug Functions ---
    // -----------------------

    /// Convert matrix to a string for debug print.
    std::string			str(const int iDecimals= -1	///< [in] Number of decimals
                           ) const
    {
        std::string s = get_xrow().str(iDecimals);
            s += "\n" + get_yrow().str(iDecimals);
            s += "\n" + get_zrow().str(iDecimals);
        return s;
    }



    // -------------------
    // --- Member Data ---
    // -------------------
private:
    m22_value_type			m_m22;		///< 2x2 matrix (m22<T>), topleft part of the total 3x3 matrix
    v2_value_type			m_v2;		///< X,Y components of rightmost coloumn of the total 3x3 matrix

};

// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <class T>
const g33<T> g33<T>::M_ZERO	=  g33<T>(T(0), T(0), T(0), T(0), T(0), T(0));

template <class T>
const g33<T> g33<T>::M_I	=  g33<T>(T(1), T(0), T(0), T(1), T(0), T(0));


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - matrix multiplication. Scales the matrix with a scalar.
template <typename T>
inline g33<T>	operator *(	const T fVal, 		// [in] Left Hand Side operand (scalar).
                            const g33<T>& m)	// [in] Right hand side operand matrix.
{
    return m*fVal;	// Implement using operator g33<T> * T
}


//  /**
//'*' operator Multiply inverse matrix with vector. Assuming the matrix represents an
// affine transformation in homogeneous coordinates, this operator is a quick way of
// multiplying	the inverse matrix with a vector. Like this:
// \code
//	V*M == M.get_inverse()*V
// \endcode
// So it performs a normal M^-1*V, just a little faster since no unnessesary copying
// (like it's done when calling M.get_inverse()) is done!
// \return Inverse matrix mulplied with vector V*M == M^-1*V
// */
// template <typename T>
// inline v2<T>	operator *(	const v2<T>& v,		///< [in] Left hand side operand (v2).
// 							const g33<T>& m)	///< [in] Right hand side operand (g33).
//{
//		return v2_value_type(	m.xcol().x()*v.x() + m.xcol().y()*v.y() - m.zcol().x(),
//								m.ycol().x()*v.x() + m.ycol().y()*v.y() - m.zcol().y() );
//}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write matrix to stream.
    \return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&		operator <<(std::ostream& os,	///< [out] Output stream to write to.
                                        const g33<T>& m)	///< [in] Matrix to write.
{
    os<<m.xcol()<<m.ycol()<<m.zcol();
    return os;
}

/** '>>' operator. Read input from stream. */
template <typename T>
inline std::istream&		operator >>(std::istream& is,	///< [in]  Input stream to read from.
                                        g33<T>& m)			///< [out] Matrix recieve the data from the stream.
{
    is>>m.xcol()>>m.ycol()>>m.zcol();
    return is;
}


typedef g33<float>	g33f;
typedef g33<int>    g33i;

}

