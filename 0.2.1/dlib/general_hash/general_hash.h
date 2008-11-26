// Copyright (C) 2003  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_GENERAL_HASh_
#define DLIB_GENERAL_HASh_


#include <string>

namespace dlib 
{

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------- provide a general hashing function object ----------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    template <
        typename T
        >
    class general_hash
    {
    public:
        inline unsigned long operator() (
            const T& item
        ) const;
    };

    /*!
        Note that the default behavior of general hash is to attempt to cast
        an object of type T to an unsigned long and use that as the hash.

        REQUIREMENTS ON general_hash
            - must have a default constructor
            - must be a function object which overloads operator() as follows:
              unsigned long operator()(const T& item)             
            - must take item, compute a hash number and return it 
            - must not throw
            - must define the hash in such a way that all equivalent objects have 
              the same hash.  where equivalent means the following:
                  definition of equivalent:
                  a is equivalent to b if
                  a < b == false and
                  b < a == false
    !*/

// ---------------

    template <
        typename T
        >
    unsigned long general_hash<T>:: 
    operator() (
        const T& item
    ) const
    {
        // hash any types that have a conversion to unsigned long
        return static_cast<unsigned long>(item);
    }


// ---------------

    // std::string hash
    template <>
    inline unsigned long general_hash<std::string>:: 
    operator() (
        const std::string& item
    ) const
    {
        unsigned long hash = 0;
        std::string::size_type size = item.size();
        for (std::string::size_type i = 0; i < size; ++i)
            hash = hash*37 + static_cast<unsigned long>(item[i]);
        return hash;
    }

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

}

#endif // DLIB_GENERAL_HASh_

