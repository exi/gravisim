// Copyright (C) 2005  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_BASE_WIDGETs_ABSTRACT_
#ifdef DLIB_BASE_WIDGETs_ABSTRACT_

#include "fonts_abstract.h"
#include "drawable_abstract.h"

#include "../gui_core.h"
#include <string>

namespace dlib
{

    /*!
        GENERAL REMARKS
            This file contains objects that are useful for creating complex drawable 
            widgets.

        THREAD SAFETY
            All objects and functions defined in this file are thread safe.  You may
            call them from any thread without serializing access to them.

        EVENT HANDLERS
            If you derive from any of the drawable objects and redefine any of the on_*() 
            event handlers then you should ensure that your version calls the same event 
            handler in the base object so that the base class part of your object will also 
            be able to process the event. 

            Also note that all event handlers, including the user registered callback
            functions, are executed in the event handling thread. 
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class dragable
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class dragable : public drawable
    {
        /*!
            INITIAL VALUE
                dragable_area() == an initial value for its type 

            WHAT THIS OBJECT REPRESENTS
                This object represents a drawable object that is dragable by the mouse.  
                You use it by inheriting from it and defining the draw() method and any
                of the on_*() event handlers you need.  

                This object is dragable by the user when is_enabled() == true and 
                not dragable otherwise.
        !*/

    public:

        dragable(  
            drawable_window& w,
            unsigned long events = 0
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
                - This object will not receive any events or draw() requests until 
                  enable_events() is called
                - the events flags are passed on to the drawable object's 
                  constructor.
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~dragable(
        ) = 0;
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        rectangle dragable_area (
        ) const;
        /*!
            ensures
                - returns the area that this dragable can be dragged around in. 
        !*/

        void set_dragable_area (
            const rectangle& area 
        ); 
        /*!
            ensures
                - #dragable_area() == area
        !*/

    protected:

        // does nothing by default
        virtual void on_drag (
        ){}
        /*!
            requires
                - enable_events() has been called
                - is_enabled() == true
                - is_hidden() == false
                - mutex drawable::m is locked
                - is called when the user drags this object
                - get_rect() == the rectangle that defines the new position
                  of this object.
            ensures
                - does not change the state of mutex drawable::m. 
        !*/

    private:

        // restricted functions
        dragable(dragable&);        // copy constructor
        dragable& operator=(dragable&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class mouse_over_event 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class mouse_over_event : public drawable
    {
        /*!
            INITIAL VALUE
                is_mouse_over() == false

            WHAT THIS OBJECT REPRESENTS
                This object represents a drawable object with the additoin of two events
                that will alert you when the mouse enters or leaves your drawable object.

                You use it by inheriting from it and defining the draw() method and any
                of the on_*() event handlers you need.  
        !*/

    public:

        mouse_over_event(  
            drawable_window& w,
            unsigned long events = 0
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
                - #*this will not receive any events or draw() requests until 
                  enable_events() is called
                - the events flags are passed on to the drawable object's 
                  constructor.
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~mouse_over_event(
        ) = 0;
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

    protected:

        bool is_mouse_over (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - if (the mouse is currently over this widget) then
                    - returns true
                - else
                    - returns false
        !*/

        // does nothing by default
        virtual void on_mouse_over (
        ){}
        /*!
            requires
                - enable_events() has been called
                - mutex drawable::m is locked
                - is_enabled() == true
                - is_hidden() == false
                - is called whenever this object transitions from the state where
                  is_mouse_over() == false to is_mouse_over() == true
            ensures
                - does not change the state of mutex drawable::m. 
        !*/

        // does nothing by default
        virtual void on_mouse_not_over (
        ){}
        /*!
            requires
                - enable_events() has been called
                - mutex drawable::m is locked
                - is_enabled() == true
                - is_hidden() == false
                - is called whenever this object transitions from the state where
                  is_mouse_over() == true to is_mouse_over() == false 
            ensures
                - does not change the state of mutex drawable::m. 
        !*/

    private:

        // restricted functions
        mouse_over_event(mouse_over_event&);        // copy constructor
        mouse_over_event& operator=(mouse_over_event&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class button_action 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class button_action : public mouse_over_event 
    {
        /*!
            INITIAL VALUE
                is_depressed() == false

            WHAT THIS OBJECT REPRESENTS
                This object represents the clicking action of a push button.  It provides
                simple callbacks that can be used to make various kinds of button 
                widgets.

                You use it by inheriting from it and defining the draw() method and any
                of the on_*() event handlers you need.  
        !*/

    public:

        button_action(  
            drawable_window& w,
            unsigned long events = 0
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
                - #*this will not receive any events or draw() requests until 
                  enable_events() is called
                - the events flags are passed on to the drawable object's 
                  constructor.
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~button_action(
        ) = 0;
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

    protected:

        bool is_depressed (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - if (this button is currently in a depressed state) then
                    - the user has left clicked on this drawable and is still
                      holding the left mouse button down over it.
                    - returns true
                - else
                    - returns false
        !*/

        // does nothing by default
        virtual void on_button_down (
        ){}
        /*!
            requires
                - enable_events() has been called
                - mutex drawable::m is locked
                - is_enabled() == true
                - is_hidden() == false
                - the area in parent_window() defined by get_rect() has been invalidated. 
                  (This means you don't have to call invalidate_rectangle())
                - is called whenever this object transitions from the state where
                  is_depressed() == false to is_depressed() == true
            ensures
                - does not change the state of mutex drawable::m. 
        !*/

        // does nothing by default
        virtual void on_button_up (
            bool mouse_over
        ){}
        /*!
            requires
                - enable_events() has been called
                - mutex drawable::m is locked
                - is_enabled() == true
                - is_hidden() == false
                - the area in parent_window() defined by get_rect() has been invalidated. 
                  (This means you don't have to call invalidate_rectangle())
                - is called whenever this object transitions from the state where
                  is_depressed() == true to is_depressed() == false 
                - if (the mouse was over this button when this event occurred) then
                    - mouse_over == true
                - else
                    - mouse_over == false
            ensures
                - does not change the state of mutex drawable::m. 
        !*/

    private:

        // restricted functions
        button_action(button_action&);        // copy constructor
        button_action& operator=(button_action&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class arrow_button 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class arrow_button : public button_action 
    {
        /*!
            INITIAL VALUE
                direction() == a value given to the constructor.

            WHAT THIS OBJECT REPRESENTS
                This object represents a push button with an arrow in the middle. 

                When this object is disabled it means it will not respond to user clicks.
        !*/

    public:
        enum arrow_direction 
        {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        arrow_button(  
            drawable_window& w,
            arrow_direction dir 
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #direction() == dir
                - #parent_window() == w
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~arrow_button(
        );
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        arrow_direction direction (
        ) const;
        /*!
            ensures
                - returns the direction that this arrow_button's arrow points
        !*/

        void set_direction (
            arrow_direction new_direction
        );
        /*!
            ensures
                - #direction() == new_direction
        !*/

        void set_size (
            unsigned long width_,
            unsigned long height_
        );
        /*! 
            ensures
                - #width() == width_
                - #height() == height_
                - #top() == top()
                - #left() == left()
                - i.e. The location of the upper left corner of this button stays the
                  same but its width and height are modified
        !*/

        bool is_depressed (
        ) const;
        /*!
            ensures
                - if (this button is currently in a depressed state) then
                    - the user has left clicked on this drawable and is still
                      holding the left mouse button down over it.
                    - returns true
                - else
                    - returns false
        !*/

        template <
            typename T
            >
        void set_button_down_handler (
            T& object,
            void (T::*event_handler)()
        );
        /*!
            requires
                - event_handler is a valid pointer to a member function in T
            ensures
                - The event_handler function is called whenever this object transitions
                  from the state where is_depressed() == false to is_depressed() == true
                - any previous calls to this function are overridden by this new call.  
                  (i.e. you can only have one event handler associated with this 
                  event at a time)
            throws
                - std::bad_alloc
        !*/

        template <
            typename T
            >
        void set_button_up_handler (
            T& object,
            void (T::*event_handler)(bool mouse_over)
        );
        /*!
            requires
                - event_handler is a valid pointer to a member function in T
            ensures
                - The event_handler function is called whenever this object transitions
                  from the state where is_depressed() == true to is_depressed() == false.
                  furthermore:
                    - if (the mouse was over this button when this event occurred) then
                        - mouse_over == true
                    - else
                        - mouse_over == false
                - any previous calls to this function are overridden by this new call.  
                  (i.e. you can only have one event handler associated with this 
                  event at a time)
            throws
                - std::bad_alloc
        !*/

    private:

        // restricted functions
        arrow_button(arrow_button&);        // copy constructor
        arrow_button& operator=(arrow_button&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class scroll_bar 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class scroll_bar : public drawable 
    {
        /*!
            INITIAL VALUE
                orientation() == a value given to the constructor.
                max_slider_pos() == 0
                slider_pos() == 0
                jump_size() == 10

            WHAT THIS OBJECT REPRESENTS
                This object represents a scroll bar.  The slider_pos() of the scroll bar
                ranges from 0 to max_slider_pos().  The 0 position of the scroll_bar is
                in the top or left side of the scroll_bar depending on its orientation.

                When this object is disabled it means it will not respond to user clicks.
        !*/

    public:
        enum bar_orientation 
        {
            HORIZONTAL,
            VERTICAL
        };

        scroll_bar(  
            drawable_window& w,
            bar_orientation orientation 
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #orientation() == orientation 
                - #parent_window() == w
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~scroll_bar(
        );
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        bar_orientation orientation (
        ) const;
        /*!
            ensures
                - returns the orientation of this scroll_bar 
        !*/

        void set_orientation (
            bar_orientation new_orientation   
        );
        /*!
            ensures
                - #orientation() == new_orientation
        !*/

        void set_length (
            unsigned long length,
        );
        /*! 
            ensures
                - if (orientation() == HORIZONTAL) then
                    - #width() == max(length,1)
                - else
                    - #height() == max(length,1)
        !*/

        long max_slider_pos (
        ) const;
        /*!
            ensures
                - returns the maximum value that slider_pos() can take. 
        !*/

        void set_max_slider_pos (
            long mpos
        );
        /*!
            ensures
                - if (mpos < 0) then
                    - #max_slider_pos() == 0
                - else
                    - #max_slider_pos() == mpos
                - if (slider_pos() > #max_slider_pos()) then
                    - #slider_pos() == #max_slider_pos() 
                - else
                    - #slider_pos() == slider_pos()
        !*/

        void set_slider_pos (
            unsigned long pos
        );
        /*!
            ensures
                - if (pos < 0) then
                    - #slider_pos() == 0
                - else if (pos > max_slider_pos()) then
                    - #slider_pos() == max_slider_pos()
                - else
                    - #slider_pos() == pos
        !*/

        long slider_pos (
        ) const;
        /*!
            ensures
                - returns the current position of the slider box within the scroll bar.
        !*/

        long jump_size (
        ) const;
        /*!
            ensures
                - returns the number of positions that the slider bar will jump when the
                  user clicks on the empty gaps above or below the slider bar.
                  (note that the slider will jump less than the jump size if it hits the 
                  end of the scroll bar)
        !*/

        void set_jump_size (
            long js 
        );
        /*!
            ensures
                - if (js < 1) then
                    - #jump_size() == 1
                - else
                    - #jump_size() == js 
        !*/


        template <
            typename T
            >
        void set_scroll_handler (
            T& object,
            void (T::*event_handler)()
        );
        /*!
            requires
                - event_handler is a valid pointer to a member function in T
            ensures
                - The event_handler function is called whenever the user causes the slider box
                  to move.  
                - This event is NOT triggered by calling set_slider_pos()
                - any previous calls to this function are overridden by this new call.  
                  (i.e. you can only have one event handler associated with this 
                  event at a time)
            throws
                - std::bad_alloc
        !*/

    private:

        // restricted functions
        scroll_bar(scroll_bar&);        // copy constructor
        scroll_bar& operator=(scroll_bar&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class widget_group 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class widget_group : public drawable 
    {
        /*!
            INITIAL VALUE
                size() == 0
                get_rect().is_empty() == true
                left() == 0
                top() == 0

            WHAT THIS OBJECT REPRESENTS
                This object represents a grouping of drawable widgets.  It doesn't draw 
                anything itself, rather it lets you manipulate the position, enabled
                status, and visibility of a set of widgets as a group.
        !*/

    public:
        widget_group(  
            drawable_window& w
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~widget_group(
        );
        /*!
            ensures
                - all resources associated with *this have been released.
        !*/

        void empty (
        );
        /*!
            ensures
                - #size() == 0
        !*/

        void fit_to_contents (
        );
        /*!
            ensures
                - does not change the position of this object. 
                  (i.e. the upper left corner of get_rect() remains at the same position)
                - if (size() == 0) then
                    - #get_rect().is_empty() == true
                - else
                    - #get_rect() will be the smallest rectangle that contains all the 
                      widgets in this group and the upper left corner of get_rect(). 
        !*/

        unsigned long size (
        ) const;
        /*!
            ensures
                - returns the number of widgets currently in *this.
        !*/

        void add (
            drawable& widget,
            unsigned long x,
            unsigned long y
        );
        /*!
            ensures
                - #is_member(widget) == true
                - if (is_member(widget) == false) then
                    - #size() == size() + 1
                - else
                    - #size() == size()
                - The following conditions apply to this function as well as to all of the 
                  following functions so long as is_member(widget) == true: 
                  enable(), disable(), hide(), show(), set_z_order(), and set_pos().
                    - #widget.left() == left()+x
                    - #widget.width() == widget.width()
                    - #widget.top() == top()+y
                    - #widget.height() == widget.height()
                    - #widget.is_hidden() == is_hidden()
                    - #widget.is_enabled() == is_enabled()
                    - #widget.z_order() == z_order()
            throws
                - std::bad_alloc
        !*/

        bool is_member (
            const drawable& widget
        ) const;
        /*!
            ensures
                - returns true if widget is currently in this object, returns false otherwise.
        !*/

        void remove (
            const drawable& widget
        );
        /*!
            ensures
                - #is_member(widget) == false 
                - if (is_member(widget) == true) then
                    - #size() == size() - 1
                - else
                    - #size() == size()
        !*/

    protected:

        // this object doesn't draw anything but also isn't abstract
        void draw (
            const canvas& c
        ) const {}

    private:

        // restricted functions
        widget_group(widget_group&);        // copy constructor
        widget_group& operator=(widget_group&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class image_widget 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class image_widget : public dragable
    {
        /*!
            INITIAL VALUE
                dragable_area() == an initial value for its type.
                This object isn't displaying anything. 

            WHAT THIS OBJECT REPRESENTS
                This object represents a dragable image.  You give it an image to display
                by calling set_image().

                Also note that initially the dragable area is empty so it won't be 
                dragable unless you call set_dragable_area() to some non-empty region.

                The image is drawn such that:
                    - the pixel img[0][0] is the upper left corner of the image.
                    - the pixel img[img.nr()-1][0] is the lower left corner of the image.
                    - the pixel img[0][img.nc()-1] is the upper right corner of the image.
                    - the pixel img[img.nr()-1][img.nc()-1] is the lower right corner of the image.
                
        !*/

    public:

        image_widget(  
            drawable_window& w
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~image_widget(
        );
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        template <
            typename image_type 
            >
        void set_image (
            const image_type& img
        );
        /*!
            requires
                - image_type == an implementation of array2d/array2d_kernel_abstract.h
                - pixel_traits<typename image_type::type> must be defined 
            ensures
                - #width() == img.nc()
                - #height() == img.nr()
                - #*this widget is now displaying the given image img.
        !*/

    private:

        // restricted functions
        image_widget(image_widget&);        // copy constructor
        image_widget& operator=(image_widget&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // class tooltip 
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class tooltip : public mouse_over_event 
    {
        /*!
            INITIAL VALUE
                - text() == ""
                - the tooltip is inactive until the text is changed to
                  a non-empty string.

            WHAT THIS OBJECT REPRESENTS
                This object represents a region on a window where if the user
                hovers the mouse over this region a tooltip with a message
                appears.
        !*/

    public:

        tooltip(  
            drawable_window& w
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~tooltip(
        );
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        void set_size (
            long width_, 
            long height_ 
        );
        /*!
            ensures
                - #width() == width_
                - #height() == height_
                - #top() == top()
                - #left() == left()
                - i.e. The location of the upper left corner of this widget stays the
                  same but its width and height are modified
        !*/

        void set_text (
            const std::string& str
        );
        /*!
            ensures
                - #text() == str
                - activates the tooltip.  i.e. after this function the tooltip
                  will display on the screen when the user hovers the mouse over it
        !*/

        const std::string text (
        ) const;
        /*!
            ensures
                - returns the text that is displayed inside this
                  tooltip
        !*/

    private:

        // restricted functions
        tooltip(tooltip&);        // copy constructor
        tooltip& operator=(tooltip&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
    // popup menu stuff  
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class menu_item
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an abstract class that defines the interface a
                menu item in a popup_menu must implement.

                Note that a menu_item is drawn as 3 separate pieces:
                    ---------------------------------
                    | left | middle |         right |
                    ---------------------------------

                Also note that derived classes must be copyable via
                their copy constructors.
        !*/

    public:

        virtual ~menu_item() {}

        virtual void on_click (
        ) const {}
        /*!
            requires
                - the mutex drawable::m is locked
                - if (has_click_event()) then
                    - this function is called when the user clicks on this menu_item
        !*/

        virtual bool has_click_event (
        ) const { return false; }
        /*!
            ensures
                - if (this menu_item wants to receive on_click events) then
                    - returns true
                - else
                    - returns false
        !*/

        virtual unichar get_hot_key (
        ) const { return 0; }
        /*!
            ensures
                - if (this menu item has a keyboard hot key) then
                    - returns the unicode value of the key
                - else
                    - returns 0
        !*/

        virtual rectangle get_left_size (
        ) const { return rectangle(); } // return empty rect by default
        /*!
            ensures
                - returns the dimensions of the left part of the menu_item
        !*/

        virtual rectangle get_middle_size (
        ) const = 0; 
        /*!
            ensures
                - returns the dimensions of the middle part of the menu_item
        !*/

        virtual rectangle get_right_size (
        ) const { return rectangle(); } // return empty rect by default
        /*!
            ensures
                - returns the dimensions of the right part of the menu_item
        !*/

        virtual void draw_background (
            const canvas& c,
            const rectangle& rect,
            const bool enabled,
            const bool is_selected
        ) const {}
        /*!
            requires
                - the mutex drawable::m is locked
            requires
                - c == the canvas to draw on
                - rect == the rectangle in which we are to draw the background
                - enabled == true if the menu_item is to be drawn enabled
                - is_selected == true if the menu_item is to be drawn selected
            ensures
                - draws the background of the menu_item on the canvas c at the location 
                  given by rect.  
        !*/

        virtual void draw_left (
            const canvas& c,
            const rectangle& rect,
            const bool enabled,
            const bool is_selected
        ) const {}
        /*!
            requires
                - the mutex drawable::m is locked
            requires
                - c == the canvas to draw on
                - rect == the rectangle in which we are to draw the background
                - enabled == true if the menu_item is to be drawn enabled
                - is_selected == true if the menu_item is to be drawn selected
            ensures
                - draws the left part of the menu_item on the canvas c at the location 
                  given by rect.  
        !*/

        virtual void draw_middle (
            const canvas& c,
            const rectangle& rect,
            const bool enabled,
            const bool is_selected
        ) const = 0;
        /*!
            requires
                - the mutex drawable::m is locked
            requires
                - c == the canvas to draw on
                - rect == the rectangle in which we are to draw the background
                - enabled == true if the menu_item is to be drawn enabled
                - is_selected == true if the menu_item is to be drawn selected
            ensures
                - draws the middle part of the menu_item on the canvas c at the location 
                  given by rect.  
        !*/

        virtual void draw_right (
            const canvas& c,
            const rectangle& rect,
            const bool enabled,
            const bool is_selected
        ) const {}
        /*!
            requires
                - the mutex drawable::m is locked
            requires
                - c == the canvas to draw on
                - rect == the rectangle in which we are to draw the background
                - enabled == true if the menu_item is to be drawn enabled
                - is_selected == true if the menu_item is to be drawn selected
            ensures
                - draws the right part of the menu_item on the canvas c at the location 
                  given by rect.  
        !*/
    };

// ----------------------------------------------------------------------------------------

    class menu_item_text : public menu_item
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object is a simple text menu item
        !*/

    public:

        template <
            typename T
            >
        menu_item_text (
            const std::string& str,
            T& object,
            void (T::*on_click_handler)(),
            unichar hotkey = 0
        ); 
        /*!
            ensures
                - The text of this menu item will be str
                - the on_click_handler function is called on object when this menu_item 
                  clicked by the user.
                - #get_hot_key() == hotkey
        !*/
    };

// ----------------------------------------------------------------------------------------

    class menu_item_submenu : public menu_item
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object is a simple text item intended to be used with
                submenus inside a popup_menu.
        !*/

    public:

        menu_item_submenu (
            const std::string& str,
            unichar hotkey = 0
        ); 
        /*!
            ensures
                - The text of this menu item will be str
                  clicked by the user.
                - #get_hot_key() == hotkey
        !*/
    };

// ----------------------------------------------------------------------------------------

    class menu_item_separator : public menu_item
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object is a horizontal separator in a popup menu 
        !*/
    };

// ----------------------------------------------------------------------------------------

    class popup_menu : public base_window
    {
        /*!
            INITIAL VALUE
                - size() == 0

            WHAT THIS OBJECT REPRESENTS
                This object represents a popup menu window capable of containing
                menu_item objects.
        !*/

    public:

        popup_menu (
        );
        /*!
            ensures 
                - #*this is properly initialized 
            throws
                - std::bad_alloc
                - dlib::thread_error
                - dlib::gui_error
        !*/

        void clear (
        );
        /*!
            ensures
                - #*this has its initial value
            throws
                - std::bad_alloc 
                  if this exception is thrown then *this is unusable
                  until clear() is called and succeeds
        !*/

        template <
            typename menu_item_type
            >
        unsigned long add_menu_item (
            const menu_item_type& new_item
        );
        /*!
            requires
                - menu_item_type == a type that inherits from menu_item 
            ensures
                - adds new_item onto the bottom of this popup_menu. 
                - returns size() 
                  (This is also the index by which this item can be
                  referenced by the enable_menu_item() and disable_menu_item()
                  functions.)
        !*/
        
        template <
            typename menu_item_type
            >
        unsigned long add_submenu (
            const menu_item_type& new_item,
            popup_menu& submenu
        );
        /*!
            requires
                - menu_item_type == a type that inherits from menu_item 
            ensures
                - adds new_item onto the bottom of this popup_menu. 
                - when the user puts the mouse above this menu_item the given
                  submenu popup_menu will be displayed.
                - returns size() 
                  (This is also the index by which this item can be
                  referenced by the enable_menu_item() and disable_menu_item()
                  functions.)
        !*/

        void enable_menu_item (
            unsigned long idx
        );
        /*!
            requires
                - idx < size()
            ensures
                - the menu_item in this with the index idx has been enabled 
        !*/

        void disable_menu_item (
            unsigned long idx
        );
        /*!
            requires
                - idx < size()
            ensures
                - the menu_item in this with the index idx has been disabled
        !*/

        unsigned long size (
        ) const;
        /*!
            ensures
                - returns the number of menu_item objects in this popup_menu
        !*/

        template <typename T>
        void set_on_hide_handler (
            T& object,
            void (T::*event_handler)()
        );
        /*!
            ensures
                - the event_handler function is called on object when this popup_menu
                  hides itself due to an action by the user. 
                - Note that you can register multiple handlers for this event. 
        !*/

        void select_first_item (
        );
        /*!
            ensures
                - causes this popup menu to highlight the first 
                  menu item that it contains which has a click event 
                  and is enabled.
        !*/

        bool forwarded_on_keydown (
            unsigned long key,
            bool is_printable,
            unsigned long state
        );
        /*!
            requires
                - key, is_printable, and state are the variables from the
                  base_window::on_keydown() event
            ensures
                - forwards this keyboard event to this popup window so that it
                  may deal with keyboard events from other windows.
                - if (this popup_menu uses the keyboard event) then
                    - returns true
                - else
                    - returns false
        !*/

    private:

        // restricted functions
        popup_menu(popup_menu&);        // copy constructor
        popup_menu& operator=(popup_menu&);    // assignment operator

    };

// ----------------------------------------------------------------------------------------

    class zoomable_region : public drawable 
    {
        /*
            INITIAL VALUE
                - min_zoom_scale() == 0.15
                - max_zoom_scale() == 1.0
                - zoom_increment() == 0.02
                - zoom_scale() == 1.0

            WHAT THIS OBJECT REPRESENTS
                This object represents a 2D Cartesian graph that you can zoom into and
                out of.  It is a graphical a widget that draws a rectangle with 
                a horizontal and vertical scroll bar that allow the user to scroll
                around on a Cartesian graph that is much larger than the actual 
                area occupied by this object on the screen.  It also allows 
                the user to zoom in and out.

                To use this object you inherit from it and make use of its public and
                protected member functions.  It provides functions for converting between
                pixel locations and the points in our 2D Cartesian graph so that when the 
                user is scrolling/zooming the widget you can still determine where
                things are to be placed on the screen and what screen pixels correspond
                to in the Cartesian graph.

                Note that the Cartesian graph in this object is bounded by the point
                (0,0), corresponding to the upper left corner when we are zoomed all 
                the way out, and max_graph_point() which corresponds to the lower right 
                corner when zoomed all the way out. The value of max_graph_point() is 
                determined automatically from the size of this object's on screen 
                rectangle and the value of min_zoom_scale() which determines how far 
                out you can zoom.

                Also note that while vector<double> is used to represent graph points
                the z field is always ignored by this object.
        */

    public:

        zoomable_region (
            drawable_window& w,
            unsigned long events = 0
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
                - This object will not receive any events or draw() requests until 
                  enable_events() is called
                - the events flags are passed on to the drawable object's 
                  constructor.
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~zoomable_region (
        ) = 0;
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        void set_zoom_increment (
            double zi
        );
        /*!
            ensures
                - #zoom_increment() == zi
        !*/

        double zoom_increment (
        ) const;
        /*!
            ensures
                - returns the amount by which zoom_scale() is changed when the user
                  zooms in or out by using the mouse wheel.
        !*/

        void set_max_zoom_scale (
            double ms 
        );
        /*!
            ensures
                - #max_zoom_scale() == ms
        !*/

        void set_min_zoom_scale (
            double ms 
        );
        /*!
            ensures
                - #min_zoom_scale() == ms
        !*/

        double min_zoom_scale (
        ) const;
        /*!
            ensures
                - returns the minimum allowed value of zoom_scale()
        !*/

        double max_zoom_scale (
        ) const;
        /*!
            ensures
                - returns the maximum allowed value of zoom_scale() 
        !*/

        void set_size (
            long width,
            long height
        );
        /*! 
            ensures
                - #width() == width_
                - #height() == height_
                - #top() == top()
                - #left() == left()
                - i.e. The location of the upper left corner of this button stays the
                  same but its width and height are modified
        !*/

    protected:

        rectangle display_rect (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns the rectangle on the screen that contains the Cartesian
                  graph in this widget.  I.e. this is the area of this widget minus
                  the area taken up by the scroll bars and border decorations.
        !*/

        point graph_to_gui_space (
            const vector<double>& graph_point
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns the location of the pixel on the screen that corresponds
                  to the given point in Cartesian graph space
        !*/

        vector<double> gui_to_graph_space (
            const point& pixel_point
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns the point in Cartesian graph space that corresponds to the given
                  pixel location
        !*/

        vector<double> max_graph_point (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns the pixel farthest from the graph point (0,0) that is still
                  in the graph.  I.e. returns the point in graph space that corresponds
                  to the lower right corner of the display_rect() when we are zoomed
                  all the way out.
        !*/

        double zoom_scale (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns a double Z that represents the current zoom.  
                    - Smaller values of Z represent the user zooming out. 
                    - Bigger values of Z represent the user zooming in.  
                    - The default unzoomed case is when Z == 1
                    - objects should be drawn such that they are zoom_scale() 
                      times their normal size
        !*/

        void set_zoom_scale (
            double new_scale
        );
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - if (min_zoom_scale() <= new_scale && new_scale <= max_zoom_scale()) then
                    - #zoom_scale() == new_scale
                    - invalidates the display_rect() so that it will be redrawn
                - else
                   - #zoom_scale() == zoom_scale()
                     I.e. this function has no effect
        !*/

        void center_display_at_graph_point (
            const vector<double>& graph_point
        );
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - causes the given graph point to be centered in the display
                  if possible
                - invalidates the display_rect() so that it will be redrawn
        !*/

    // ---------------------------- event handlers ----------------------------
    // The following event handlers are used in this object.  So if you
    // use any of them in your derived object you should pass the events 
    // back to it so that they still operate unless you wish to hijack the
    // event for your own reasons (e.g. to override the mouse drag this object
    // performs)

        void on_wheel_down ();
        void on_wheel_up ();
        void on_mouse_move ( unsigned long state, long x, long y);
        void on_mouse_up ( unsigned long btn, unsigned long state, long x, long y);
        void on_mouse_down ( unsigned long btn, unsigned long state, long x, long y, bool is_double_click);
        void draw ( const canvas& c) const;

    private:

        // restricted functions
        zoomable_region(zoomable_region&);        // copy constructor
        zoomable_region& operator=(zoomable_region&);    // assignment operator

    };

// ----------------------------------------------------------------------------------------

    class scrollable_region : public drawable 
    {
        /*!
            INITIAL VALUE
                - horizontal_scroll_pos() == 0
                - horizontal_scroll_increment() == 1
                - vertical_scroll_pos() == 0
                - vertical_scroll_increment() == 1
                - total_rect().empty() == true

            WHAT THIS OBJECT REPRESENTS
                This object represents a 2D region of arbitrary size that is displayed
                within a possibly smaller scrollable gui widget.  That is, it is a 
                graphical a widget that draws a rectangle with a horizontal and vertical 
                scroll bar that allow the user to scroll around on a region that is much 
                larger than the actual area occupied by this object on the screen. 
                
                To use this object you inherit from it and make use of its public and
                protected member functions.  It provides a function, total_rect(), that
                tells you where the 2D region is on the screen.  You draw your stuff 
                inside total_rect() as you would normally except that you only modify 
                pixels that are also inside display_rect(). 
        !*/

    public:
        scrollable_region (
            drawable_window& w,
            unsigned long events = 0
        );
        /*!
            ensures 
                - #*this is properly initialized 
                - #*this has been added to window w
                - #parent_window() == w
                - This object will not receive any events or draw() requests until 
                  enable_events() is called
                - the events flags are passed on to the drawable object's 
                  constructor.
            throws
                - std::bad_alloc
                - dlib::thread_error
        !*/

        virtual ~scrollable_region (
        ) = 0;
        /*!
            ensures
                - all resources associated with *this have been released
        !*/

        void set_size (
            unsigned long width,
            unsigned long height
        );
        /*! 
            ensures
                - #width() == width_
                - #height() == height_
                - #top() == top()
                - #left() == left()
                - i.e. The location of the upper left corner of this button stays the
                  same but its width and height are modified
        !*/

        long horizontal_scroll_pos (
        ) const;
        /*!
            ensures
                - returns the current position of the horizontal scroll bar.
                  0 means it is at the far left while bigger values represent
                  scroll positions closer to the right.
        !*/

        long vertical_scroll_pos (
        ) const;
        /*!
            ensures
                - returns the current position of the vertical scroll bar.
                  0 means it is at the top and bigger values represent scroll positions
                  closer to the bottom.
        !*/

        void set_horizontal_scroll_pos (
            long pos
        );
        /*!
            ensures
                - if (pos is a valid horizontal scroll position) then
                    - #horizontal_scroll_pos() == pos
                - else
                    - #horizontal_scroll_pos() == the valid scroll position closest to pos
        !*/

        void set_vertical_scroll_pos (
            long pos
        );
        /*!
            ensures
                - if (pos is a valid vertical scroll position) then
                    - #vertical_scroll_pos() == pos
                - else
                    - #vertical_scroll_pos() == the valid scroll position closest to pos
        !*/

        unsigned long horizontal_scroll_increment (
        ) const;
        /*!
            ensures
                - returns the number of pixels that total_rect() is moved by when
                  the horizontal scroll bar moves by one position
        !*/

        unsigned long vertical_scroll_increment (
        ) const;
        /*!
            ensures
                - returns the number of pixels that total_rect() is moved by when
                  the vertical scroll bar moves by one position
        !*/

        void set_horizontal_scroll_increment (
            unsigned long inc
        );
        /*!
            ensures
                - #horizontal_scroll_increment() == inc
        !*/

        void set_vertical_scroll_increment (
            unsigned long inc
        );
        /*!
            ensures
                - #vertical_scroll_increment() == inc
        !*/

    protected:

        rectangle display_rect (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns the rectangle on the screen that contains the scrollable 
                  area in this widget.  I.e. this is the area of this widget minus
                  the area taken up by the scroll bars and border decorations.
        !*/

        void set_total_rect_size (
            unsigned long width,
            unsigned long height
        );
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - #total_rect().width() == width
                - #total_rect().height() == height 
        !*/

        const rectangle& total_rect (
        ) const;
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - returns a rectangle that represents the entire scrollable
                  region inside this widget, even the parts that are outside
                  this->rect.  
        !*/

        void scroll_to_rect (
            const rectangle& r
        );
        /*!
            requires
                - mutex drawable::m is locked
            ensures
                - adjusts the scroll bars of this object so that the rectangle
                  r is displayed in the display_rect() (or as close to being 
                  displayed as possible if r is outside of total_rect())
        !*/

    // ---------------------------- event handlers ----------------------------
    // The following event handlers are used in this object.  So if you
    // use any of them in your derived object you should pass the events 
    // back to it so that they still operate unless you wish to hijack the
    // event for your own reasons (e.g. to override the mouse wheel action 
    // this object performs)

        void on_wheel_down ();
        void on_wheel_up ();
        void draw (const canvas& c) const;

    private:

        // restricted functions
        scrollable_region(scrollable_region&);        // copy constructor
        scrollable_region& operator=(scrollable_region&);    // assignment operator

    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_BASE_WIDGETs_ABSTRACT_


