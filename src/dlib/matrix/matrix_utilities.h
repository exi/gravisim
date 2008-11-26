// Copyright (C) 2006  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_MATRIx_UTILITIES_
#define DLIB_MATRIx_UTILITIES_

#include "matrix_utilities_abstract.h"
#include "matrix.h"
#include <cmath>
#include <complex>
#include <limits>
#include "../pixel.h"
#include "../geometry.h"
#include "../is_kind.h"
#include "../stl_checked.h"
#include <vector>


namespace dlib
{

// ----------------------------------------------------------------------------------------

    template <typename T>
    struct is_matrix<matrix_exp<T> > { static const bool value = true; }; 
    template <typename T, long NR, long NC, typename mm>
    struct is_matrix<matrix_ref<T,NR,NC,mm> > { static const bool value = true; }; 
    template <typename T, long NR, long NC, typename mm>
    struct is_matrix<matrix<T,NR,NC,mm> > { static const bool value = true; }; 
    /*
        is_matrix<T>::value == 1 if T is a matrix type else 0
    */

// ----------------------------------------------------------------------------------------

    /*
        templates for finding the max of two matrix expressions' dimensions
    */

    template <typename EXP1, typename EXP2 = void, typename EXP3 = void, typename EXP4 = void>
    struct max_nr;

    template <typename EXP1>
    struct max_nr<EXP1,void,void,void>
    {
        const static long val = EXP1::NR;
    };

    template <typename EXP1, typename EXP2>
    struct max_nr<EXP1,EXP2,void,void>
    {
        const static long val = (EXP1::NR > EXP2::NR) ? (EXP1::NR) : (EXP2::NR);
    };

    template <typename EXP1, typename EXP2, typename EXP3>
    struct max_nr<EXP1,EXP2,EXP3,void>
    {
    private:
        const static long max12 = (EXP1::NR > EXP2::NR) ? (EXP1::NR) : (EXP2::NR);
    public:
        const static long val = (max12 > EXP3::NR) ? (max12) : (EXP3::NR);
    };

    template <typename EXP1, typename EXP2, typename EXP3, typename EXP4>
    struct max_nr
    {
    private:
        const static long max12 = (EXP1::NR > EXP2::NR) ? (EXP1::NR) : (EXP2::NR);
        const static long max34 = (EXP3::NR > EXP4::NR) ? (EXP3::NR) : (EXP4::NR);
    public:
        const static long val = (max12 > max34) ? (max12) : (max34);
    };


    template <typename EXP1, typename EXP2 = void, typename EXP3 = void, typename EXP4 = void>
    struct max_nc;

    template <typename EXP1>
    struct max_nc<EXP1,void,void,void>
    {
        const static long val = EXP1::NC;
    };

    template <typename EXP1, typename EXP2>
    struct max_nc<EXP1,EXP2,void,void>
    {
        const static long val = (EXP1::NC > EXP2::NC) ? (EXP1::NC) : (EXP2::NC);
    };

    template <typename EXP1, typename EXP2, typename EXP3>
    struct max_nc<EXP1,EXP2,EXP3,void>
    {
    private:
        const static long max12 = (EXP1::NC > EXP2::NC) ? (EXP1::NC) : (EXP2::NC);
    public:
        const static long val = (max12 > EXP3::NC) ? (max12) : (EXP3::NC);
    };

    template <typename EXP1, typename EXP2, typename EXP3, typename EXP4>
    struct max_nc
    {
    private:
        const static long max12 = (EXP1::NC > EXP2::NC) ? (EXP1::NC) : (EXP2::NC);
        const static long max34 = (EXP3::NC > EXP4::NC) ? (EXP3::NC) : (EXP4::NC);
    public:
        const static long val = (max12 > max34) ? (max12) : (max34);
    };

// ----------------------------------------------------------------------------------------

    template <
        typename OP
        >
    class matrix_zeroary_exp;  

    template <
        typename M,
        typename OP
        >
    class matrix_unary_exp;  

    template <
        typename M1,
        typename M2,
        typename OP
        >
    class matrix_binary_exp;

    struct has_destructive_aliasing
    {
        template <typename M, typename U, long iNR, long iNC, typename MM >
        static bool destructively_aliases (
            const M& m,
            const matrix<U,iNR,iNC,MM>& item
        ) { return m.aliases(item); }

        template <typename M1, typename M2, typename U, long iNR, long iNC, typename MM >
        static bool destructively_aliases (
            const M1& m1,
            const M2& m2,
            const matrix<U,iNR,iNC,MM>& item
        ) { return m1.aliases(item) || m2.aliases(item) ; }
    };

    struct has_nondestructive_aliasing
    {
        template <typename M, typename U, long iNR, long iNC, typename MM >
        static bool destructively_aliases (
            const M& m,
            const matrix<U,iNR,iNC,MM>& item
        ) { return m.destructively_aliases(item); }

        template <typename M1, typename M2, typename U, long iNR, long iNC, typename MM >
        static bool destructively_aliases (
            const M1& m1,
            const M2& m2,
            const matrix<U,iNR,iNC, MM>& item
        ) { return m1.destructively_aliases(item) || m2.destructively_aliases(item) ; }
    };

    template <typename EXP1, typename EXP2 = void, typename EXP3 = void, typename EXP4 = void>
    struct preserves_dimensions
    {
        const static long NR = max_nr<EXP1,EXP2,EXP3,EXP4>::val;
        const static long NC = max_nc<EXP1,EXP2,EXP3,EXP4>::val;

        typedef typename EXP1::mem_manager_type mem_manager_type;

        template <typename M>
        static long nr (const M& m) { return m.nr(); }
        template <typename M>
        static long nc (const M& m) { return m.nc(); }
        template <typename M1, typename M2>
        static long nr (const M1& m1, const M2& ) { return m1.nr(); }
        template <typename M1, typename M2>
        static long nc (const M1& m1, const M2& ) { return m1.nc(); }
    };

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const typename matrix_exp<EXP>::type max (
        const matrix_exp<EXP>& m
    )
    {
        typedef typename matrix_exp<EXP>::type type;

        type val = m(0,0);
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                type temp = m(r,c);
                if (temp > val)
                    val = temp;
            }
        }
        return val;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const typename matrix_exp<EXP>::type min (
        const matrix_exp<EXP>& m
    )
    {
        typedef typename matrix_exp<EXP>::type type;

        type val = m(0,0);
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                type temp = m(r,c);
                if (temp < val)
                    val = temp;
            }
        }
        return val;
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    namespace nric
    {
        // This namespace contains stuff from Numerical Recipes in C

        template <typename T>
        inline T pythag(const T& a, const T& b)
        {
            T absa,absb;
            absa=std::abs(a);
            absb=std::abs(b);
            if (absa > absb) 
            {
                T val = absb/absa;
                val *= val;
                return absa*std::sqrt(1.0+val);
            }
            else 
            {
                if (absb == 0.0)
                {
                    return 0.0;
                }
                else
                {
                    T val = absa/absb;
                    val *= val;
                    return  absb*std::sqrt(1.0+val);
                }
            }
        }

        template <typename T>
        inline T sign(const T& a, const T& b)
        {
            if (b < 0)
            {
                return -std::abs(a);
            }
            else
            {
                return std::abs(a);
            }
        }


        template <
            typename T,
            long M, long N,
            long wN, long wX,
            long vN, 
            long rN, long rX,
            typename MM1,
            typename MM2,
            typename MM3,
            typename MM4
            >
        bool svdcmp(
            matrix<T,M,N,MM1>& a,  
            matrix<T,wN,wX,MM2>& w,
            matrix<T,vN,vN,MM3>& v,
            matrix<T,rN,rX,MM4>& rv1
        )
        /*!  ( this function is derived from the one in numerical recipes in C chapter 2.6)
            requires
                - w.nr() == a.nc()
                - w.nc() == 1
                - v.nr() == a.nc()
                - v.nc() == a.nc()
                - rv1.nr() == a.nc()
                - rv1.nc() == 1
            ensures
                - computes the singular value decomposition of a
                - let W be the matrix such that diag(W) == #w then:
                    - a == #a*W*trans(#v)
                - trans(#a)*#a == identity matrix
                - trans(#v)*#v == identity matrix
                - #rv1 == some undefined value
                - returns true for success and false for failure
        !*/
        {

            DLIB_ASSERT(
                 w.nr() == a.nc() &&
                 w.nc() == 1 &&
                 v.nr() == a.nc() &&
                 v.nc() == a.nc() &&
                 rv1.nr() == a.nc() &&
                 rv1.nc() == 1, "");

            COMPILE_TIME_ASSERT(wX == 0 || wX == 1);
            COMPILE_TIME_ASSERT(rX == 0 || rX == 1);

            const T one = 1.0;
            const long max_iter = 30;
            const long n = a.nc();
            const long m = a.nr();
            const T eps = std::numeric_limits<T>::epsilon();
            long nm = 0, l = 0;
            bool flag;
            T anorm,c,f,g,h,s,scale,x,y,z;
            g = 0.0;
            scale = 0.0;
            anorm = 0.0; 

            for (long i = 0; i < n; ++i) 
            {
                l = i+1;
                rv1(i) = scale*g;
                g = s = scale = 0.0;
                if (i < m) 
                {
                    for (long k = i; k < m; ++k) 
                        scale += std::abs(a(k,i));

                    if (scale) 
                    {
                        for (long k = i; k < m; ++k) 
                        {
                            a(k,i) /= scale;
                            s += a(k,i)*a(k,i);
                        }
                        f = a(i,i);
                        g = -sign(std::sqrt(s),f);
                        h = f*g - s;
                        a(i,i) = f - g;
                        for (long j = l; j < n; ++j) 
                        {
                            s = 0.0;
                            for (long k = i; k < m; ++k) 
                                s += a(k,i)*a(k,j);

                            f = s/h;

                            for (long k = i; k < m; ++k) 
                                a(k,j) += f*a(k,i);
                        }
                        for (long k = i; k < m; ++k) 
                            a(k,i) *= scale;
                    }
                }

                w(i) = scale *g;

                g=s=scale=0.0;

                if (i < m && i < n-1) 
                {
                    for (long k = l; k < n; ++k) 
                        scale += std::abs(a(i,k));

                    if (scale) 
                    {
                        for (long k = l; k < n; ++k) 
                        {
                            a(i,k) /= scale;
                            s += a(i,k)*a(i,k);
                        }
                        f = a(i,l);
                        g = -sign(std::sqrt(s),f);
                        h = f*g - s;
                        a(i,l) = f - g;

                        for (long k = l; k < n; ++k) 
                            rv1(k) = a(i,k)/h;

                        for (long j = l; j < m; ++j) 
                        {
                            s = 0.0;
                            for (long k = l; k < n; ++k) 
                                s += a(j,k)*a(i,k);

                            for (long k = l; k < n; ++k) 
                                a(j,k) += s*rv1(k);
                        }
                        for (long k = l; k < n; ++k) 
                            a(i,k) *= scale;
                    }
                }
                anorm = std::max(anorm,(std::abs(w(i))+std::abs(rv1(i))));
            }
            for (long i = n-1; i >= 0; --i) 
            { 
                if (i < n-1) 
                {
                    if (g != 0) 
                    {
                        for (long j = l; j < n ; ++j) 
                            v(j,i) = (a(i,j)/a(i,l))/g;

                        for (long j = l; j < n; ++j) 
                        {
                            s = 0.0;
                            for (long k = l; k < n; ++k) 
                                s += a(i,k)*v(k,j);

                            for (long k = l; k < n; ++k) 
                                v(k,j) += s*v(k,i);
                        }
                    }

                    for (long j = l; j < n; ++j) 
                        v(i,j) = v(j,i) = 0.0;
                }

                v(i,i) = 1.0;
                g = rv1(i);
                l = i;
            }

            for (long i = std::min(m,n)-1; i >= 0; --i) 
            { 
                l = i + 1;
                g = w(i);

                for (long j = l; j < n; ++j) 
                    a(i,j) = 0.0;

                if (g != 0) 
                {
                    g = 1.0/g;

                    for (long j = l; j < n; ++j) 
                    {
                        s = 0.0;
                        for (long k = l; k < m; ++k) 
                            s += a(k,i)*a(k,j);

                        f=(s/a(i,i))*g;

                        for (long k = i; k < m; ++k) 
                            a(k,j) += f*a(k,i);
                    }
                    for (long j = i; j < m; ++j) 
                        a(j,i) *= g;
                } 
                else 
                {
                    for (long j = i; j < m; ++j) 
                        a(j,i) = 0.0;
                }

                ++a(i,i);
            }

            for (long k = n-1; k >= 0; --k) 
            { 
                for (long its = 1; its <= max_iter; ++its) 
                { 
                    flag = true;
                    for (l = k; l >= 1; --l) 
                    { 
                        nm = l - 1; 
                        if (std::abs(rv1(l)) <= eps*anorm) 
                        {
                            flag = false;
                            break;
                        }
                        if (std::abs(w(nm)) <= eps*anorm) 
                        {
                            break;
                        }
                    }

                    if (flag) 
                    {
                        c = 0.0;  
                        s = 1.0;
                        for (long i = l; i <= k; ++i) 
                        {
                            f = s*rv1(i);
                            rv1(i) = c*rv1(i);
                            if (std::abs(f) <= eps*anorm) 
                                break;

                            g = w(i);
                            h = pythag(f,g);
                            w(i) = h;
                            h = 1.0/h;
                            c = g*h;
                            s = -f*h;
                            for (long j = 0; j < m; ++j) 
                            {
                                y = a(j,nm);
                                z = a(j,i);
                                a(j,nm) = y*c + z*s;
                                a(j,i) = z*c - y*s;
                            }
                        }
                    }

                    z = w(k);
                    if (l == k) 
                    { 
                        if (z < 0.0) 
                        {
                            w(k) = -z;
                            for (long j = 0; j < n; ++j) 
                                v(j,k) = -v(j,k);
                        }
                        break;
                    }

                    if (its == max_iter) 
                        return false;

                    x = w(l); 
                    nm = k - 1;
                    y = w(nm);
                    g = rv1(nm);
                    h = rv1(k);
                    f = ((y-z)*(y+z) + (g-h)*(g+h))/(2.0*h*y);
                    g = pythag(f,one);
                    f = ((x-z)*(x+z) + h*((y/(f+sign(g,f)))-h))/x;
                    c = s = 1.0; 
                    for (long j = l; j <= nm; ++j) 
                    {
                        long i = j + 1;
                        g = rv1(i);
                        y = w(i);
                        h = s*g;
                        g = c*g;
                        z = pythag(f,h);
                        rv1(j) = z;
                        c = f/z;
                        s = h/z;
                        f = x*c + g*s;
                        g = g*c - x*s;
                        h = y*s;
                        y *= c;
                        for (long jj = 0; jj < n; ++jj) 
                        {
                            x = v(jj,j);
                            z = v(jj,i);
                            v(jj,j) = x*c + z*s;
                            v(jj,i) = z*c - x*s;
                        }
                        z = pythag(f,h);
                        w(j) = z; 
                        if (z != 0) 
                        {
                            z = 1.0/z;
                            c = f*z;
                            s = h*z;
                        }
                        f = c*g + s*y;
                        x = c*y - s*g;
                        for (long jj = 0; jj < m; ++jj) 
                        {
                            y = a(jj,j);
                            z = a(jj,i);
                            a(jj,j) = y*c + z*s;
                            a(jj,i) = z*c - y*s;
                        }
                    }
                    rv1(l) = 0.0;
                    rv1(k) = f;
                    w(k) = x;
                }
            }
            return true;
        }


        template <
            typename T,
            long N,
            long NX,
            typename MM1,
            typename MM2,
            typename MM3
            >
        bool ludcmp (
            matrix<T,N,N,MM1>& a,
            matrix<long,N,NX,MM2>& indx,
            T& d,
            matrix<T,N,NX,MM3> vv
        )
        /*!
            ( this function is derived from the one in numerical recipes in C chapter 2.3)
            ensures
                - #a == both the L and U matrices
                - #indx == the permutation vector (see numerical recipes in C)
                - #d == some other thing (see numerical recipes in C)
                - #vv == some undefined value.  this is just used for scratch space
                - if (the matrix is singular and we can't do anything) then
                    - returns false
                - else
                    - returns true
        !*/
        {
            DLIB_ASSERT(indx.nc() == 1,"in dlib::nric::ludcmp() the indx matrix must be a column vector");
            DLIB_ASSERT(vv.nc() == 1,"in dlib::nric::ludcmp() the vv matrix must be a column vector");
            const long n = a.nr();
            long imax = 0;
            T big, dum, sum, temp;

            d = 1.0;
            for (long i = 0; i < n; ++i)
            {
                big = 0;
                for (long j = 0; j < n; ++j)
                {
                    if ((temp=std::abs(a(i,j))) > big)
                        big = temp;
                }
                if (big == 0.0)
                {
                    return false;
                }
                vv(i) = 1/big;
            }

            for (long j = 0; j < n; ++j)
            {
                for (long i = 0; i < j; ++i)
                {
                    sum = a(i,j);
                    for (long k = 0; k < i; ++k)
                        sum -= a(i,k)*a(k,j);
                    a(i,j) = sum;
                }
                big = 0;
                for (long i = j; i < n; ++i)
                {
                    sum = a(i,j);
                    for (long k = 0; k < j; ++k)
                        sum -= a(i,k)*a(k,j);
                    a(i,j) = sum;
                    if ( (dum=vv(i)*std::abs(sum)) >= big)
                    {
                        big = dum;
                        imax = i;
                    }
                }
                if (j != imax)
                {
                    for (long k = 0; k < n; ++k)
                    {
                        dum = a(imax,k);
                        a(imax,k) = a(j,k);
                        a(j,k) = dum;
                    }
                    d = -d;
                    vv(imax) = vv(j);
                }
                indx(j) = imax;

                if (j < n-1)
                {
                    dum = 1/a(j,j);
                    for (long i = j+1; i < n; ++i)
                        a(i,j) *= dum;
                }
            }
            return true;
        }

// ----------------------------------------------------------------------------------------

        template <
            typename T,
            long N,
            long NX,
            typename MM1,
            typename MM2,
            typename MM3
            >
        void lubksb (
            const matrix<T,N,N,MM1>& a,
            const matrix<long,N,NX,MM2>& indx,
            matrix<T,N,NX,MM3>& b
        )
        /*!
            ( this function is derived from the one in numerical recipes in C chapter 2.3)
            requires
                - a == the LU decomposition you get from ludcmp()
                - indx == the indx term you get out of ludcmp()
                - b == the right hand side vector from the expression a*x = b
            ensures
                - #b == the solution vector x from the expression a*x = b
                  (basically, this function solves for x given b and a)
        !*/
        {
            DLIB_ASSERT(indx.nc() == 1,"in dlib::nric::lubksb() the indx matrix must be a column vector");
            DLIB_ASSERT(b.nc() == 1,"in dlib::nric::lubksb() the b matrix must be a column vector");
            const long n = a.nr();
            long i, ii = -1, ip, j;
            T sum;

            for (i = 0; i < n; ++i)
            {
                ip = indx(i);
                sum=b(ip);
                b(ip) = b(i);
                if (ii != -1)
                {
                    for (j = ii; j < i; ++j)
                        sum -= a(i,j)*b(j);
                }
                else if (sum)
                {
                    ii = i;
                }
                b(i) = sum;
            }
            for (i = n-1; i >= 0; --i)
            {
                sum = b(i);
                for (j = i+1; j < n; ++j)
                    sum -= a(i,j)*b(j);
                b(i) = sum/a(i,i);
            }
        }
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    template <
        typename OP
        >
    class matrix_zeroary_exp  
    {
    public:
        typedef typename OP::type type;
        typedef matrix_zeroary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(r,c); }

        template <typename U, long iNR, long iNC , typename MM>
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC, typename MM >
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        long nr (
        ) const { return NR; }

        long nc (
        ) const { return NC; }

        const ref_type& ref(
        ) const { return *this; }

    };

// ----------------------------------------------------------------------------------------

    template <
        typename S,
        typename OP
        >
    class dynamic_matrix_scalar_unary_exp  
    {
        /*!
            REQUIREMENTS ON S 
                - must NOT be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename OP::type type;
        typedef dynamic_matrix_scalar_unary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        dynamic_matrix_scalar_unary_exp (
            long nr__,
            long nc__,
            const S& s_
        ) :
            nr_(nr__),
            nc_(nc__),
            s(s_)
        {
            COMPILE_TIME_ASSERT(is_matrix<S>::value == false);
        }

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(s,r,c); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return nr_; }

        long nc (
        ) const { return nc_; }

    private:

        const long nr_;
        const long nc_;
        const S s;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename S,
        typename OP
        >
    class matrix_scalar_unary_exp  
    {
        /*!
            REQUIREMENTS ON S 
                - must NOT be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename OP::type type;
        typedef matrix_scalar_unary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        matrix_scalar_unary_exp (
            const S& s_
        ) :
            s(s_)
        {
            COMPILE_TIME_ASSERT(is_matrix<S>::value == false);
        }

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(s,r,c); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC, typename MM >
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return NR; }

        long nc (
        ) const { return NC; }

    private:

        const S s;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename M,
        typename OP
        >
    class matrix_unary_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename OP::type type;
        typedef matrix_unary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        matrix_unary_exp (
            const M& m_
        ) :
            m(m_)
        {}

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(m,r,c); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return m.aliases(item); }

        template <typename U, long iNR, long iNC, typename MM >
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return OP::destructively_aliases(m,item); }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return OP::nr(m); }

        long nc (
        ) const { return OP::nc(m); }

    private:

        const M m;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename M
        >
    class matrix_std_vector_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a std::vector object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename M::value_type type;
        typedef matrix_std_vector_exp ref_type;
        typedef typename memory_manager<char>::kernel_1a mem_manager_type;
        const static long NR = 0;
        const static long NC = 1;

        matrix_std_vector_exp (
            const M& m_
        ) :
            m(m_)
        {
        }

        const typename M::value_type operator() (
            long r, 
            long 
        ) const { return m[r]; }

        template <typename U, long iNR, long iNC, typename MM>
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return m.size(); }

        long nc (
        ) const { return 1; }

    private:
        const M& m;
    };

// ----------------------------------------------------------------------------------------


    template <
        typename M
        >
    class matrix_vector_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a dlib::array object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename M::type type;
        typedef matrix_vector_exp ref_type;
        typedef typename M::mem_manager_type mem_manager_type;
        const static long NR = 0;
        const static long NC = 1;

        matrix_vector_exp (
            const M& m_
        ) :
            m(m_)
        {
        }

        const typename M::type operator() (
            long r, 
            long 
        ) const { return m[r]; }

        template <typename U, long iNR, long iNC, typename MM>
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return m.size(); }

        long nc (
        ) const { return 1; }

    private:
        const M& m;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename M
        >
    class matrix_array_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a dlib::array2d object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename M::type type;
        typedef matrix_array_exp ref_type;
        typedef typename M::mem_manager_type mem_manager_type;
        const static long NR = 0;
        const static long NC = 0;

        matrix_array_exp (
            const M& m_
        ) :
            m(m_)
        {
        }

        const typename M::type operator() (
            long r, 
            long c
        ) const { return m[r][c]; }

        template <typename U, long iNR, long iNC, typename MM>
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return false; }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return m.nr(); }

        long nc (
        ) const { return m.nc(); }

    private:
        const M& m;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename M
        >
    class matrix_sub_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename M::type type;
        typedef matrix_sub_exp ref_type;
        typedef typename M::mem_manager_type mem_manager_type;
        const static long NR = 0;
        const static long NC = 0;

        matrix_sub_exp (
            const M& m_,
            const long& r__,
            const long& c__,
            const long& nr__,
            const long& nc__
        ) :
            m(m_),
            r_(r__),
            c_(c__),
            nr_(nr__),
            nc_(nc__)
        {
        }

        const typename M::type operator() (
            long r, 
            long c
        ) const { return m(r+r_,c+c_); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return m.aliases(item); }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return m.aliases(item); }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return nr_; }

        long nc (
        ) const { return nc_; }

    private:

        const M m;
        const long r_, c_, nr_, nc_;
    };

// ----------------------------------------------------------------------------------------


    template <
        typename M,
        typename S,
        typename OP
        >
    class matrix_scalar_binary_exp  
    {
        /*!
            REQUIREMENTS ON M 
                - must be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename OP::type type;
        typedef matrix_scalar_binary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        matrix_scalar_binary_exp (
            const M& m_,
            const S& s_
        ) :
            m(m_),
            s(s_)
        {
            COMPILE_TIME_ASSERT(is_matrix<S>::value == false);
        }

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(m,s,r,c); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return m.aliases(item); }

        template <typename U, long iNR, long iNC , typename MM>
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return OP::destructively_aliases(m,item); }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return OP::nr(m); }

        long nc (
        ) const { return OP::nc(m); }

    private:

        const M m;
        const S s;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename M1,
        typename M2,
        typename OP
        >
    class matrix_binary_exp  
    {
        /*!
            REQUIREMENTS ON M1 AND M2 
                - must be a matrix_exp or matrix_ref object (or
                  an object with a compatible interface).
        !*/
    public:
        typedef typename OP::type type;
        typedef matrix_binary_exp ref_type;
        typedef typename OP::mem_manager_type mem_manager_type;
        const static long NR = OP::NR;
        const static long NC = OP::NC;

        matrix_binary_exp (
            const M1& m1_,
            const M2& m2_
        ) :
            m1(m1_),
            m2(m2_)
        {}

        const typename OP::type operator() (
            long r, 
            long c
        ) const { return OP::apply(m1,m2,r,c); }

        template <typename U, long iNR, long iNC, typename MM >
        bool aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return m1.aliases(item) || m2.aliases(item); }

        template <typename U, long iNR, long iNC, typename MM >
        bool destructively_aliases (
            const matrix<U,iNR,iNC,MM>& item
        ) const { return OP::destructively_aliases(m1,m2,item); }

        const ref_type& ref(
        ) const { return *this; }

        long nr (
        ) const { return OP::nr(m1,m2); }

        long nc (
        ) const { return OP::nc(m1,m2); }

    private:

        const M1 m1;
        const M2 m2;
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    template <
        typename array_type
        >
    const matrix_exp<matrix_array_exp<array_type> > array_to_matrix (
        const array_type& array
    )
    {
        typedef matrix_array_exp<array_type> exp;
        return matrix_exp<exp>(exp(array));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename vector_type
        >
    const matrix_exp<matrix_vector_exp<vector_type> > vector_to_matrix (
        const vector_type& vector
    )
    {
        typedef matrix_vector_exp<vector_type> exp;
        return matrix_exp<exp>(exp(vector));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename value_type,
        typename alloc
        >
    const matrix_exp<matrix_std_vector_exp<std::vector<value_type,alloc> > > vector_to_matrix (
        const std::vector<value_type,alloc>& vector
    )
    {
        typedef matrix_std_vector_exp<std::vector<value_type,alloc> > exp;
        return matrix_exp<exp>(exp(vector));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename value_type,
        typename alloc
        >
    const matrix_exp<matrix_std_vector_exp<std_vector_c<value_type,alloc> > > vector_to_matrix (
        const std_vector_c<value_type,alloc>& vector
    )
    {
        typedef matrix_std_vector_exp<std_vector_c<value_type,alloc> > exp;
        return matrix_exp<exp>(exp(vector));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const rectangle get_rect (
        const matrix_exp<EXP>& m
    )
    {
        return rectangle(0, 0, m.nc()-1, m.nr()-1);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const matrix_exp<matrix_sub_exp<matrix_exp<EXP> > > subm (
        const matrix_exp<EXP>& m,
        long r, 
        long c,
        long nr,
        long nc
    )
    {
        DLIB_ASSERT(r >= 0 && c >= 0 && r+nr < m.nr() && c+nc < m.nc(), 
            "\tconst matrix_exp subm(const matrix_exp& m, r, c, nr, nc)"
            << "\n\tYou have specified invalid sub matrix dimensions"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\tr:      " << r 
            << "\n\tc:      " << c 
            << "\n\tnr:     " << nr 
            << "\n\tnc:     " << nc 
            );

        typedef matrix_sub_exp<matrix_exp<EXP> > exp;
        return matrix_exp<exp>(exp(m,r,c,nr,nc));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const matrix_exp<matrix_sub_exp<matrix_exp<EXP> > > subm (
        const matrix_exp<EXP>& m,
        const rectangle& rect
    )
    {
        DLIB_ASSERT(get_rect(m).contains(rect) == true, 
            "\tconst matrix_exp subm(const matrix_exp& m, const rectangle& rect)"
            << "\n\tYou have specified invalid sub matrix dimensions"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\trect.left():   " << rect.left()
            << "\n\trect.top():    " << rect.top()
            << "\n\trect.right():  " << rect.right()
            << "\n\trect.bottom(): " << rect.bottom()
            );

        typedef matrix_sub_exp<matrix_exp<EXP> > exp;
        return matrix_exp<exp>(exp(m,rect.top(),rect.left(),rect.height(),rect.width()));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_rowm : has_destructive_aliasing
    {
        const static long NR = 1;
        const static long NC = EXP::NC;
        typedef typename EXP::type type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        template <typename M>
        static type apply ( const M& m, long row, long, long c)
        { return m(row,c); }

        template <typename M>
        static long nr (const M& m) { return 1; }
        template <typename M>
        static long nc (const M& m) { return m.nc(); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,long,op_rowm<EXP> > > rowm (
        const matrix_exp<EXP>& m,
        long row
    )
    {
        DLIB_ASSERT(row >= 0 && row < m.nr(), 
            "\tconst matrix_exp rowm(const matrix_exp& m, row)"
            << "\n\tYou have specified invalid sub matrix dimensions"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\trow:    " << row 
            );

        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,long,op_rowm<EXP> > exp;
        return matrix_exp<exp>(exp(m,row));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_colm : has_destructive_aliasing
    {
        const static long NR = EXP::NR;
        const static long NC = 1;
        typedef typename EXP::type type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        template <typename M>
        static type apply ( const M& m, long col, long r, long)
        { return m(r,col); }

        template <typename M>
        static long nr (const M& m) { return m.nr(); }
        template <typename M>
        static long nc (const M& m) { return 1; }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_scalar_binary_exp<matrix_exp<EXP>,long,op_colm<EXP> > > colm (
        const matrix_exp<EXP>& m,
        long col 
    )
    {
        DLIB_ASSERT(col >= 0 && col < m.nc(), 
            "\tconst matrix_exp colm(const matrix_exp& m, row)"
            << "\n\tYou have specified invalid sub matrix dimensions"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\tcol:    " << col 
            );

        typedef matrix_scalar_binary_exp<matrix_exp<EXP>,long,op_colm<EXP> > exp;
        return matrix_exp<exp>(exp(m,col));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_trans : has_destructive_aliasing
    {
        const static long NR = EXP::NC;
        const static long NC = EXP::NR;
        typedef typename EXP::type type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return m(c,r); }

        template <typename M>
        static long nr (const M& m) { return m.nc(); }
        template <typename M>
        static long nc (const M& m) { return m.nr(); }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_trans<EXP> > > trans (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_trans<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP, long R, long C>
    struct op_removerc : has_destructive_aliasing
    {
        const static long NR = EXP::NR - 1;
        const static long NC = EXP::NC - 1;
        typedef typename EXP::type type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { 
            if (r < R)
            {
                if (c < C)
                    return m(r,c); 
                else
                    return m(r,c+1); 
            }
            else
            {
                if (c < C)
                    return m(r+1,c); 
                else
                    return m(r+1,c+1); 
            }
        }

        template <typename M>
        static long nr (const M& m) { return m.nr() - 1; }
        template <typename M>
        static long nc (const M& m) { return m.nc() - 1; }
    };

    template <
        long R,
        long C,
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_removerc<EXP,R,C> > > removerc (
        const matrix_exp<EXP>& m
    )
    {
        // you can't remove a row from a matrix with only one row
        COMPILE_TIME_ASSERT(EXP::NR > 1 || EXP::NR == 0);
        // you can't remove a column from a matrix with only one column 
        COMPILE_TIME_ASSERT(EXP::NC > 1 || EXP::NR == 0);
        DLIB_ASSERT(m.nr() > 1 && m.nc() > 1, 
            "\tconst matrix_exp removerc<R,C>(const matrix_exp& m)"
            << "\n\tYou can't remove a row/column from a matrix with only one row/column"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\tR:      " << R 
            << "\n\tC:      " << C 
            );
        typedef matrix_unary_exp<matrix_exp<EXP>,op_removerc<EXP,R,C> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP>
    struct op_diag : has_destructive_aliasing
    {
        const static long NR = EXP::NC;
        const static long NC = 1;
        typedef typename EXP::type type;
        typedef typename EXP::mem_manager_type mem_manager_type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return m(r,r); }

        template <typename M>
        static long nr (const M& m) { return m.nr(); }
        template <typename M>
        static long nc (const M& m) { return 1; }
    };

    template <
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_diag<EXP> > > diag (
        const matrix_exp<EXP>& m
    )
    {
        // You can only get the diagonal for square matrices.
        COMPILE_TIME_ASSERT(EXP::NR == EXP::NC);
        DLIB_ASSERT(m.nr() == m.nc(), 
            "\tconst matrix_exp diag(const matrix_exp& m)"
            << "\n\tYou can only apply diag() to a square matrix"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            );
        typedef matrix_unary_exp<matrix_exp<EXP>,op_diag<EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP, typename target_type>
    struct op_cast : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef target_type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return static_cast<target_type>(m(r,c)); }
    };

    template <
        typename target_type,
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_cast<EXP,target_type> > > matrix_cast (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_cast<EXP,target_type> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename T,
        long NR,
        long NC,
        typename MM,
        typename U
        >
    void set_all_elements (
        matrix<T,NR,NC,MM>& m,
        U value
    )
    {
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                m(r,c) = static_cast<T>(value);
            }
        }
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP,
        long uNR, 
        long uNC,
        long wN, 
        long vN,
        typename MM1,
        typename MM2,
        typename MM3
        >
    inline void svd (
        const matrix_exp<EXP>& m,
        matrix<typename matrix_exp<EXP>::type, uNR, uNC,MM1>& u,
        matrix<typename matrix_exp<EXP>::type, wN, wN,MM2>& w,
        matrix<typename matrix_exp<EXP>::type, vN, vN,MM3>& v
    )
    {
        typedef typename matrix_exp<EXP>::type T;
        const long NR = matrix_exp<EXP>::NR;
        const long NC = matrix_exp<EXP>::NC;

        // make sure the output matrices have valid dimensions if they are statically dimensioned
        COMPILE_TIME_ASSERT(NR == 0 || uNR == 0 || NR == uNR);
        COMPILE_TIME_ASSERT(NC == 0 || uNC == 0 || NC == uNC);
        COMPILE_TIME_ASSERT(NC == 0 || wN == 0 || NC == wN);
        COMPILE_TIME_ASSERT(NC == 0 || vN == 0 || NC == vN);
        COMPILE_TIME_ASSERT(NR >= NC || NR == 0);

        w.set_size(m.nc(),m.nc());
        v.set_size(m.nc(),m.nc());

        typedef typename matrix_exp<EXP>::type T;
        u = m;

        matrix<T,matrix_exp<EXP>::NC,1,MM1> W(m.nc(),1);
        matrix<T,matrix_exp<EXP>::NC,1,MM1> rv1(m.nc(),1);
        set_all_elements(w,0);

        nric::svdcmp(u,W,v,rv1);

        for (long r = 0; r < W.nr(); ++r)
            w(r,r) = W(r);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::matrix_type pinv (
        const matrix_exp<EXP>& m
    )
    { 
        typename matrix_exp<EXP>::matrix_type u;
        matrix<typename EXP::type, EXP::NC, EXP::NC, typename EXP::mem_manager_type> w, v;
        svd(m,u,w,v);

        const double machine_eps = std::numeric_limits<typename EXP::type>::epsilon();
        // compute a reasonable epsilon below which we round to zero before doing the
        // reciprocal
        const double eps = machine_eps*std::max(m.nr(),m.nc())*max(diag(w));

        // compute the reciprocal of the diagonal of w
        matrix<typename EXP::type,EXP::NC,1, typename EXP::mem_manager_type> w_diag = reciprocal(round_zeros(diag(w),eps));

        // now compute the pseudoinverse
        return tmp(scale_columns(v,w_diag))*trans(u);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP,
        long N
        >
    struct inv_helper
    {
        static const typename matrix_exp<EXP>::matrix_type inv (
            const matrix_exp<EXP>& m
        )
        {
            using namespace nric;
            typedef typename EXP::mem_manager_type MM;
            // you can't invert a non-square matrix
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC || 
                                matrix_exp<EXP>::NR == 0 ||
                                matrix_exp<EXP>::NC == 0);
            DLIB_ASSERT(m.nr() == m.nc(), 
                "\tconst matrix_exp::type inv(const matrix_exp& m)"
                << "\n\tYou can only apply inv() to a square matrix"
                << "\n\tm.nr(): " << m.nr()
                << "\n\tm.nc(): " << m.nc() 
                );
            typedef typename matrix_exp<EXP>::type type;

            matrix<type, N, N,MM> a(m), y(m.nr(),m.nr());
            matrix<long,N,1,MM> indx(m.nr(),1);
            matrix<type,N,1,MM> col(m.nr(),1);
            matrix<type,N,1,MM> vv(m.nr(),1);
            type d;
            long i, j;
            if (ludcmp(a,indx,d,vv))
            {
                for (j = 0; j < m.nr(); ++j)
                {
                    for (i = 0; i < m.nr(); ++i)
                        col(i) = 0;
                    col(j) = 1;
                    lubksb(a,indx,col);
                    for (i = 0; i < m.nr(); ++i)
                        y(i,j) = col(i);
                }
            }
            else
            {
                // m is singular so lets just set y equal to m just so that 
                // it has some value
                y = m;
            }
            return y;
        }
    };

    template <
        typename EXP
        >
    struct inv_helper<EXP,1>
    {
        static const typename matrix_exp<EXP>::matrix_type inv (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            matrix<type, 1, 1, typename EXP::mem_manager_type> a;
            a(0) = 1/m(0);
            return a;
        }
    };

    template <
        typename EXP
        >
    struct inv_helper<EXP,2>
    {
        static const typename matrix_exp<EXP>::matrix_type inv (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            matrix<type, 2, 2, typename EXP::mem_manager_type> a;
            type d = static_cast<type>(1.0/det(m));
            a(0,0) = m(1,1)*d;
            a(0,1) = m(0,1)*-d;
            a(1,0) = m(1,0)*-d;
            a(1,1) = m(0,0)*d;
            return a;
        }
    };

    template <
        typename EXP
        >
    struct inv_helper<EXP,3>
    {
        static const typename matrix_exp<EXP>::matrix_type inv (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            matrix<type, 3, 3, typename EXP::mem_manager_type> ret;
            const type de = static_cast<type>(1.0/det(m));
            const type a = m(0,0);
            const type b = m(0,1);
            const type c = m(0,2);
            const type d = m(1,0);
            const type e = m(1,1);
            const type f = m(1,2);
            const type g = m(2,0);
            const type h = m(2,1);
            const type i = m(2,2);

            ret(0,0) = (e*i - f*h)*de;
            ret(1,0) = (f*g - d*i)*de;
            ret(2,0) = (d*h - e*g)*de;

            ret(0,1) = (c*h - b*i)*de;
            ret(1,1) = (a*i - c*g)*de;
            ret(2,1) = (b*g - a*h)*de;

            ret(0,2) = (b*f - c*e)*de;
            ret(1,2) = (c*d - a*f)*de;
            ret(2,2) = (a*e - b*d)*de;

            return ret;
        }
    };

    template <
        typename EXP
        >
    struct inv_helper<EXP,4>
    {
        static const typename matrix_exp<EXP>::matrix_type inv (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            matrix<type, 4, 4, typename EXP::mem_manager_type> ret;
            const type de = static_cast<type>(1.0/det(m));
            ret(0,0) =  det(removerc<0,0>(m));
            ret(0,1) = -det(removerc<0,1>(m));
            ret(0,2) =  det(removerc<0,2>(m));
            ret(0,3) = -det(removerc<0,3>(m));

            ret(1,0) = -det(removerc<1,0>(m));
            ret(1,1) =  det(removerc<1,1>(m));
            ret(1,2) = -det(removerc<1,2>(m));
            ret(1,3) =  det(removerc<1,3>(m));

            ret(2,0) =  det(removerc<2,0>(m));
            ret(2,1) = -det(removerc<2,1>(m));
            ret(2,2) =  det(removerc<2,2>(m));
            ret(2,3) = -det(removerc<2,3>(m));

            ret(3,0) = -det(removerc<3,0>(m));
            ret(3,1) =  det(removerc<3,1>(m));
            ret(3,2) = -det(removerc<3,2>(m));
            ret(3,3) =  det(removerc<3,3>(m));

            return trans(ret)*de;
        }
    };

    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::matrix_type inv (
        const matrix_exp<EXP>& m
    ) { return inv_helper<EXP,matrix_exp<EXP>::NR>::inv(m); }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::matrix_type cholesky_decomposition (
        const matrix_exp<EXP>& A
    )
    {
        DLIB_ASSERT(A.nr() == A.nc(), 
            "\tconst matrix cholesky_decomposition(const matrix_exp& A)"
            << "\n\tYou can only apply the cholesky_decomposition to a square matrix"
            << "\n\tA.nr(): " << A.nr()
            << "\n\tA.nc(): " << A.nc() 
            );

        typename matrix_exp<EXP>::matrix_type L(A.nr(),A.nc());
        typedef typename EXP::type T;
        set_all_elements(L,0);

        // do nothing if the matrix is empty
        if (A.size() == 0)
            return L;

        // compute the upper left corner
        if (A(0,0) > 0)
            L(0,0) = std::sqrt(A(0,0));

        // compute the first column
        for (long r = 1; r < A.nr(); ++r)
        {
            if (L(0,0) > 0)
                L(r,0) = A(r,0)/L(0,0);
            else
                L(r,0) = A(r,0);
        }

        // now compute all the other columns
        for (long c = 1; c < A.nc(); ++c)
        {
            // compute the diagonal element
            T temp = A(c,c);
            for (long i = 0; i < c; ++i)
            {
                temp -= L(c,i)*L(c,i);
            }
            if (temp > 0)
                L(c,c) = std::sqrt(temp);

            // compute the non diagonal elements
            for (long r = c+1; r < A.nr(); ++r)
            {
                temp = A(r,c);
                for (long i = 0; i < c; ++i)
                {
                    temp -= L(r,i)*L(c,i);
                }
                if (L(c,c) > 0)
                    L(r,c) = temp/L(c,c);
                else
                    L(r,c) = temp;
            }
        }

        return L;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::matrix_type tmp (
        const matrix_exp<EXP>& m
    )
    {
        return typename matrix_exp<EXP>::matrix_type (m);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const typename lazy_disable_if<is_matrix<typename EXP::type>, EXP>::type sum (
        const matrix_exp<EXP>& m
    )
    {
        typedef typename matrix_exp<EXP>::type type;

        type val = 0;
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                val += m(r,c);
            }
        }
        return val;
    }

    template <
        typename EXP
        >
    const typename lazy_enable_if<is_matrix<typename EXP::type>, EXP>::type sum (
        const matrix_exp<EXP>& m
    )
    {
        typedef typename matrix_exp<EXP>::type type;

        type val;
        set_all_elements(val,0);
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                val += m(r,c);
            }
        }
        return val;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::type mean (
        const matrix_exp<EXP>& m
    )
    {
        return sum(m)/(m.nr()*m.nc());
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const typename lazy_disable_if<is_matrix<typename EXP::type>, EXP>::type variance (
        const matrix_exp<EXP>& m
    )
    {
        const typename matrix_exp<EXP>::type avg = mean(m);

        typedef typename matrix_exp<EXP>::type type;

        type val = 0;
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                val += std::pow(m(r,c) - avg,2);
            }
        }

        if (m.nr() * m.nc() == 1)
            return val;
        else
            return val/(m.nr()*m.nc() - 1);
    }

    template <
        typename EXP
        >
    const typename lazy_enable_if<is_matrix<typename EXP::type>, EXP >::type variance (
        const matrix_exp<EXP>& m
    )
    {
        const typename matrix_exp<EXP>::type avg = mean(m);

        typedef typename matrix_exp<EXP>::type type;

        type val;
        set_all_elements(val,0);
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                val += pow(m(r,c) - avg,2);
            }
        }

        if (m.nr() * m.nc() == 1)
            return val;
        else
            return val/(m.nr()*m.nc() - 1);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const matrix<typename EXP::type::type, EXP::type::NR, EXP::type::NR, typename EXP::mem_manager_type> covariance (
        const matrix_exp<EXP>& m
    )
    {
        // perform static checks to make sure m is a column vector 
        COMPILE_TIME_ASSERT(EXP::NR == 0 || EXP::NR > 1);
        COMPILE_TIME_ASSERT(EXP::NC == 1 || EXP::NC == 0);

        // perform static checks to make sure the matrices contained in m are column vectors
        COMPILE_TIME_ASSERT(EXP::type::NC == 1 || EXP::type::NC == 0 );

        DLIB_ASSERT(m.nr() > 1 && m.nc() == 1, 
            "\tconst matrix covariance(const matrix_exp& m)"
            << "\n\tYou can only apply covariance() to a column matrix"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            );
#ifdef ENABLE_ASSERTS
        for (long i = 0; i < m.nr(); ++i)
        {
            DLIB_ASSERT(m(0).nr() == m(i).nr() && m(i).nr() > 0 && m(i).nc() == 1, 
                   "\tconst matrix covariance(const matrix_exp& m)"
                   << "\n\tYou can only apply covariance() to a column matrix of column matrices"
                   << "\n\tm(0).nr(): " << m(0).nr()
                   << "\n\tm(i).nr(): " << m(i).nr() 
                   << "\n\tm(i).nc(): " << m(i).nc() 
                   << "\n\ti:         " << i 
                );
        }
#endif

        // now perform the actual calculation of the covariance matrix.
        matrix<typename EXP::type::type, EXP::type::NR, EXP::type::NR, typename EXP::mem_manager_type> cov(m(0).nr(),m(0).nr());
        set_all_elements(cov,0);

        const matrix<double,4,1, typename EXP::mem_manager_type> avg = mean(m);

        for (long r = 0; r < m.nr(); ++r)
        {
            cov += (m(r) - avg)*trans(m(r) - avg);
        }

        cov *= 1.0 / (m.nr() - 1.0);
        return cov;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP
        >
    const typename matrix_exp<EXP>::type prod (
        const matrix_exp<EXP>& m
    )
    {
        typedef typename matrix_exp<EXP>::type type;

        type val = 1;
        for (long r = 0; r < m.nr(); ++r)
        {
            for (long c = 0; c < m.nc(); ++c)
            {
                val *= m(r,c);
            }
        }
        return val;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP,
        long N = EXP::NR
        >
    struct det_helper
    {
        static const typename matrix_exp<EXP>::type det (
            const matrix_exp<EXP>& m
        )
        {
            using namespace nric;
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC ||
                                matrix_exp<EXP>::NR == 0 ||
                                matrix_exp<EXP>::NC == 0 
                                );
            DLIB_ASSERT(m.nr() == m.nc(), 
                "\tconst matrix_exp::type det(const matrix_exp& m)"
                << "\n\tYou can only apply det() to a square matrix"
                << "\n\tm.nr(): " << m.nr()
                << "\n\tm.nc(): " << m.nc() 
                );
            typedef typename matrix_exp<EXP>::type type;
            typedef typename matrix_exp<EXP>::mem_manager_type MM;

            matrix<type, N, N,MM> lu(m);
            matrix<long,N,1,MM> indx(m.nr(),1);
            matrix<type,N,1,MM> vv(m.nr(),1);
            type d;
            if (ludcmp(lu,indx,d,vv) == false)
            {
                // the matrix is singular so its det is 0
                return 0;
            }

            return prod(diag(lu))*d;
        }
    };

    template <
        typename EXP
        >
    struct det_helper<EXP,1>
    {
        static const typename matrix_exp<EXP>::type det (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            return m(0);
        }
    };

    template <
        typename EXP
        >
    struct det_helper<EXP,2>
    {
        static const typename matrix_exp<EXP>::type det (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            return m(0,0)*m(1,1) - m(0,1)*m(1,0);
        }
    };

    template <
        typename EXP
        >
    struct det_helper<EXP,3>
    {
        static const typename matrix_exp<EXP>::type det (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            type temp = m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) -
                        m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) +
                        m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0));
            return temp;
        }
    };


    template <
        typename EXP
        >
    inline const typename matrix_exp<EXP>::type det (
        const matrix_exp<EXP>& m
    ) { return det_helper<EXP>::det(m); }


    template <
        typename EXP
        >
    struct det_helper<EXP,4>
    {
        static const typename matrix_exp<EXP>::type det (
            const matrix_exp<EXP>& m
        )
        {
            COMPILE_TIME_ASSERT(matrix_exp<EXP>::NR == matrix_exp<EXP>::NC);
            typedef typename matrix_exp<EXP>::type type;

            type temp = m(0,0)*(dlib::det(removerc<0,0>(m))) -
                        m(0,1)*(dlib::det(removerc<0,1>(m))) +
                        m(0,2)*(dlib::det(removerc<0,2>(m))) -
                        m(0,3)*(dlib::det(removerc<0,3>(m)));
            return temp;
        }
    };

// ----------------------------------------------------------------------------------------

    template <
        typename T 
        >
    struct op_uniform_matrix_3 : has_nondestructive_aliasing 
    {
        const static long NR = 0;
        const static long NC = 0;
        typedef typename memory_manager<char>::kernel_1a mem_manager_type;
        typedef T type;
        static type apply (const T& val, long r, long c)
        { return val; }
    };

    template <
        typename T
        >
    const matrix_exp<dynamic_matrix_scalar_unary_exp<T,op_uniform_matrix_3<T> > > uniform_matrix (
        long nr,
        long nc,
        const T& val
    )
    {
        DLIB_ASSERT(nr > 0 && nc > 0, 
            "\tconst matrix_exp uniform_matrix<T>(nr, nc)"
            << "\n\tnr and nc have to be bigger than 0"
            << "\n\tnr: " << nr
            << "\n\tnc: " << nc
            );
        typedef dynamic_matrix_scalar_unary_exp<T,op_uniform_matrix_3<T> > exp;
        return matrix_exp<exp>(exp(nr,nc,val));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename T, 
        long NR_, 
        long NC_ 
        >
    struct op_uniform_matrix_2 : has_nondestructive_aliasing 
    {
        const static long NR = NR_;
        const static long NC = NC_;
        typedef typename memory_manager<char>::kernel_1a mem_manager_type;
        typedef T type;
        static type apply (const T& val, long r, long c)
        { return val; }
    };

    template <
        typename T,
        long NR, 
        long NC
        >
    const matrix_exp<matrix_scalar_unary_exp<T,op_uniform_matrix_2<T,NR,NC> > > uniform_matrix (
        const T& val
    )
    {
        COMPILE_TIME_ASSERT(NR > 0 && NC > 0);

        typedef matrix_scalar_unary_exp<T,op_uniform_matrix_2<T,NR,NC> > exp;
        return matrix_exp<exp>(exp(val));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename T, 
        long NR_, 
        long NC_, 
        T val
        >
    struct op_uniform_matrix : has_nondestructive_aliasing
    {
        const static long NR = NR_;
        const static long NC = NC_;
        typedef typename memory_manager<char>::kernel_1a mem_manager_type;
        typedef T type;
        static type apply ( long r, long c)
        { return val; }
    };

    template <
        typename T, 
        long NR, 
        long NC, 
        T val
        >
    const matrix_exp<matrix_zeroary_exp<op_uniform_matrix<T,NR,NC,val> > > uniform_matrix (
    )
    {
        COMPILE_TIME_ASSERT(NR > 0 && NC > 0);
        typedef matrix_zeroary_exp<op_uniform_matrix<T,NR,NC,val> > exp;
        return matrix_exp<exp>(exp());
    }

// ----------------------------------------------------------------------------------------

    template <
        typename T, 
        long N
        >
    struct op_identity_matrix : has_nondestructive_aliasing
    {
        const static long NR = N;
        const static long NC = N;
        typedef typename memory_manager<char>::kernel_1a mem_manager_type;
        typedef T type;
        static type apply ( long r, long c)
        { return static_cast<type>(r == c); }

        template <typename M>
        static long nr (const M&) { return NR; }
        template <typename M>
        static long nc (const M&) { return NC; }
    };

    template <
        typename T, 
        long N
        >
    const matrix_exp<matrix_zeroary_exp<op_identity_matrix<T,N> > > identity_matrix (
    )
    {
        COMPILE_TIME_ASSERT(N > 0);

        typedef matrix_zeroary_exp<op_identity_matrix<T,N> > exp;
        return matrix_exp<exp>(exp());
    }

// ----------------------------------------------------------------------------------------

    template <long R, long C, typename EXP>
    struct op_rotate : has_destructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;
        template <typename M>
        static type apply ( const M& m, long r, long c)
        { return m((r+R)%m.nr(),(c+C)%m.nc()); }
    };

    template <
        long R,
        long C,
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_rotate<R,C,EXP> > > rotate (
        const matrix_exp<EXP>& m
    )
    {
        // You can't rotate a matrix by more rows than it has.
        COMPILE_TIME_ASSERT(R < EXP::NR || EXP::NR == 0);
        // You can't rotate a matrix by more columns than it has.
        COMPILE_TIME_ASSERT(C < EXP::NC || EXP::NC == 0);
        DLIB_ASSERT( R < m.nr() && C < m.nc(),
            "\tconst matrix_exp::type rotate(const matrix_exp& m)"
            << "\n\tYou can't rotate a matrix by more rows or columns than it has"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\tR:      " << R 
            << "\n\tC:      " << C 
            );
        typedef matrix_unary_exp<matrix_exp<EXP>,op_rotate<R,C,EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP1, typename EXP2, typename EXP3 = void, typename EXP4 = void>
    struct op_pointwise_multiply : public has_nondestructive_aliasing, public preserves_dimensions<EXP1,EXP2,EXP3,EXP4>
    {
        typedef typename EXP1::type type;

        template <typename M1, typename M2>
        static type apply ( const M1& m1, const M2& m2 , long r, long c)
        { return m1(r,c)*m2(r,c); }
    };

    template <
        typename EXP1,
        typename EXP2
        >
    inline const matrix_exp<matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > > pointwise_multiply (
        const matrix_exp<EXP1>& a,
        const matrix_exp<EXP2>& b 
    )
    {
        COMPILE_TIME_ASSERT((is_same_type<typename EXP1::type,typename EXP2::type>::value == true));
        COMPILE_TIME_ASSERT(EXP1::NR == EXP2::NR || EXP1::NR == 0 || EXP2::NR == 0);
        COMPILE_TIME_ASSERT(EXP1::NC == EXP2::NC || EXP1::NC == 0 || EXP2::NC == 0);
        DLIB_ASSERT(a.nr() == b.nr() &&
               a.nc() == b.nc(), 
            "\tconst matrix_exp::type pointwise_multiply(const matrix_exp& a, const matrix_exp& b)"
            << "\n\tYou can only make a do a pointwise multiply with two equally sized matrices"
            << "\n\ta.nr(): " << a.nr()
            << "\n\ta.nc(): " << a.nc() 
            << "\n\tb.nr(): " << b.nr()
            << "\n\tb.nc(): " << b.nc() 
            );
        typedef matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > exp;
        return matrix_exp<exp>(exp(a,b));
    }

    template <
        typename EXP1,
        typename EXP2,
        typename EXP3
        >
    inline const matrix_exp<
        matrix_binary_exp< matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > ,
                          matrix_exp<EXP3>, op_pointwise_multiply<EXP1,EXP2,EXP3> > >
        pointwise_multiply (
        const matrix_exp<EXP1>& a,
        const matrix_exp<EXP2>& b, 
        const matrix_exp<EXP3>& c
    )
    {
        COMPILE_TIME_ASSERT((is_same_type<typename EXP1::type,typename EXP2::type>::value == true));
        COMPILE_TIME_ASSERT(EXP1::NR == EXP2::NR || EXP1::NR == 0 || EXP2::NR == 0);
        COMPILE_TIME_ASSERT(EXP1::NC == EXP2::NC || EXP1::NR == 0 || EXP2::NC == 0);
        COMPILE_TIME_ASSERT(EXP2::NR == EXP3::NR || EXP2::NR == 0 || EXP3::NR == 0);
        COMPILE_TIME_ASSERT(EXP2::NC == EXP3::NC || EXP2::NC == 0 || EXP3::NC == 0);
        DLIB_ASSERT(a.nr() == b.nr() &&
               a.nc() == b.nc() &&
               b.nr() == c.nr() &&
               b.nc() == c.nc(), 
            "\tconst matrix_exp::type pointwise_multiply(a,b,c)"
            << "\n\tYou can only make a do a pointwise multiply between equally sized matrices"
            << "\n\ta.nr(): " << a.nr()
            << "\n\ta.nc(): " << a.nc() 
            << "\n\tb.nr(): " << b.nr()
            << "\n\tb.nc(): " << b.nc() 
            << "\n\tc.nr(): " << c.nr()
            << "\n\tc.nc(): " << c.nc() 
            );
        typedef  matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > exp; 
        typedef matrix_binary_exp< exp , matrix_exp<EXP3>, op_pointwise_multiply<EXP1,EXP2,EXP3> > exp2;

        return matrix_exp<exp2>(exp2(exp(a,b),c));
    }

    template <
        typename EXP1,
        typename EXP2,
        typename EXP3,
        typename EXP4
        >
    inline const matrix_exp<
        matrix_binary_exp< matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > ,
                          matrix_binary_exp<matrix_exp<EXP3>,matrix_exp<EXP4>,op_pointwise_multiply<EXP3,EXP4> >, 
                          op_pointwise_multiply<EXP1,EXP2,EXP3,EXP4> > >
        pointwise_multiply (
        const matrix_exp<EXP1>& a,
        const matrix_exp<EXP2>& b, 
        const matrix_exp<EXP3>& c,
        const matrix_exp<EXP4>& d
    )
    {
        COMPILE_TIME_ASSERT((is_same_type<typename EXP1::type,typename EXP2::type>::value == true));
        COMPILE_TIME_ASSERT(EXP1::NR == EXP2::NR || EXP1::NR == 0 || EXP2::NR == 0);
        COMPILE_TIME_ASSERT(EXP1::NC == EXP2::NC || EXP1::NC == 0 || EXP2::NC == 0 );
        COMPILE_TIME_ASSERT(EXP2::NR == EXP3::NR || EXP2::NR == 0 || EXP3::NR == 0);
        COMPILE_TIME_ASSERT(EXP2::NC == EXP3::NC || EXP2::NC == 0 || EXP3::NC == 0);
        COMPILE_TIME_ASSERT(EXP3::NR == EXP4::NR || EXP3::NR == 0 || EXP4::NR == 0);
        COMPILE_TIME_ASSERT(EXP3::NC == EXP4::NC || EXP3::NC == 0 || EXP4::NC == 0);
        DLIB_ASSERT(a.nr() == b.nr() &&
               a.nc() == b.nc() &&
               b.nr() == c.nr() &&
               b.nc() == c.nc() &&
               c.nr() == d.nr() &&
               c.nc() == d.nc(), 
            "\tconst matrix_exp::type pointwise_multiply(a,b,c,d)"
            << "\n\tYou can only make a do a pointwise multiply between equally sized matrices"
            << "\n\ta.nr(): " << a.nr()
            << "\n\ta.nc(): " << a.nc() 
            << "\n\tb.nr(): " << b.nr()
            << "\n\tb.nc(): " << b.nc() 
            << "\n\tc.nr(): " << c.nr()
            << "\n\tc.nc(): " << c.nc() 
            << "\n\td.nr(): " << d.nr()
            << "\n\td.nc(): " << d.nc() 
            );
        typedef matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_pointwise_multiply<EXP1,EXP2> > exp1;
        typedef matrix_binary_exp<matrix_exp<EXP3>,matrix_exp<EXP4>,op_pointwise_multiply<EXP3,EXP4> > exp2;

        typedef matrix_binary_exp<  exp1  ,  exp2, op_pointwise_multiply<EXP1,EXP2,EXP3,EXP4> > exp3;
        return matrix_exp<exp3>(exp3(exp1(a,b),exp2(c,d)));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename P,
        int type = static_switch<
            pixel_traits<P>::grayscale,
            pixel_traits<P>::rgb,
            pixel_traits<P>::hsi,
            pixel_traits<P>::rgb_alpha
            >::value
        >
    struct pixel_to_vector_helper;

    template <typename P>
    struct pixel_to_vector_helper<P,1>
    {
        template <typename M>
        static void assign (
            M& m,
            const P& pixel
        )
        {
            m(0) = static_cast<typename M::type>(pixel);
        }
    };

    template <typename P>
    struct pixel_to_vector_helper<P,2>
    {
        template <typename M>
        static void assign (
            M& m,
            const P& pixel
        )
        {
            m(0) = static_cast<typename M::type>(pixel.red);
            m(1) = static_cast<typename M::type>(pixel.green);
            m(2) = static_cast<typename M::type>(pixel.blue);
        }
    };

    template <typename P>
    struct pixel_to_vector_helper<P,3>
    {
        template <typename M>
        static void assign (
            M& m,
            const P& pixel
        )
        {
            m(0) = static_cast<typename M::type>(pixel.h);
            m(1) = static_cast<typename M::type>(pixel.s);
            m(2) = static_cast<typename M::type>(pixel.i);
        }
    };

    template <typename P>
    struct pixel_to_vector_helper<P,4>
    {
        template <typename M>
        static void assign (
            M& m,
            const P& pixel
        )
        {
            m(0) = static_cast<typename M::type>(pixel.red);
            m(1) = static_cast<typename M::type>(pixel.green);
            m(2) = static_cast<typename M::type>(pixel.blue);
            m(3) = static_cast<typename M::type>(pixel.alpha);
        }
    };


    template <
        typename T,
        typename P
        >
    inline const matrix<T,pixel_traits<P>::num,1> pixel_to_vector (
        const P& pixel
    )
    {
        COMPILE_TIME_ASSERT(pixel_traits<P>::num > 0);
        matrix<T,pixel_traits<P>::num,1> m;
        pixel_to_vector_helper<P>::assign(m,pixel);
        return m;
    }

// ----------------------------------------------------------------------------------------

    template <
        typename P,
        int type = static_switch<
            pixel_traits<P>::grayscale,
            pixel_traits<P>::rgb,
            pixel_traits<P>::hsi,
            pixel_traits<P>::rgb_alpha
            >::value
        >
    struct vector_to_pixel_helper;

    template <typename P>
    struct vector_to_pixel_helper<P,1>
    {
        template <typename M>
        static void assign (
            P& pixel,
            const M& m
        )
        {
            pixel = static_cast<unsigned char>(m(0));
        }
    };

    template <typename P>
    struct vector_to_pixel_helper<P,2>
    {
        template <typename M>
        static void assign (
            P& pixel,
            const M& m
        )
        {
            pixel.red = static_cast<unsigned char>(m(0));
            pixel.green = static_cast<unsigned char>(m(1));
            pixel.blue = static_cast<unsigned char>(m(2));
        }
    };

    template <typename P>
    struct vector_to_pixel_helper<P,3>
    {
        template <typename M>
        static void assign (
            P& pixel,
            const M& m
        )
        {
            pixel.h = static_cast<unsigned char>(m(0));
            pixel.s = static_cast<unsigned char>(m(1));
            pixel.i = static_cast<unsigned char>(m(2));
        }
    };

    template <typename P>
    struct vector_to_pixel_helper<P,4>
    {
        template <typename M>
        static void assign (
            P& pixel,
            const M& m
        )
        {
            pixel.red = static_cast<unsigned char>(m(0));
            pixel.green = static_cast<unsigned char>(m(1));
            pixel.blue = static_cast<unsigned char>(m(2));
            pixel.alpha = static_cast<unsigned char>(m(3));
        }
    };

    template <
        typename P,
        typename EXP
        >
    inline void vector_to_pixel (
        P& pixel,
        const matrix_exp<EXP>& vector 
    )
    {
        COMPILE_TIME_ASSERT(pixel_traits<P>::num == matrix_exp<EXP>::NR);
        COMPILE_TIME_ASSERT(matrix_exp<EXP>::NC == 1);
        vector_to_pixel_helper<P>::assign(pixel,vector);
    }

// ----------------------------------------------------------------------------------------

    template <long lower, long upper, typename EXP>
    struct op_clamp : has_nondestructive_aliasing, preserves_dimensions<EXP>
    {
        typedef typename EXP::type type;

        template <typename M>
        static type apply ( const M& m, long r, long c)
        { 
            const type temp = m(r,c);
            if (temp > static_cast<type>(upper))
                return static_cast<type>(upper);
            else if (temp < static_cast<type>(lower))
                return static_cast<type>(lower);
            else
                return temp;
        }
    };

    template <
        long l, 
        long u,
        typename EXP
        >
    const matrix_exp<matrix_unary_exp<matrix_exp<EXP>,op_clamp<l,u,EXP> > > clamp (
        const matrix_exp<EXP>& m
    )
    {
        typedef matrix_unary_exp<matrix_exp<EXP>,op_clamp<l,u,EXP> > exp;
        return matrix_exp<exp>(exp(m));
    }

// ----------------------------------------------------------------------------------------

    template <
        typename EXP1,
        typename EXP2
        >
    bool equal (
        const matrix_exp<EXP1>& a,
        const matrix_exp<EXP2>& b,
        const typename EXP1::type eps = 100*std::numeric_limits<typename EXP1::type>::epsilon()
    )
    {
        // check if the dimensions don't match
        if (a.nr() != b.nr() || a.nc() != b.nc())
            return false;

        for (long r = 0; r < a.nr(); ++r)
        {
            for (long c = 0; c < a.nc(); ++c)
            {
                if (std::abs(a(r,c)-b(r,c)) > eps)
                    return false;
            }
        }

        // no non-equal points found so we return true 
        return true;
    }

// ----------------------------------------------------------------------------------------

    template <typename EXP1, typename EXP2>
    struct op_scale_columns : has_nondestructive_aliasing
    {
        typedef typename EXP1::type type;
        typedef typename EXP1::mem_manager_type mem_manager_type;
        const static long NR = EXP1::NR;
        const static long NC = EXP1::NC;

        template <typename M1, typename M2>
        static type apply ( const M1& m1, const M2& m2 , long r, long c)
        { return m1(r,c)*m2(c); }

        template <typename M1, typename M2>
        static long nr (const M1& m1, const M2& ) { return m1.nr(); }
        template <typename M1, typename M2>
        static long nc (const M1& m1, const M2& ) { return m1.nc(); }
    };

    template <
        typename EXP1,
        typename EXP2
        >
    const matrix_exp<matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_scale_columns<EXP1,EXP2> > > scale_columns (
        const matrix_exp<EXP1>& m,
        const matrix_exp<EXP2>& v 
    )
    {
        COMPILE_TIME_ASSERT((is_same_type<typename EXP1::type,typename EXP2::type>::value == true));
        COMPILE_TIME_ASSERT(EXP2::NC == 1 || EXP2::NC == 0);
        COMPILE_TIME_ASSERT(EXP1::NC == EXP2::NR || EXP1::NC == 0 || EXP2::NR == 0);

        DLIB_ASSERT(v.nc() == 1 && v.nr() == m.nc(), 
            "\tconst matrix_exp scale_columns(m, v)"
            << "\n\tv must be a column vector and its length must match the number of columns in m"
            << "\n\tm.nr(): " << m.nr()
            << "\n\tm.nc(): " << m.nc() 
            << "\n\tv.nr(): " << v.nr()
            << "\n\tv.nc(): " << v.nc() 
            );
        typedef matrix_binary_exp<matrix_exp<EXP1>,matrix_exp<EXP2>,op_scale_columns<EXP1,EXP2> > exp;
        return matrix_exp<exp>(exp(m,v));
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_MATRIx_UTILITIES_

