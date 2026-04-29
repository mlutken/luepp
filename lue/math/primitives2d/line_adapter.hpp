#pragma once

#include "../base/lue_math_internal_utils.hpp"
#include "../base/v2.hpp"

#include <boost/math/special_functions.hpp>

namespace lue::math::p2d {


/**
Line (2D) container adapter.
*/
template
<
    typename NUMBER_TYPE,
    class GetPixelPolicy
>
class line_adapter
{
public:

    // ----------------
    // --- Typedefs ---
    // ----------------
    typedef typename	GetPixelPolicy::value_type		value_type;
    typedef typename	GetPixelPolicy::reference		reference;
    typedef typename	GetPixelPolicy::const_reference	const_reference;
    typedef typename	GetPixelPolicy::pointer			pointer;
    typedef typename	GetPixelPolicy::const_pointer	const_pointer;
    typedef typename	GetPixelPolicy::size_type		size_type;
    typedef typename	GetPixelPolicy::difference_type	difference_type;


    // --- vector typedefs ---
    typedef v2<NUMBER_TYPE>			v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    typedef v2<int>                 v2_pixel_type;


    line_adapter()
        : m_step_size(1)
    {}

    line_adapter(const GetPixelPolicy& get_pixel_policy)
        : m_step_size       (1)
        , m_getPixelPolicy  (get_pixel_policy)
    {}


    line_adapter(const v2_value_type& pos_start, const v2_value_type& pos_end)
        : m_step_size   (1)
        , m_pos_start   (pos_start)
        , m_pos_end     (pos_end)
    {
        update_calculated();
    }

    line_adapter(const GetPixelPolicy& get_pixel_policy, const v2_value_type& pos_start, const v2_value_type& pos_end)
        : m_step_size       (1)
        , m_getPixelPolicy  (get_pixel_policy)
        , m_pos_start       (pos_start)
        , m_pos_end         (pos_end)
    {
        update_calculated();
    }

    /** [] Operator, Acces individual elements of a v2 vector.
        \return Reference to indexed element (coordinate) of vector. */
    value_type&					operator[](const size_type i)				///< [in] Index value.
    {
        return m_getPixelPolicy.pixel_ref(index_to_pixel_pos(i));
    }

    const value_type&			operator[](const size_type i) const			///< [in] Index value.
    {
        return m_getPixelPolicy.get_pixel(index_to_pixel_pos(i));
    }


    v2_value_type index_to_pos( size_type index)
    {
        return m_pos_start + static_cast<NUMBER_TYPE>(index)*m_vec_step;
    }

    v2_value_type index_to_pixel_pos( size_type index)
    {
        const v2_value_type v = index_to_pos(index);
        return v2_pixel_type(boost::math::iround(v.x()), boost::math::iround(v.y()));
    }

//    size_type           size    () const    { return boost::math::iround(len()) +1; }
    size_type           size    () const
    {
        const NUMBER_TYPE len_x = std::fabs(m_pos_start.x() - m_pos_end.x());
        const NUMBER_TYPE len_y = std::fabs(m_pos_start.y() - m_pos_end.y());
        const NUMBER_TYPE max = std::max(len_x, len_y);
        const int max_rounded = boost::math::iround(max);
        return static_cast<size_type>(max_rounded) +1;
    }
    NUMBER_TYPE         len2    () const    { return m_vec.len2();   }
    NUMBER_TYPE         len     () const    { return m_vec.len();    }
    // ----------------------------------------
    // --- PUBLIC: Simple set/get functions ---
    // ----------------------------------------
    const v2_value_type&    start       () const                    { return m_pos_start;  }
    void                    start_set   ( const v2_value_type& pos) { m_pos_start = pos; update_calculated(); }
    const v2_value_type&    end         () const                    { return m_pos_end;  }
    void                    end_set     ( const v2_value_type& pos) { m_pos_end = pos; update_calculated(); }

    void                    get_pixel_policy_set  ( const GetPixelPolicy& policy )  { m_getPixelPolicy = policy; }
    GetPixelPolicy&         get_pixel_policy      ()                                { return m_getPixelPolicy; }

    const v2_value_type     vec_step    () const { return m_vec_step; }
private:
    void update_calculated()
    {
        m_vec = m_pos_end - m_pos_start;
        m_vec_step = m_vec.normalized();
        const size_type sz_len = size() -1;
        if (sz_len > 0) {
            m_vec_step *= len()/static_cast<NUMBER_TYPE>(sz_len);
        }
    }

    NUMBER_TYPE             m_step_size; // TODO: Do we want to support this?
    GetPixelPolicy          m_getPixelPolicy;
    v2_value_type           m_pos_start;
    v2_value_type           m_pos_end;
    v2_value_type           m_vec;
    v2_value_type           m_vec_step;



};

}


