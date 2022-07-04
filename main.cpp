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

  std::string name; for( auto& x : crontab ){
    c.assign(x);
    if ( r != -1 &&( r = c.nextDate( Now ) ) < rawtime ){
        rawtime = r; job = c.expression();
  }     }

  strftime ( buffer, 80, "%Y/%m/%d %H:%M:%S", localtime(&rawtime) );
  std::cout << std::endl << "The next job will be \"" << job << "\" at: " << rawtime << " (" << buffer << ")." << std::endl << std::endl;

  return 0;
}
