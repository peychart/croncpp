Croncpp:
=====================


Software:
---------

C++ source.

"Croncpp" is a library for processing CRON expressions.

It implements two basic operations: find the next date or find the last date of execution of a periodic processing defined in cron format.

<table>
  <tr>
    <td><tt>cron()</tt></td>
    <td>initialze a cron object</td>
  </tr>
  <tr>
    <td><tt>cron( std::string( `a cron format string definition` )</tt></td>
    <td>initialize and set a cron job object</td>
  </tr>
  <tr>
    <td><tt>std::string cron.expression()</tt></td>
    <td>return the commande on the job</td>
  </tr>
  <tr>
    <td><tt>cron& cron.assign( std::string )</td>
    <td>assign a new cron definition</td>
  </tr>
  <tr>
    <td><tt>cron = std:string()</td>
    <td>assign a new cron definition</td>
  </tr>
  <tr>
    <td><tt>cron& cron.clear()</td>
    <td>clear the cron object</td>
  </tr>
  <tr>
    <td><tt>bool cron.error() (false on error)</td>
    <td>allows to verify the cron expression</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.nextDate( const time_t )</td>
    <td>Find the next date of the cron job (-1 on error)</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.nextDate( std::tn )</td>
    <td>Find the next date of the cron job</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.previousDate( const time_t )</td>
    <td>give the last date of the previous cron job</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.previousDate( std::tn )</td>
    <td>give the last date of the previous cron job</td>
  </tr>
</table>


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
        std::string cronexpr( "*/10 55 * * apr * 2021-2025 myCommand" );
        // Or:
        // localtime( &rawtime );
        // cron myCron = "*/10 55 * * apr * 2021-2025 myCommand";
        // rawtime = myCron.nextDate( &rawtime );
        // std::cout << localtime( &rawtime ) << std::endl;
        // ...

        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
        std::cout << "Now() is " << rawtime << " (" << buffer << ")" << std::endl << "Cron expression is: " << cronexpr << std::endl;

        // Next date:
        result = cron( cronexpr ).nextDate( rawtime );
        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&result) );
        std::cout << "The next \"" << cron( cronexpr ).expression() << "\" task will be performed at: " << result << " (" << buffer << ")." << std::endl;

        // Previous date:
        result = cron( cronexpr ).previousDate( rawtime );
        strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&result) );
        std::cout << "The last execution of the \"" << cron( cronexpr ).expression() << "\" task was at: " << result << " (" << buffer << ")." << std::endl;

        return 0;
        }

Display:
---------
	Now() is 1656593363 (2022/06/30 02:49:23)
	Cron expression is: */10 55 * * apr * 2021,2025 myCommand
	The next "myCommand" task will be performed at: 1743504900 (2025/04/01 00:55:00).
	The last execution of the "myCommand" task was at: 1617274500 (2021/04/30 23:55:50)

Or:
---------
	
	Now() is 1656744468 (2022/07/01 20:47:48)
	Cron expression is: 5-55 55 * * apr * myCommand
	The next "myCommand" task will be performed at: 1680346505 (2023/04/01 00:55:05).
	The last execution of the "myCommand" task was at: 1651312555 (2022/04/29 23:55:55).

Or:
---------
	Now() is 1656905304 (2022/07/03 17:28:24)
	Cron expression is: * 25-30 * * * * myCommand
	The next "myCommand" task will be performed at: 1656905340 (2022/07/03 17:29:00).
	The last execution of the "myCommand" task was at: 1656905280 (2022/07/03 17:28:00).

...
