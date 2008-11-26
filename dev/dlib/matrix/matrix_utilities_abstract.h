// Copyright (C) 2006  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_MATRIx_UTILITIES_ABSTRACT_
#ifdef DLIB_MATRIx_UTILITIES_ABSTRACT_

#include "matrix_abstract.h"
#include <complex>
#include "pixel.h"
#include "../geometry.h"
#inclue <vector>

namespace dlib
{

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
//                             Elementary matrix operations
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    const matrix_exp diag (
        const matrix_exp& m
    );
    /*!
        requires
            - m is a square matrix
        ensures
            - returns a column vector R that contains the elements from the diagonal 
              of m in the order R(0)==m(0,0), R(1)==m(1,1), R(2)==m(2,2) and so on.
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp trans (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the transpose of the matrix m
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename T, 
        long NR, 
        long NC, 
        T val
        >
    const matrix_exp uniform_matrix (
    );
    /*!
        requires
            - NR > 0 && NC > 0
        ensures
            - returns an NR by NC matrix with elements of type T and all set to val.
    !*/

// ----------------------------------------------------------------------------------------

    template <
        long NR, 
        long NC,
        typename T
        >
    const matrix_exp uniform_matrix (
        const T& val
    );
    /*!
        requires
            - NR > 0 && NC > 0
        ensures
            - returns an NR by NC matrix with elements of type T and all set to val.
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename T
        >
    const matrix_exp uniform_matrix (
        long nr,
        long nc,
        const T& val
    );
    /*!
        requires
            - nr > 0 && nc > 0
        ensures
            - returns an nr by nc matrix with elements of type T and all set to val.
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename T, 
        long N
        >
    const matrix_exp identity_matrix (
    );
    /*!
        requires
            - N > 0
        ensures
            - returns an N by N identity matrix with elements of type T.
    !*/

// ----------------------------------------------------------------------------------------

    template <
        long R,
        long C
        >
    const matrix_exp rotate (
        const matrix_exp& m
    );
    /*!
        requires
            - R < m.nr()
            - C < m.nc()
        ensures
            - returns a matrix R such that:
                - R::type == the same type that was in m
                - R has the same dimensions as m
                - for all valid r and c:
                  R( (r+R)%m.nr() , (c+C)%m.nc() ) == m(r,c)
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename vector_type
        >
    const matrix_exp vector_to_matrix (
        const vector_type& vector
    );
    /*!
        requires
            - vector_type is an implementation of array/array_kernel_abstract.h or
              std::vector or dlib::std_vector_c
        ensures
            - returns a matrix R such that:
                - R.nr() == vector.size() 
                - R.nc() == 1 
                - for all valid r:
                  R(r) == vector[r]
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename array_type
        >
    const matrix_exp array_to_matrix (
        const array_type& array
    );
    /*!
        requires
            - array_type is an implementation of array2d/array2d_kernel_abstract.h
        ensures
            - returns a matrix R such that:
                - R.nr() == array.nr() 
                - R.nc() == array.nc()
                - for all valid r and c:
                  R(r, c) == array[r][c]
    !*/

// ----------------------------------------------------------------------------------------

    const rectangle get_rect (  
        const matrix_exp& m
    );
    /*!
        ensures
            - returns rectangle(0, 0, m.nc()-1, m.nr()-1)
              (i.e. returns a rectangle that has the same dimensions as
              the matrix m)
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp subm (
        const matrix_exp& m,
        long row,
        long col,
        long nr,
        long nc
    );
    /*!
        requires
            - row >= 0
            - row + nr < m.nr()
            - col >= 0
            - col + nc < m.nc()
        ensures
            - returns a matrix R such that:
                - R.nr() == nr 
                - R.nc() == nc
                - for all valid r and c:
                  R(r, c) == m(r+row,c+col)
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp subm (
        const matrix_exp& m,
        const rectangle& rect
    );
    /*!
        requires
            - get_rect(m).contains(rect) == true
              (i.e. rect is a region inside the matrix m)
        ensures
            - returns a matrix R such that:
                - R.nr() == rect.height()  
                - R.nc() == rect.width()
                - for all valid r and c:
                  R(r, c) == m(r+rect.top(), c+rect.left())
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp rowm (
        const matrix_exp& m,
        long row
    );
    /*!
        requires
            - 0 <= row < m.nr()
        ensures
            - returns a matrix R such that:
                - R.nr() == 1
                - R.nc() == m.nc()
                - for all valid i:
                  R(i) == m(row,i)
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp colm (
        const matrix_exp& m,
        long col 
    );
    /*!
        requires
            - 0 <= col < m.nr()
        ensures
            - returns a matrix R such that:
                - R.nr() == m.nr() 
                - R.nc() == 1
                - for all valid i:
                  R(i) == m(i,col)
    !*/

// ----------------------------------------------------------------------------------------

    template <
        long R,
        long C
        >
    const matrix_exp removerc (
        const matrix_exp& m
    );
    /*!
        requires
            - m.nr() > 1
            - m.nc() > 1
        ensures
            - returns a matrix R such that:
                - R.nr() == m.nr() - 1
                - R.nc() == m.nc() - 1
                - R == m with its R row and C column removed
    !*/

// ----------------------------------------------------------------------------------------

    template <
       typename target_type
       >
    const matrix_exp matrix_cast (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns a matrix R where for all valid r and c:
              R(r,c) == static_cast<target_type>(m(r,c))
              also, R has the same dimensions as m.
    !*/

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
    );
    /*!
        ensures
            - for all valid r and c:
              m(r,c) == value
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::matrix_type tmp (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns a temporary matrix object that is a copy of m. 
              (This is useful because it allows you to easily force a matrix_exp to 
              fully evaluate before giving it to some other function that queries
              the elements of the matrix more than once each, such as the matrix
              multiplication operator.)
    !*/

// ----------------------------------------------------------------------------------------

    bool equal (
        const matrix_exp& a,
        const matrix_exp& b,
        const matrix_exp::type epsilon = 100*std::numeric_limits<matrix_exp::type>::epsilon()
    );
    /*!
        ensures
            - if (a and b don't have the same dimensions) then
                - returns false
            - else if (there exists an r and c such that abs(a(r,c)-b(r,c)) > epsilon) then
                - returns false
            - else
                - returns true
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp pointwise_multiply (
        const matrix_exp& a,
        const matrix_exp& b 
    );
    /*!
        requires
            - a.nr() == b.nr()
            - a.nc() == b.nc()
            - a and b both contain the same type of element
        ensures
            - returns a matrix R such that:
                - R::type == the same type that was in a and b.
                - R has the same dimensions as a and b. 
                - for all valid r and c:
                  R(r,c) == a(r,c) * b(r,c)
    !*/

    const matrix_exp pointwise_multiply (
        const matrix_exp& a,
        const matrix_exp& b,
        const matrix_exp& c 
    );
    /*!
        performs pointwise_multiply(a,pointwise_multiply(b,c));
    !*/

    const matrix_exp pointwise_multiply (
        const matrix_exp& a,
        const matrix_exp& b,
        const matrix_exp& c,
        const matrix_exp& d 
    );
    /*!
        performs pointwise_multiply(pointwise_multiply(a,b),pointwise_multiply(c,d));
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp scale_columns (
        const matrix_exp& m,
        const matrix_exp& v
    );
    /*!
        requires
            - v.nc() == 1 (i.e. v is a column vector)
            - v.nr() == m.nc()
            - m and v both contain the same type of element
        ensures
            - returns a matrix R such that:
                - R::type == the same type that was in m and v.
                - R has the same dimensions as m. 
                - for all valid r and c:
                  R(r,c) == m(r,c) * v(c)
                - i.e. R is the result of multiplying each of m's columns by
                  the corresponding scalar in v.
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
//                             Linear algebra functions 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    const matrix_exp::matrix_type inv (
        const matrix_exp& m
    );
    /*!
        requires
            - m is a square matrix
        ensures
            - returns the inverse of m 
              (Note that if m is singular or so close to being singular that there
              is a lot of numerical error then the returned matrix will be bogus.  
              You can check by seeing if m*inv(m) is an identity matrix)
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::matrix_type pinv (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the Moore-Penrose pseudoinverse of m.
            - The returned matrix has m.nr() columns and m.nc() rows.
    !*/

// ----------------------------------------------------------------------------------------

    void svd (
        const matrix_exp& m,
        matrix<matrix_exp::type>& u,
        matrix<matrix_exp::type>& w,
        matrix<matrix_exp::type>& v
    );
    /*!
        ensures
            - computes the singular value decomposition of m
            - m == #u*#w*trans(#v)
            - trans(#u)*#u == identity matrix
            - trans(#v)*#v == identity matrix
            - #u.nr() == m.nr()
            - #u.nc() == m.nc()
            - #w.nr() == m.nc()
            - #w.nc() == m.nc()
            - #v.nr() == m.nc()
            - #v.nc() == m.nc()
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type det (
        const matrix_exp& m
    );
    /*!
        requires
            - m is a square matrix
        ensures
            - returns the determinant of m
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::matrix_type cholesky_decomposition (
        const matrix_exp& A
    );
    /*!
        requires
            - A is a square matrix
        ensures
            - if (A has a Cholesky Decomposition) then
                - returns the decomposition of A.  That is, returns a matrix L
                  such that L*trans(L) == A.  L will also be lower triangular.
            - else
                - returns a matrix with the same dimensions as A but it 
                  will have a bogus value.  I.e. it won't be a decomposition.
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
//                              Statistics
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    const matrix_exp::type min (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the value of the smallest element of m
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type max (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the value of the biggest element of m
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type sum (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the sum of all elements in m
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type prod (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the results of multiplying all elements of m together. 
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type mean (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the mean of all elements in m. 
              (i.e. returns sum(m)/(m.nr()*m.nc()))
    !*/

// ----------------------------------------------------------------------------------------

    const matrix_exp::type variance (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns the unbiased sample variance of all elements in m 
              (i.e. 1.0/(m.nr()*m.nc() - 1)*(sum of all pow(m(i,j) - mean(m),2)))
    !*/

// ----------------------------------------------------------------------------------------

    const matrix covariance (
        const matrix_exp& m
    );
    /*!
        requires
            - matrix_exp::type == a dlib::matrix object
            - m.nr() > 1
            - m.nc() == 1 (i.e. m is a column vector)
            - for all valid i, j:
                - m(i).nr() > 0
                - m(i).nc() == 1
                - m(i).nr() == m(j).nr() 
                - i.e. m contains only column vectors and all the column vectors
                  have the same non-zero length
        ensures
            - returns the unbiased sample covariance matrix for the set of samples
              in m.  
              (i.e. 1.0/(m.nr()-1)*(sum of all (m(i) - mean(m))*trans(m(i) - mean(m))))
            - the returned matrix will contain elements of type matrix_exp::type::type.
            - the returned matrix will have m(0).nr() rows and columns.
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
//                                 Pixel and Image Utilities
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    template <
        typename T,
        typename P
        >
    const matrix_exp pixel_to_vector (
        const P& pixel
    );
    /*!
        requires
            - pixel_traits<P>::has_alpha == false
        ensures
            - returns a matrix M such that:
                - M::type == T
                - M::NC == 1 
                - M::NR == pixel_traits<P>::num
                - if (pixel_traits<P>::grayscale) then
                    - M(0) == pixel 
                - if (pixel_traits<P>::rgb) then
                    - M(0) == pixel.red 
                    - M(1) == pixel.green 
                    - M(2) == pixel.blue 
                - if (pixel_traits<P>::hsi) then
                    - M(0) == pixel.h 
                    - M(1) == pixel.s 
                    - M(2) == pixel.i 
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename P
        >
    void vector_to_pixel (
        P& pixel,
        const matrix_exp& vector 
    );
    /*!
        requires
            - pixel_traits<P>::has_alpha == false
            - vector::NR == pixel_traits<P>::num
            - vector::NC == 1 
              (i.e. you have to use a statically dimensioned vector)
        ensures
            - if (pixel_traits<P>::grayscale) then
                - pixel == M(0) 
            - if (pixel_traits<P>::rgb) then
                - pixel.red   == M(0)  
                - pixel.green == M(1) 
                - pixel.blue  == M(2)  
            - if (pixel_traits<P>::hsi) then
                - pixel.h == M(0)
                - pixel.s == M(1)
                - pixel.i == M(2)
    !*/

// ----------------------------------------------------------------------------------------

    template <
        long lower,
        long upper 
        >
    const matrix_exp clamp (
        const matrix_exp& m
    );
    /*!
        ensures
            - returns a matrix R such that:
                - R::type == the same type that was in m
                - R has the same dimensions as m
                - for all valid r and c:
                    - if (m(r,c) > upper) then
                        - R(r,c) == upper
                    - else if (m(r,c) < lower) then
                        - R(r,c) == lower
                    - else
                        - R(r,c) == m(r,c)
    !*/

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_MATRIx_UTILITIES_ABSTRACT_

