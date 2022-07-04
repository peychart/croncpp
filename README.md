Croncpp:
=====================


Software:
---------

C++ source.

"Croncpp" is a light library (iot-friendly) for processing CRON expressions.

It implements two basic operations: find the next date or find the last date of execution of a periodic processing defined in cron format.

<table>
  <tr>
    <td><tt>cron()</tt></td>
    <td>initialze a cron object</td>
  </tr>
  <tr>
    <td><tt>cron( std::string( `a cron format string definition (see below)` )</tt></td>
    <td>initialize and set a cron job object</td>
  </tr>
  <tr>
    <td><tt>std::string cron.expression()</tt></td>
    <td>return the command off the job</td>
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
    <td><tt>bool cron.error() (return true on error)</td>
    <td>allows to verify the cron expression</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.nextDate( std::tn* )</td>
    <td>Find the next rawtime of the cron job (-1 on error)</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.nextDate( const time_t* )</td>
    <td>Find the next rawtime of the cron job</td>
  </tr>
  <tr>
  <tr>
    <td><tt>const time_t cron.nextDate( const time_t& )</td>
    <td>Find the next rawtime of the cron job</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.previousDate( std::tn )</td>
    <td>give the last rawtime of the previous cron job exec</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.previousDate( const time_t* )</td>
    <td>give the last rawtime of the previous cron job exec</td>
  </tr>
  <tr>
    <td><tt>const time_t cron.previousDate( const time_t& )</td>
    <td>give the last rawtime of the previous cron job exec</td>
  </tr>
</table>

Cron string format : "S M H d m w [Y] cmd" (Year is optional ; default limit values of the year: +/- 8 years relative to the current year).

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
	Cron expression is: * 25-30 * * * * myCommand   // the year field is optional...
	The next "myCommand" task will be performed at: 1656905340 (2022/07/03 17:29:00).
	The last execution of the "myCommand" task was at: 1656905280 (2022/07/03 17:28:00).

...

Next job of a crontab :
=======================

	# define out /*
	g++ -o main $0 cron.cpp && ./main; exit $?
	# define out */
	# undef out

	#include <string>
	#include <vector>
	#include <iostream>
	#include "cron.h"

	int main() {
	std::vector<std::string> crontab = {
	      "5-55 55 * * apr * myCmd1",
	      "* 25-30 * * * * myCmd2",
	      "*/10 55 * * apr * 2021,2025 myCmd3",
	      "* 05-30 * * * 2-3 * myCmd4"
	};

	char buffer [80];
	time_t r, Now(time(NULL));
	time_t rawtime( cron( crontab[0] ).nextDate( Now ) );
	std::string job;
	cron c;

	strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&Now) );
	std::cout << "Now() is " << Now << " (" << buffer << "):" << std::endl << std::endl;
	
	for( int i(0); i<crontab.size(); i++){
	      c.clear() = crontab[i]; rawtime = c.nextDate(Now);
	      strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime( &rawtime ) );
	      std::cout << "The job \"" << c.expression() << "\" lanched at: " << rawtime << " (" << buffer << "), - \"" << crontab[i] << "\"" << std::endl;
	}

 	for( auto& x : crontab ){
	     c.assign(x);
	     if ( r != -1 &&( r = c.nextDate( Now ) ) < rawtime ){
	         rawtime = r;
		 job = c.expression();
        }    }

  	strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
  	std::cout << std::endl << "The next job will be \"" << job << "\" at: " << rawtime << " (" << buffer << ")." << std::endl << std::endl;

  	return 0;
  	}
	
Display:
---------

        Now() is 1656967888 (2022/07/04 10:51:28):

        The job "myCmd1" lanched at: 1680346505 (2023/04/01 00:55:05), - "5-55 55 * * apr * myCmd1"
        The job "myCmd2" lanched at: 1656969900 (2022/07/04 11:25:00), - "* 25-30 * * * * myCmd2"
        The job "myCmd3" lanched at: 1743504900 (2025/04/01 00:55:00), - "*/10 55 * * apr * 2021,2025 myCmd3"
        The job "myCmd4" lanched at: 1657015500 (2022/07/05 00:05:00), - "* 05-30 * * * 2-3 * myCmd4"

        The next job will be "myCmd2" at: 1656969900 (2022/07/04 11:25:00).
