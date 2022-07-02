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
  std::string cronexpr( "5-55 55 * * apr * 2021,2025 myCommand" );

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
