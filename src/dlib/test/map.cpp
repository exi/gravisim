// Copyright (C) 2003  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.


#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <dlib/map.h>
#include "tester.h"

namespace  
{

    using namespace test;
    using namespace std;
    using namespace dlib;

    logger dlog("test.map");

    template <
        typename map
        >
    void map_kernel_test (
    )
    /*!
        requires
            - map is an implementation of map/map_kernel_abstract.h and
              is instantiated to map int to int
        ensures
            - runs tests on map for compliance with the specs 
    !*/
    {        

        print_spinner();

        srand(static_cast<unsigned int>(time(0)));



        map test, test2;

        enumerable<map_pair<int,int> >& e = test;
        DLIB_CASSERT(e.at_start() == true,"");

        for (int j = 0; j < 4; ++j)
        {

            DLIB_CASSERT(test.at_start() == true,"");
            DLIB_CASSERT(test.current_element_valid() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.at_start() == false,"");
            DLIB_CASSERT(test.current_element_valid() == false,"");


            DLIB_CASSERT(test.size() == 0,"");
            DLIB_CASSERT(test.is_in_domain(5) == false,"");
            DLIB_CASSERT(test.is_in_domain(0) == false,"");
            DLIB_CASSERT(test.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test.is_in_domain(4999) == false,"");


            int a,b;
            a = 8;
            b = 94;
            test.add(a,b);
            DLIB_CASSERT(test.size() == 1,"");
            DLIB_CASSERT(test.is_in_domain(8) == true,"");
            DLIB_CASSERT(test.is_in_domain(5) == false,"");
            DLIB_CASSERT(test.is_in_domain(0) == false,"");
            DLIB_CASSERT(test.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test.is_in_domain(4999) == false,"");
            DLIB_CASSERT(test[8] == 94,"");
            a = 53;
            b = 4;
            test.add(a,b);
            DLIB_CASSERT(test.size() == 2,"");
            DLIB_CASSERT(test.is_in_domain(53) == true,"");
            DLIB_CASSERT(test.is_in_domain(5) == false,"");
            DLIB_CASSERT(test.is_in_domain(0) == false,"");
            DLIB_CASSERT(test.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test.is_in_domain(4999) == false,"");
            DLIB_CASSERT(test[53] == 4,"");


            swap(test,test2);


            DLIB_CASSERT(test2.size() == 2,"");
            DLIB_CASSERT(test2.is_in_domain(8) == true,"");
            DLIB_CASSERT(test2.is_in_domain(5) == false,"");
            DLIB_CASSERT(test2.is_in_domain(0) == false,"");
            DLIB_CASSERT(test2.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test2.is_in_domain(4999) == false,"");
            DLIB_CASSERT(test2[8] == 94,"");
            DLIB_CASSERT(test2.size() == 2,"");
            DLIB_CASSERT(test2.is_in_domain(53) == true,"");
            DLIB_CASSERT(test2.is_in_domain(5) == false,"");
            DLIB_CASSERT(test2.is_in_domain(0) == false,"");
            DLIB_CASSERT(test2.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test2.is_in_domain(4999) == false,"");
            DLIB_CASSERT(test2[53] == 4,"");


            DLIB_CASSERT(test.size() == 0,"");
            DLIB_CASSERT(test.is_in_domain(8) == false,"");
            DLIB_CASSERT(test.is_in_domain(5) == false,"");
            DLIB_CASSERT(test.is_in_domain(0) == false,"");
            DLIB_CASSERT(test.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test.is_in_domain(4999) == false,"");
            DLIB_CASSERT(test.size() == 0,"");
            DLIB_CASSERT(test.is_in_domain(53) == false,"");
            DLIB_CASSERT(test.is_in_domain(5) == false,"");
            DLIB_CASSERT(test.is_in_domain(0) == false,"");
            DLIB_CASSERT(test.is_in_domain(-999) == false,"");
            DLIB_CASSERT(test.is_in_domain(4999) == false,"");


            test.clear();
            DLIB_CASSERT(test.at_start() == true,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.at_start() == false,"");


            DLIB_CASSERT(test.size() == 0,"");

            while (test.size() < 10000)
            {
                a = ::rand();
                b = ::rand();
                if (!test.is_in_domain(a))
                    test.add(a,b);
            }

            DLIB_CASSERT(test.size() == 10000,"");
            test.clear();
            DLIB_CASSERT(test.size() == 0,"");

            while (test.size() < 10000)
            {
                a = ::rand();
                b = ::rand();
                if (!test.is_in_domain(a))
                    test.add(a,b);
            }

            DLIB_CASSERT(test.size() == 10000,"");

            int count = 0;
            a = -1;
            while (test.move_next())
            {
                DLIB_CASSERT(test.element().key() == test.element().key(),"");
                DLIB_CASSERT(test.element().value() == test.element().value(),"");
                DLIB_CASSERT(test.element().key() == test.element().key(),"");
                DLIB_CASSERT(test.element().value() == test.element().value(),"");


                DLIB_CASSERT(a < test.element().key(),"");
                a = test.element().key();
                ++count;
            }
            DLIB_CASSERT(test.current_element_valid() == false,"");
            DLIB_CASSERT(test.at_start() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");
            DLIB_CASSERT(test.current_element_valid() == false,"");
            DLIB_CASSERT(test.at_start() == false,"");
            DLIB_CASSERT(test.move_next() == false,"");

            DLIB_CASSERT(count == 10000,"");

            test.swap(test2);

            DLIB_CASSERT(test.size() == 2,"");
            DLIB_CASSERT(test2.size() == 10000,"");
            count = 0;
            a = -1;
            test2.reset();

            test2.move_next();
            test2.element().value() = 99;
            DLIB_CASSERT(test2[test2.element().key()] == 99,"");
            DLIB_CASSERT(test2.element().value() == 99,"");

            test2.reset();

            while (test2.move_next())
            {
                DLIB_CASSERT(test2[test2.element().key()] == test2.element().value(),"");
                DLIB_CASSERT(test2.element().key() == test2.element().key(),"");
                DLIB_CASSERT(test2.element().value() == test2.element().value(),"");
                DLIB_CASSERT(test2.element().key() == test2.element().key(),"");
                DLIB_CASSERT(test2.element().value() == test2.element().value(),"");
                DLIB_CASSERT(a < test2.element().key(),"");
                a = test2.element().key();                
                ++count;
            }
            DLIB_CASSERT(test2.size() == 10000,"");
            DLIB_CASSERT(count == 10000,"");
            DLIB_CASSERT(test2.current_element_valid() == false,"");
            DLIB_CASSERT(test2.at_start() == false,"");
            DLIB_CASSERT(test2.move_next() == false,"");
            DLIB_CASSERT(test2.current_element_valid() == false,"");
            DLIB_CASSERT(test2.at_start() == false,"");
            DLIB_CASSERT(test2.move_next() == false,"");



            test2.clear();
            DLIB_CASSERT(test2.size() == 0,"");
            DLIB_CASSERT(test2.at_start() == true,"");

            while (test.size() < 20000)
            {
                a = ::rand();
                b = ::rand();
                if (!test.is_in_domain(a))
                    test.add(a,b);
            }

            // serialize the state of test, then clear test, then
            // load the state back into test.
            ostringstream sout;
            serialize(test,sout);
            istringstream sin(sout.str());
            test.clear();
            deserialize(test,sin);

            DLIB_CASSERT(test.at_start() == true,"");

            {
                int* array1 = new int[test.size()];
                int* array2 = new int[test.size()];

                int* tmp1 = array1;
                int* tmp2 = array2;

                count = 0;
                while (test.move_next())
                {
                    DLIB_CASSERT(test.element().key() == test.element().key(),"");
                    DLIB_CASSERT(test.element().value() == test.element().value(),"");
                    DLIB_CASSERT(test.element().key() == test.element().key(),"");
                    DLIB_CASSERT(test.current_element_valid() == true,"");
                    *tmp1 = test.element().key();
                    *tmp2 = test.element().value();
                    ++tmp1;
                    ++tmp2;
                    ++count;
                }
                DLIB_CASSERT(count == 20000,"");

                tmp1 = array1;
                tmp2 = array2;
                for (int i = 0; i < 20000; ++i)
                {
                    DLIB_CASSERT(test.is_in_domain(*tmp1) == true,"");
                    DLIB_CASSERT(test[*tmp1] == *tmp2,"");
                    ++tmp1;
                    ++tmp2;
                }

                DLIB_CASSERT(test.size() == 20000,"");

                tmp1 = array1;
                tmp2 = array2;
                count = 0;
                while (test.size() > 10000)
                {
                    test.remove(*tmp1,a,b);
                    DLIB_CASSERT(*tmp1 == a,"");
                    DLIB_CASSERT(*tmp2 == b,"");
                    ++tmp1;
                    ++tmp2;
                    ++count;
                }
                DLIB_CASSERT(count == 10000,"");
                DLIB_CASSERT(test.size() == 10000,"");

                while (test.move_next())
                {
                    DLIB_CASSERT(test.element().key() == *tmp1,"");
                    DLIB_CASSERT(test.element().key() == *tmp1,"");
                    DLIB_CASSERT(test.element().key() == *tmp1,"");
                    DLIB_CASSERT(test.element().value() == *tmp2,"");
                    DLIB_CASSERT(test.element().value() == *tmp2,"");
                    DLIB_CASSERT(test.element().value() == *tmp2,"");
                    ++tmp1;
                    ++tmp2;
                    ++count;
                }
                DLIB_CASSERT(count == 20000,"");
                DLIB_CASSERT(test.size() == 10000,"");

                while (test.size() < 20000)
                {
                    a = ::rand();
                    b = ::rand();
                    if (!test.is_in_domain(a))
                        test.add(a,b);
                }

                test2.swap(test);

                count = 0;
                a = -1;
                while (test2.move_next())
                {
                    DLIB_CASSERT(test2.element().key() == test2.element().key(),"");
                    DLIB_CASSERT(test2.element().value() == test2.element().value(),"");
                    DLIB_CASSERT(test2.element().key() == test2.element().key(),"");
                    DLIB_CASSERT(a < test2.element().key(),"");
                    a = test2.element().key();                
                    ++count;
                }

                DLIB_CASSERT(count == 20000,"");
                DLIB_CASSERT(test2.size() == 20000,"");

                a = -1;
                int c = 0;
                while (test2.size()>0)
                {
                    test2.remove_any(b,c);
                    DLIB_CASSERT( a < b,"");
                    a = b;
                }

                DLIB_CASSERT(test2.size() == 0,"");
                delete [] array1;
                delete [] array2;
            }

            test.clear();
            test2.clear();
            while (test.size() < 10000)
            {
                a = ::rand();
                b = ::rand();
                if (!test.is_in_domain(a))
                    test.add(a,b);
            }

            count = 0; 
            a = -1;
            while (test.move_next())
            {
                DLIB_CASSERT(a < test.element().key(),"");
                DLIB_CASSERT(test[test.element().key()] == test.element().value(),"");
                a = test.element().key();
                ++count;
                if (count == 5000)
                    break;
                DLIB_CASSERT(test.current_element_valid() == true,"");
            }

            test.reset();

            count = 0; 
            a = -1;
            while (test.move_next())
            {
                DLIB_CASSERT(a < test.element().key(),"");
                a = test.element().key();
                ++count;
                DLIB_CASSERT(test.current_element_valid() == true,"");
            }

            DLIB_CASSERT(count == 10000,"");


            test.clear();
            test2.clear();
        }


        {
            test.clear();
            DLIB_CASSERT(test.size() == 0,"");
            int a = 5;
            int b = 6;
            test.add(a,b);
            a = 7;
            b = 8;
            test.add(a,b);
            DLIB_CASSERT(test.size() == 2,"");
            DLIB_CASSERT(test[7] == 8,"");
            DLIB_CASSERT(test[5] == 6,"");
            DLIB_CASSERT(test.is_in_domain(7),"");
            DLIB_CASSERT(test.is_in_domain(5),"");
            test.destroy(7);
            DLIB_CASSERT(test.size() == 1,"");
            DLIB_CASSERT(!test.is_in_domain(7),"");
            DLIB_CASSERT(test.is_in_domain(5),"");
            test.destroy(5);
            DLIB_CASSERT(test.size() == 0,"");
            DLIB_CASSERT(!test.is_in_domain(7),"");
            DLIB_CASSERT(!test.is_in_domain(5),"");
        }

    }




    class map_tester : public tester
    {
    public:
        map_tester (
        ) :
            tester ("test_map",
                    "Runs tests on the map component.")
        {}

        void perform_test (
        )
        {
            dlog << LINFO << "testing kernel_1a";
            map_kernel_test<dlib::map<int,int>::kernel_1a>  ();
            dlog << LINFO << "testing kernel_1a_c";
            map_kernel_test<dlib::map<int,int>::kernel_1a_c>();
            dlog << LINFO << "testing kernel_1b";
            map_kernel_test<dlib::map<int,int>::kernel_1b>  ();
            dlog << LINFO << "testing kernel_1b_c";
            map_kernel_test<dlib::map<int,int>::kernel_1b_c>();
        }
    } a;

}

