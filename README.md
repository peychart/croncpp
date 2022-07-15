Croncpp:
=====================


Software:
---------

C++ source.

"Croncpp" is a light library (iot-friendly) for processing CRON expressions.

It implements two basic operations: find the next date or find the last date of execution of a periodic processing defined in cron format.


This allows to find the current status of the iot device after rebooting and plan its next state ...

<table>
  <tr>
    <td><tt>cron()</tt></td>
    <td>initialze a cron object</td>
  </tr>
  <tr>
    <td><tt>cron( std::string( `a cron format definition (see below)` )</tt></td>
    <td>initialize and set a cron job object</td>
  </tr>
  <tr>
    <td><tt>std::string cron.expression()</tt></td>
    <td>return the command of the job</td>
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

Format of a cron string : "S M H d m w [Y] cmd" - (Year is optional ; default limit values of the year: +/- 8 years relative to the current year).

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
	      "* 05-30 * * * 2-3 * myCmd4",
	      "* * * 31 * * myCmd5"
	};

	char buffer [80];
	time_t t, Now(time(NULL));
	time_t rawtime( cron( crontab[0] ).nextDate( Now ) );
	std::string job;
	cron c;

	strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&Now) );
	std::cout << "Now() is " << Now << " (" << buffer << "):" << std::endl << std::endl;
	
 	for( auto& x : crontab ){
 	   c.assign(x);
 	   t = c.nextDate( Now );

 	   strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime( &t ) );
 	   std::cout << "The job \"" << c.expression() << "\" will be lanched at: " << t << " (" << buffer << ") - \"" << x << "\"" << std::endl;

 	   if ( t != -1 && t < rawtime ){
 	      rawtime = t; job = c.expression();
        }    }

  	strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
  	std::cout << std::endl << "The next job will be \"" << job << "\" at: " << rawtime << " (" << buffer << ")." << std::endl;

 	rawtime=-1; for( auto& x : crontab ){
 	   c.assign(x);
 	   t = c.previousDate( Now );

 	   strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime( &t ) );
 	   std::cout << "The job \"" << c.expression() << "\" has been lanched at: " << t << " (" << buffer << ") - \"" << x << "\"" << std::endl;

 	   if ( t != -1 && t > rawtime ){
 	      rawtime = t; job = c.expression();
        }    }

  	strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
  	std::cout << std::endl << "The last job was \"" << job << "\" at: " << rawtime << " (" << buffer << ")." << std::endl << std::endl;

  	return 0;
  	}
	
Display:
---------

        Now() is 1657317135 (2022/07/08 11:52:15):

        The job "myCmd1" will be lanched at: 1680346505 (2023/04/01 00:55:05) - "5-55 55 * * apr * myCmd1"
        The job "myCmd2" will be lanched at: 1657319100 (2022/07/08 12:25:00) - "* 25-30 * * * * myCmd2"
        The job "myCmd3" will be lanched at: 1743504900 (2025/04/01 00:55:00) - "*/10 55 * * apr * 2021,2025 myCmd3"
        The job "myCmd4" will be lanched at: 1657620300 (2022/07/12 00:05:00) - "* 05-30 * * * 2-3 * myCmd4"
        The job "myCmd5" will be lanched at: 1659261600 (2022/07/31 00:00:00) - "* * * 31 * * myCmd5"

        The next job will be "myCmd2" at: 1657319100 (2022/07/08 12:25:00).

        The job "myCmd1" has been lanched at: 1651398955 (2022/04/30 23:55:55) - "5-55 55 * * apr * myCmd1"
        The job "myCmd2" has been lanched at: 1657315800 (2022/07/08 11:30:00) - "* 25-30 * * * * myCmd2"
        The job "myCmd3" has been lanched at: 1619862950 (2021/04/30 23:55:50) - "*/10 55 * * apr * 2021,2025 myCmd3"
        The job "myCmd4" has been lanched at: 1657186200 (2022/07/06 23:30:00) - "* 05-30 * * * 2-3 * myCmd4"
        The job "myCmd5" has been lanched at: 1653991200 (2022/05/31 00:00:00) - "* * * 31 * * myCmd5"

        The last job was "myCmd2" at: 1657315800 (2022/07/08 11:30:00).
