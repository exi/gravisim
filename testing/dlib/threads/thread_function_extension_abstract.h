// Copyright (C) 2007  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_THREAD_FUNCTIOn_ABSTRACT_
#ifdef DLIB_THREAD_FUNCTIOn_ABSTRACT_ 

#include "threads_kernel_abstract.h"

namespace dlib
{

// ----------------------------------------------------------------------------------------

    class thread_function 
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object represents a thread on a global C++ function.  That is, 
                it allows you to run a global function in its own thread.
        !*/
    public:

        template <typename T>
        thread_function (
            const T& funct
        );
        /*!
            ensures
                - #*this is properly initialized
                - the function object funct has been started in its own thread
            throws
                - std::bad_alloc
                - dlib::thread_error
                    the constructor may throw this exception if there is a problem 
                    gathering resources to create threading objects.
        !*/

        thread_function (
            void (*funct)()
        );
        /*!
            ensures
                - #*this is properly initialized
                - the function pointed to by funct has been started in its own thread
            throws
                - std::bad_alloc
                - dlib::thread_error
                    the constructor may throw this exception if there is a problem 
                    gathering resources to create threading objects.
        !*/

        template <typename T>
        thread_function (
            void (*funct)(T),
            T arg
        );
        /*!
            ensures
                - #*this is properly initialized
                - the function pointed to by funct has been started in its own thread and
                  passed the argument arg
            throws
                - std::bad_alloc
                - dlib::thread_error
                    the constructor may throw this exception if there is a problem 
                    gathering resources to create threading objects.
        !*/

        ~thread_function (
        );
        /*!
            ensures
                - all resources allocated by *this have been freed.  
                - blocks until is_alive() == false
        !*/

        bool is_alive (
        ) const;
        /*!
            ensures
                - if (this object's thread has yet to terminate) then
                    - returns true
                - else
                    - returns false
        !*/

        void wait (
        ) const;
        /*!
            ensures
                - if (is_alive() == true) then
                    - blocks until this object's thread terminates
        !*/

    private:

        // restricted functions
        thread_function(thread_function&);        // copy constructor
        thread_function& operator=(thread_function&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_THREAD_FUNCTIOn_ABSTRACT_


