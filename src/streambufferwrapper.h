// -*-c++-*-

/***************************************************************************
                 StreamBufferWrapper.hpp  -  A socket stream buffer
                             -------------------
    begin                : 08-JAN-2003
    copyright            : (C) 2003 by The RoboCup Soccer Server
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/


#ifndef STREAMBUFFERWRAPPER_H
#define STREAMBUFFERWRAPPER_H


//g++ 2.95.6 doesn't have the streambuf header, so iostream is used instead
//#include <streambuf>
#include <iostream>
#include "serverclientmediator.h"

class StreamBufferWrapper
    : public std::streambuf {
public:
//     typedef int int_type;
    typedef char char_type;

private:

    ClientServerMediator M_mediator;
    std::streamsize M_bufsize;
    char_type * M_inbuf;
    char_type * M_outbuf;

    // not used
    StreamBufferWrapper( const StreamBufferWrapper & );
    // not used
    StreamBufferWrapper & operator=( const StreamBufferWrapper & );

public:
    StreamBufferWrapper( std::streamsize bufsize = 8192 );

    virtual ~StreamBufferWrapper();

private:

    bool writeData();

protected:

    virtual
    int_type overflow( int_type c = traits_type::eof() );

    virtual
    int sync();

    virtual
    int_type underflow();

};

#endif
