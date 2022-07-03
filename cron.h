/*  croncpp (Version 0.1 - 2022/06)
    <https://github.com/peychart/croncpp>

    Copyright (C) 2022  -  peychart

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program.
    If not, see <http://www.gnu.org/licenses/>.

    Details of this licence are available online under:
                        http://www.gnu.org/licenses/gpl-3.0.html
*/
#ifndef HEADER_F2542F1C2196442
#define HEADER_F2542F1C2196442

#include <bitset>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>

namespace cronTab
{
  typedef unsigned char      byte;
  typedef short unsigned int ushort;

  #define SCOPE_OF_YEARS    8
  enum field_name {second, minute, hour_of_day, day_of_month, month, day_of_week, year, expr=194 + SCOPE_OF_YEARS*2 + 1}; // Adressage : field_name::hour
  static const byte field_size[]= {60,60,24,31,12, 7, SCOPE_OF_YEARS*2 + 1};         // expr == sum ...
  static byte field_offset[]    = { 0, 0, 0, 1, 1, 0, SCOPE_OF_YEARS};
  static const char* weekDay[]  = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
  static const char* monthDay[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
  static const byte  npos = byte(-1);

  class cron : std::bitset<field_name::expr>
  {
  public:
    cron( void )             :_err(false) {init();};
    cron( std::string s )    :_err(false) {init(); assign( s );};
   ~cron( void )                          {clear();};

    inline       std::string  expression  ( void )                                {return _expression;};
    inline bool         error             ( void )                                {return convError();};

    inline cron&        clear             ( void )                                {convError(false); return unset();};
    cron&               assign            ( std::string s );
    inline cron&        operator=         ( std::string s )                       {return assign( s );};

    inline time_t       nextDate          ( std::tm* t )                          {return dateAround( *t );};
    inline time_t       nextDate          ( const time_t& rawtime )               {return nextDate( localtime( &rawtime ) );};
    inline time_t       previousDate      ( std::tm* t )                          {return dateAround( *t, false );};
    inline time_t       previousDate      ( const time_t& rawtime )               {return previousDate( localtime( &rawtime ) );};

  private:
    bool                _err;
    ushort              _year;
    std::string         _expression;

    cron( const std::tm* t ) :_err(false) {init(); assign( t );};
    inline void         init              ( void )                                {time_t rawtime(time(NULL)); _year = localtime(&rawtime)->tm_year;};

    inline cron&        unset             ( void )                                {for( byte i(0); i<field_name::expr; i++ ) set( i, false ); _expression.clear(); return *this;};
    cron&               assign            ( const std::tm* );
    inline cron&        assign            ( const time_t* t )                     {return assign( localtime( t ) ); };
    inline cron&        operator=         ( const std::tm* t )                    {return assign( t );};
    inline cron&        operator=         ( const time_t* t )                     {return assign( t );};

    byte                index             ( field_name const );
    void                setField          ( field_name const, std::string, bool v );
    inline void         setScope          ( field_name const nfield, bool v, byte begin, byte end, byte delta=1 ){
                                                                                   for( byte i( begin ); i<=end && !convError(); i += delta ) setBit( nfield, i - field_offset[nfield], v );};

    inline void         setBit            ( field_name const nfield, byte i, bool v=true ){
                                                                                   if( nfield<=field_name::year && i<field_size[nfield] ) this->set(index(nfield)+i, v); else convError( true );};
    inline void         setField          ( field_name const nfield, bool v )     {for( byte i(0), j(index(nfield)); i<field_size[nfield]; i++) set( i+j, v );};

    inline bool         isSet             ( field_name const nfield, byte i )     {return( (nfield<=year && i<field_size[nfield]) ?test(index(nfield)+i) :false );};
    inline bool         isSet             ( field_name const nfield )             {for(byte i(0); i<field_size[nfield]; i++ ) if( !test(index(nfield)+i) ) return false; return true;};
    inline byte         findBit           ( field_name const nfield, byte n=0 )   {for(byte i(n); i<field_size[nfield]; i++ ) if( isSet(nfield, i) ) return (i); return npos;}
    inline bool         existingBit       ( field_name const nfield, byte n )     {return( n < field_size[nfield] && nfield <= field_name::year );};

    inline void         convError         ( bool b )                              {_err=b;};
    inline bool         convError         ( void )                                {return _err;};

    time_t              dateAround        ( const std::tm&, bool=true );

    bool                splitString       ( std::string&, std::string, std::string& );
    inline bool         isNumeric         ( std::string& s )                      {for( byte i(0); i<s.size(); i++ ) if( !isdigit( s[i] ) ) return false; return s.size();};
    inline std::string& toUpper           ( std::string& s )                      {for( auto &x : s ) x=static_cast<char>( std::toupper(x) ); return s;};
    std::string&        trimString        ( std::string& );
    std::string&        normalizeField    ( field_name const nfield, std::string& );
    cron&               initRef           ( std::tm&, int*[] );
    int                 sizeOfMonth       ( std::tm&, bool=false );
 };
}
using namespace cronTab;
#endif
