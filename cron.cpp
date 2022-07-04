/* Croncpp (Version 0.1 - 2022/06)
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
#include "cron.h"

namespace cronTab{

  byte cron::index( field_name const nfield ){
    static bool b(false);
    static byte index[field_name::year]={0};
    if(!b){b=true;
      for(byte i(1); i<=field_name::year; i++)
        index[i] = index[i-1] + field_size[i-1];
    }return index[nfield];
  }

  cron& cron::assign( std::string s ) {
    byte i(0);
    std::string second;
    clear(); convError( false ); trimString( s );

    do{
      if( !splitString( s, " ", second ) ){
        _expression = s;
        break;
      }setField( i, s, true );
      s = second;
    }while( !convError() && ++i <= field_name::year );
    if( i <= field_name::year )
          setField( field_name::year, true );
    else _expression = s;

    for( byte i(0); !convError() && i<=field_name::year; i++)
      if( isNotSet( field_name(i) ) ) {convError( true ); break;}

    return( convError() ?clear() :*this );
  }

  cron& cron::assign( const std::tm* t ){ // std::tm to cronFormat:
    std::stringstream s( std::stringstream::out );
    s      << (t->tm_sec     + field_offset[field_name::second])
    << " " << (t->tm_min     + field_offset[field_name::minute])
    << " " << (t->tm_hour    + field_offset[field_name::hour_of_day])
    << " " << (t->tm_mday -1 + field_offset[field_name::day_of_month])
    << " " << (t->tm_mon     + field_offset[field_name::month])
    << " " << (t->tm_wday    + field_offset[field_name::day_of_week])
    << " " << (1900 + t->tm_year)
    << " NIL";
    return assign( s.str() );
  }

  void cron::setField( field_name const nfield, std::string first, bool v ){
    std::string second;
    if( convError() ){
      return;

    }else if( splitString( first, ",", second ) ){
      setField( nfield, first, v ); setField( nfield, second, v );

    }else if( splitString( first, "-", second ) ){
      if( !isNumeric( normalizeField( nfield, first ) ) || !isNumeric( normalizeField( nfield, second ) ) )
            convError( true );
      else  setScope( nfield, v, atoi(first.c_str()), atoi(second.c_str()) );

    }else if( splitString( first, "/", second ) ){
      if( (first.compare("*")!=0 && !isNumeric( normalizeField( nfield, first ) )) || !isNumeric( second ) )
            convError( true );
      else {setScope( nfield, v, atoi(first.c_str()), field_size[ nfield ]-1, atoi(second.c_str()) );}

    }else if( first.compare( "*" ) == 0 ){
      setField( nfield, v );

    }else if( first.compare( "?" ) == 0 ){
      setField( nfield, v );

    }else if( first.compare( "L" ) == 0 ){
      if( nfield!=field_name::day_of_week && nfield!=field_name::day_of_month )
            convError( true );
      else {setBit( field_name(nfield), field_size[ nfield ]-1, v );}

    }else if( first.compare( "W" ) == 0 ){
      if( nfield!=field_name::day_of_week )
            convError( true );
      else {setBit( field_name(nfield), field_size[ nfield ]-1, v ); setBit( field_name(nfield), field_size[ nfield ]-2, v );}

    }else if( isNumeric( normalizeField( nfield, first ) ) ){
      setBit( nfield, atoi( first.c_str() ) - field_offset[nfield], v );

    }else convError( true );
}

  std::string& cron::normalizeField( field_name const nfield, std::string& s ){
    std::stringstream str( std::stringstream::out );
    if( field_name( nfield ) == field_name::day_of_week ){
      for(byte i(0); i< 6; i++) if( toUpper(s).compare( weekDay[i]  )==0 ) {str << i + field_offset[nfield]; s.assign( str.str() );}
    }else if( field_name( nfield ) == field_name::month ){
      for(byte i(0); i<12; i++) if( toUpper(s).compare( monthDay[i] )==0 ) {str << i + field_offset[nfield]; s.assign( str.str() );}
    }else if( field_name( nfield ) == field_name::year ){
      str << ( atoi(s.c_str()) - 1900 - _year + 2 * field_offset[nfield] ); s.assign( str.str() );
    }return s;
  }

  cron& cron::initRef( std::tm& timeinfo, int *tminfo[] ){
    static cron date; mktime( &timeinfo );  //call mktime to recompute timeinfo...
    tminfo[0] = &timeinfo.tm_sec;
    tminfo[1] = &timeinfo.tm_min;
    tminfo[2] = &timeinfo.tm_hour;
    tminfo[3] = &timeinfo.tm_mday;
    tminfo[4] = &timeinfo.tm_mon;
    tminfo[5] = &timeinfo.tm_wday;
    tminfo[6] = &timeinfo.tm_year;
    return date.assign( &timeinfo );
  }

  time_t  cron::dateAround( const std::tm& timeinfo, bool next ){bool match(true);
    if( convError() ) return time_t(-1);
    std::tm result( timeinfo );
    for( byte nfield(0); existingField(nfield); nfield++ ){
      int delta(0), *tminfo[7];
      cron& date( initRef( result, tminfo) );
      int monthSize( (nfield==field_name::day_of_month) ?sizeOfMonth( result ) :0 );
      byte i( date.findBit( field_name(nfield), 0 ) ), j(i);
      if( i==npos)    return time_t(-1);

      if( isSet( field_name(nfield) ) && !(nfield==field_name::year && match) ){
        tminfo[nfield]=0;
        continue;
      }else while( true ){
        if( isSet( field_name(nfield), i ) )
          break;
        delta += (next ?1 :-1);
        if( ( (i += (next ?1 :-1)) >= ( monthSize ?monthSize :field_size[nfield] ) ) )
          i = ( next ?0 :( ( monthSize ?monthSize :field_size[nfield] ) - 1 ) );
        if( i == j )  return time_t(-1);
      }

      if( delta ){match=false;
        *tminfo[(nfield == field_name::day_of_week) ?field_name::day_of_month :nfield] += delta;
        while( existingField(--nfield ) ) *tminfo[nfield] = ( next ?( (nfield==field_name::day_of_month) ?1 :0 ) :( monthSize ?monthSize : field_size[nfield]-1) );
      }else if( nfield == field_name::year && match ){                                                   // Ref date matchs with cron!...
        for( nfield=0; nfield<=field_name::year; nfield++ ) if( !isSet(nfield) ){  // searching for first not "*"
          if(next){
            if( isSet( nfield, *tminfo[nfield] + (next ?1 :-1) - (nfield==field_name::day_of_month) ) ){ // previous on cron "* * * A-B(matching) * * * cmd"
                   setField( nfield, false ); setBit( nfield, *tminfo[nfield] + (next ?1 :-1), true );
            }else  *tminfo[nfield]+=1;
            nfield=-2;
          }else{ while( existingField(--nfield) ) *tminfo[nfield]=0; nfield=-3; break;}                  // previous on cron "* * * X(matching) * * * cmd"
        }if( nfield==-1 )return time_t(-1 );                                       // cron is "* * * * * * * cmd"
    } }
    return mktime( &result );
  }

  bool cron::splitString( std::string& first, std::string pattern, std::string& second ){
    std::size_t index( first.find_first_of( pattern ) );
    if( index == std::string::npos ){
      second.clear();
      return false;
    }second = first.substr( index+1 ); first.erase( index );
    return( second.size() );
  }

  std::string& cron::trimString( std::string& s ){
    for( auto &x : s ) if( x=='\t' ) x=' ';
    s.erase( 0, s.find_first_not_of(' ') ); for( std::size_t i(s.find("  ")); i!=std::string::npos; i=s.find("  ") ) {s.replace( i, 2, " " );};
    return s;
  }

  int cron::sizeOfMonth(std::tm& v, bool prev ){
    static std::tm t; t.tm_isdst=t.tm_wday=t.tm_yday=t.tm_sec=t.tm_min=t.tm_hour=0;
    t.tm_mday=31; t.tm_mon=v.tm_mon-prev; t.tm_year = v.tm_year; mktime( &t );
    return( t.tm_mon==v.tm_mon ?31 :(31-t.tm_mday) );
  }
}
