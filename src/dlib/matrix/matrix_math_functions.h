// Copyright (C) 2006  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_MATRIx_MATH_FUNCTIONS
#define DLIB_MATRIx_MATH_FUNCTIONS 

#include "matrix_utilities.h"
#include "matrix.h"
#include "../algs.h"
#include <cmath>
#include <complex>
#include <limits>


namespace dlib
{

// ----------------------------------------------------------------------------------------

#define DLIB_MATRIX_SIMPLE_STD_FUNCTION(name)  template <typename EXP>         \
    struct op_##name : has_nondestructive_aliasing, preserves_dimensions<EXP>   \
    {                                                                           \
        typedef typename EXP::type type;                                        \
        template <typename M>                                                   \
        static type apply ( const M& m, long r, long c)                         \
        { return static_cast<type>(std::name(m(r,c))); }                        \
    };                                                                          \
    template < typename EXP >                                                   \
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_##name<EXP> > > name ( \
        const matrix_exp<EXP>& m)                                               \
    {                                                                           \
        typedef matrix_unary_exp<matrix_exp<EXP>,op_##name<EXP> > exp;          \
        return matrix_exp<exp>(exp(m));                                         \
    }                                                                           

// ----------------------------------------------------------------------------------------

DLIB_MATRIX_SIMPLE_STD_FUNCTION(abs)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(sqrt)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(log)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(log10)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(exp)

DLIB_MATRIX_SIMPLE_STD_FUNCTION(conj)

DLIB_MATRIX_SIMPLE_STD_FUNCTION(ceil)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(floor)

DLIB_MATRIX_SIMPLE_STD_FUNCTION(sin)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(cos)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(tan)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(sinh)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(cosh)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(tanh)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(asin)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(acos)
DLIB_MATRIX_SIMPLE_STD_FUNCTION(atan)

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_sigmoid : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { 
            const double e = 2.718281828459045235360287471352; 
            double temp = std::pow(e,-m(r,c));
            return static_cast<type>(1.0/(1.0 + temp));
        }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_sigmoid<EXP> > > sigmoid (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_sigmoid<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_round_zeros : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M, typename T>
        static type apply ( const M& m, const T& eps, long r, long c)
        { 
            const type temp = m(r,c);
            if (temp >= eps || temp <= -eps)
                return temp;
            else
                return 0;
        }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type,op_round_zeros<EXP> > > round_zeros (
        const matrix_exp<EXP>& m
    )
    {
        // you can only round matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type, op_round_zeros<EXP> > exp;
        return matrix_exp<exp>(exp(m,10*std::numeric_limits<typename EXP::type>::epsilon()));
    }

    template <
        typename EXP
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type,op_round_zeros<EXP> > > round_zeros (
        const matrix_exp<EXP>& m,
        typename EXP::type eps 
    )
    {
        // you can only round matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type, op_round_zeros<EXP> > exp;
        return matrix_exp<exp>(exp(m,eps));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_cubed : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return m(r,c)*m(r,c)*m(r,c); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_cubed<EXP> > > cubed (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_cubed<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_squared : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return m(r,c)*m(r,c); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_squared<EXP> > > squared (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_squared<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_pow : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M, typename S>
        static type apply ( const M& m, const S& s, long r, long c)
        { return static_cast<type>(std::pow(m(r,c),s)); }
    };

    template <
        typename EXP,
        typename S
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type,op_pow<EXP> > > pow (
        const matrix_exp<EXP>& m,
        const S& s
    )
    {
        // you can only round matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type,op_pow<EXP> > exp;
        return matrix_exp<exp>(exp(m,s));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_reciprocal : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { 
            const type temp = m(r,c);
            if (temp != 0)
                return static_cast<type>(1.0/temp);
            else
                return 0;
        }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_reciprocal<EXP> > > reciprocal (
        const matrix_exp<EXP>& m
    )
    {
        // you can only compute reciprocal matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_unary_exp<matrix_exp<EXP>,op_reciprocal<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_normalize : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, const type& s, long r, long c)
        { 
            return m(r,c)*s;
        }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type,op_normalize<EXP> > > normalize (
        const matrix_exp<EXP>& m
    )
    {
        // you can only compute normalized matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,typename EXP::type, op_normalize<EXP> > exp;

        typename EXP::type temp = std::sqrt(sum(squared(m)));
        if (temp != 0.0)
            temp = 1.0/temp;

        return matrix_exp<exp>(exp(m,temp));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_round : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { 
            return static_cast<type>(std::floor(m(r,c)+0.5)); 
        }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_round<EXP> > > round (
        const matrix_exp<EXP>& m
    )
    {
        // you can only round matrices that contain floats, doubles or long doubles.
        COMPILE_TIME_ASSERT((
                is_same_type<typename EXP::type,float>::value == true || 
                is_same_type<typename EXP::type,double>::value == true || 
                is_same_type<typename EXP::type,long double>::value == true 
        ));
        typedef matrix_unary_exp<matrix_exp<EXP>,op_round<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP1, typename EXP2>
    struct op_complex_matrix : has_nondestructive_aliasing, preserves_dimensions<EXP1,EXP2>
    {
        typedef std::complex<typename EXP1::type> type;

        template <typename M1, typename M2>
        static type apply ( const M1& m1, const M2& m2 , long r, long c)
        { return type(m1(r,c),m2(r,c)); }
    };

    template <
        typename EXP1,
        typename EXP2
        >
    const matrix_exp<matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_complex_matrix<EXP1,EXP2> > > complex_matrix (
        const matrix_exp<EXP1>& real_part,
        const matrix_exp<EXP2>& imag_part 
    )
    {
        COMPILE_TIME_ASSERT((is_same_type<typename EXP1::type,typename EXP2::type>::value == true));
        COMPILE_TIME_ASSERT(EXP1::NR == EXP2::NR || EXP1::NR == 0 || EXP2::NR == 0);
        COMPILE_TIME_ASSERT(EXP1::NC == EXP2::NC || EXP1::NC == 0 || EXP2::NC == 0);

        DLIB_ASSERT(real_part.nr() == imag_part.nr() &&
               real_part.nc() == imag_part.nc(), 
            "\tconst matrix_exp::type complex_matrix(real_part, imag_part)"
            << "\n\tYou can only make a complex matrix from two equally sized matrices"
            << "\n\treal_part.nr(): " << real_part.nr()
            << "\n\treal_part.nc(): " << real_part.nc() 
            << "\n\timag_part.nr(): " << imag_part.nr()
            << "\n\timag_part.nc(): " << imag_part.nc() 
            );
        typedef matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_complex_matrix<EXP1,EXP2> > exp;
        return matrix_exp<exp>(exp(real_part,imag_part));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_norm : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type::value_type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return std::norm(m(r,c)); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_norm<EXP> > > norm (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_norm<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_real : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type::value_type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return std::real(m(r,c)); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_real<EXP> > > real (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_real<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_imag : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type::value_type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return std::imag(m(r,c)); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_imag<EXP> > > imag (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_imag<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_MATRIx_MATH_FUNCTIONS

