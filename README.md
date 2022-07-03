Croncpp:
=====================

Software:
---------

C++ source.

"Croncpp" is a library for processing CRON expressions. It implements two basic operations: find the next date or find the last date of execution of a periodic processing defined in cron format.


Use example :
=====================
        # define out /*
        g++ -o main $0 cron.cpp; exit $?
        # define out */
        # undef out

        #include <string>
        #include <iostream>
        #include "cron.h"

        int main() {
        time_t rawtime(time(NULL)), result;
        char buffer [80];
        std::string cronexpr( "5-10 55 * * apr * 2021,2025 myCommand" );

        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
        std::cout << "Now() is " << rawtime << " (" << buffer << ")" << std::endl << "Cron expression is: " << cronexpr << std::endl << std::endl;

        // Next date:
        result = cron( cronexpr ).nextDate( rawtime );
        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&result) );
        std::cout << "The next \"" << cron( cronexpr ).expression() << "\" task will be performed at: " << result << " (" << buffer << ")." << std::endl << std::endl;

        // Previous date:
        result = cron( cronexpr ).previousDate( rawtime );
        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&result) );
        std::cout << "The last execution of the \"" << cron( cronexpr ).expression() << "\" task was at: " << result << " (" << buffer << ")." << std::endl << std::endl;

        return 0;
        }

Display:
---------
	Now() is 1656593363 (2022/06/30 02:49:23)
	Cron expression is: */10 55 * * apr * 2021,2025 myCommand

	The next "myCommand" task will be performed at: 1743504900 (2025/04/01 02:55:05).

	The last execution of the "myCommand" task was at: 1617274500 (2021/04/01 01:55:10)


Or:
---------
	
	Now() is 1656744468 (2022/07/01 20:47:48)
	Cron expression is: 5-55 55 * * apr * myCommand

	The next "myCommand" task will be performed at: 1680346505 (2023/04/01 00:55:05).

	The last execution of the "myCommand" task was at: 1651312555 (2022/04/29 23:55:55).
	
