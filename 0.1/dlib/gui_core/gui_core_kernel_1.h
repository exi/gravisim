// Copyright (C) 2005  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_GUI_CORE_KERNEl_1_
#define DLIB_GUI_CORE_KERNEl_1_

#ifdef DLIB_ISO_CPP_ONLY
#error "DLIB_ISO_CPP_ONLY is defined so you can't use this OS dependent code.  Turn DLIB_ISO_CPP_ONLY off if you want to use it."
#endif

#ifdef DLIB_NO_GUI_SUPPORT
#error "DLIB_NO_GUI_SUPPORT is defined so you can't use the GUI code.  Turn DLIB_NO_GUI_SUPPORT off if you want to use it."
#endif

#include <string>
#include "../windows_magic.h"


#include <windows.h>
#include <winuser.h>
#include <windowsx.h>
#include <commctrl.h>

#include "gui_core_kernel_abstract.h"

#ifdef _MSC_VER
// Disable the following warnings for Visual Studio
//
// These two warnings have to do with converting points to and from the LONG
// type.  But both these types are 32 bits in windows so it is fine.
#pragma warning(disable: 4244; disable: 4312)
#endif 

#include "../algs.h"
#include "../sync_extension.h"
#include "../binary_search_tree.h"
#include "../threads.h"
#include "../geometry/rectangle.h"
#include "../assert.h"
#include "../queue.h"
#include "../pixel.h"


namespace dlib
{

// ----------------------------------------------------------------------------------------

    class base_window;
    namespace gui_core_kernel_1_globals
    {

        LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

    }

// ----------------------------------------------------------------------------------------

    class canvas : public rectangle
    {
    public:
        struct pixel
        {
            unsigned char blue;
            unsigned char green;
            unsigned char red;
        };

        ~canvas() { }

        inline pixel* operator[] (
            unsigned long row
        ) const
        {
            DLIB_ASSERT(row < height(),
                "\tpixel* canvas::operator[]"
                << "\n\tyou have to give a row that is less than the height()"
                << "\n\tthis:     " << this
                << "\n\trow:      " << row 
                << "\n\theight(): " << height() 
                );
            unsigned char* temp = bits + row_width*row;
            return reinterpret_cast<pixel*>(temp);
        }

        void fill (
            unsigned char red_,
            unsigned char green_,
            unsigned char blue_
        ) const;

    private:

        friend LRESULT CALLBACK gui_core_kernel_1_globals::WndProc (HWND, UINT, WPARAM, LPARAM);

        canvas (
            unsigned char* bits__,
            unsigned long padding__,
            unsigned long left__,
            unsigned long top__,            
            unsigned long right__,            
            unsigned long bottom__           
        ) : 
            rectangle(left__,top__,right__,bottom__),
            bits(bits__),
            width_(width()),
            height_(height()),
            row_width(width_*3+padding__)
        {}

        // restricted functions
        canvas();        // normal constructor
        canvas(canvas&);        // copy constructor
        canvas& operator=(canvas&);    // assignment operator    

        unsigned char* const bits;
        const unsigned long width_;
        const unsigned long height_;
        const unsigned long row_width;
    };

    template <>
    struct pixel_traits<canvas::pixel>
    {
        const static bool rgb  = true;
        const static bool rgb_alpha  = false;
        const static bool grayscale = false;
        const static bool hsi = false;
        const static long num = 3;
        static unsigned long max() { return 255;}
        const static bool has_alpha = false;
    };

// ----------------------------------------------------------------------------------------

    void put_on_clipboard (
        const std::string& str
    );

// ----------------------------------------------------------------------------------------

    void get_from_clipboard (
        std::string& str
    );

// ----------------------------------------------------------------------------------------

    class base_window
    {
        friend LRESULT CALLBACK gui_core_kernel_1_globals::WndProc (HWND, UINT, WPARAM, LPARAM);

        HWND hwnd;
        DWORD style;
        bool has_been_destroyed;

        // This is true if the mouse is in this window.  false otherwise.
        // also note that this variable is only accessed from the event handling thread
        // (except for being initialized below in the constructor, but that is inside
        // the window_table mutex so it doesn't matter).
        bool mouse_in;

        // this is a copy of the last inputs we sent to the on_mouse_move() event.
        long prevx;
        long prevy;
        long prev_state;

    protected:
        const rmutex& wm;

    public:

        base_window (
            bool resizable = true,
            bool undecorated = false
        );

        virtual ~base_window (
        );

        void close_window (
        );

        bool is_closed (
        ) const;

        void set_title (
            const std::string& title
        );

        virtual void show (
        );    

        virtual void hide(
        );    

        void set_size (
            int width_,
            int height_
        );

        void set_pos (
            long x_,
            long y_
        );

        void get_pos (
            long& x_,
            long& y_
        );

        void get_size (
            unsigned long& width,
            unsigned long& height
        ) const;

        void get_display_size (
            unsigned long& width,
            unsigned long& height
        ) const;

        void invalidate_rectangle (
            const rectangle& rect
        );

        void trigger_user_event (
            void* p,
            int i
        );

        void wait_until_closed (
        ) const;

        enum on_close_return_code
        {
            DO_NOT_CLOSE_WINDOW,
            CLOSE_WINDOW
        };

        enum mouse_state_masks
        {
            NONE = 0,
            LEFT = 1,
            RIGHT = 2,
            MIDDLE = 4,
            SHIFT = 8,
            CONTROL = 16
        };

        enum keyboard_state_masks
        {
            KBD_MOD_NONE = 0,
            KBD_MOD_SHIFT = 1,
            KBD_MOD_CONTROL = 2,
            KBD_MOD_ALT = 4,
            KBD_MOD_META = 8,
            KBD_MOD_CAPS_LOCK = 16,
            KBD_MOD_NUM_LOCK = 32,
            KBD_MOD_SCROLL_LOCK = 64
        };

        enum non_printable_keyboard_keys
        {
            KEY_BACKSPACE,
            KEY_SHIFT,
            KEY_CTRL,
            KEY_ALT,
            KEY_PAUSE,
            KEY_CAPS_LOCK,
            KEY_ESC,
            KEY_PAGE_UP,
            KEY_PAGE_DOWN,
            KEY_END,
            KEY_HOME,
            KEY_LEFT,           // This is the left arrow key
            KEY_RIGHT,          // This is the right arrow key
            KEY_UP,             // This is the up arrow key
            KEY_DOWN,           // This is the down arrow key
            KEY_INSERT,
            KEY_DELETE,
            KEY_SCROLL_LOCK,
  
            // Function Keys
            KEY_F1,
            KEY_F2,
            KEY_F3,
            KEY_F4,
            KEY_F5,
            KEY_F6,
            KEY_F7,
            KEY_F8,
            KEY_F9,
            KEY_F10,
            KEY_F11,
            KEY_F12
        };

    protected:

        virtual on_close_return_code on_window_close(
        ){return CLOSE_WINDOW;}

        virtual void on_user_event (
            void* p,
            int i
        ){}

        virtual void on_window_resized(
        ){}
        
        virtual void on_window_moved(
        ){}

        virtual void on_mouse_down (
            unsigned long btn,
            unsigned long state,
            long x,
            long y,
            bool is_double_click
        ){}

        virtual void on_mouse_up (
            unsigned long btn,
            unsigned long state,
            long x,
            long y
        ){}

        virtual void on_mouse_move (
            unsigned long state,
            long x,
            long y
        ){}

        virtual void on_mouse_leave (
        ){}

        virtual void on_mouse_enter (
        ){}

        virtual void on_wheel_up (
        ){}

        virtual void on_wheel_down (
        ){}

        virtual void on_focus_gained (
        ){}

        virtual void on_focus_lost (
        ){}

        virtual void on_keydown (
            unsigned long key,          
            bool is_printable,
            unsigned long state
        ){}

    private:

        virtual void paint (
            const canvas& c
        ) =0;

        base_window(base_window&);        // copy constructor
        base_window& operator=(base_window&);    // assignment operator

    };

// ----------------------------------------------------------------------------------------

}


#ifdef NO_MAKEFILE
#include "gui_core_kernel_1.cpp"
#endif

#endif // DLIB_GUI_CORE_KERNEl_1_

