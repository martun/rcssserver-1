
#include "streambufferwrapper.h"

StreamBufferWrapper::StreamBufferWrapper( std::streamsize bufsize )
    : M_mediator(),
      M_bufsize( bufsize ),
      M_inbuf( NULL ),
      M_outbuf( NULL )
{
    M_outbuf = new char_type[M_bufsize];
    setp( M_outbuf, M_outbuf + M_bufsize );
}

StreamBufferWrapper::~StreamBufferWrapper()
{
    overflow();
    if ( M_inbuf ) delete [] M_inbuf;
    if ( M_outbuf ) delete [] M_outbuf;
}



bool
StreamBufferWrapper::writeData()
{
    unsigned long size = (unsigned long)( (pptr() - M_outbuf ) * sizeof( char_type ) );
    if ( size == 0 )
    {
        return true;
    }

    M_mediator.sendToClients( M_outbuf, size );
    return true;
}


StreamBufferWrapper::int_type
StreamBufferWrapper::overflow( int_type c )
{
    // this method is supposed to flush the put area of the buffer
    // to the I/O device

    // if the buffer was not already allocated nor set by user,
    // do it just now
    if ( ! writeData() )
    {
        return traits_type::eof();
    }

    setp( M_outbuf, M_outbuf + M_bufsize );
    if ( c != traits_type::eof() )
    {
        sputc( c );
    }
    return 0;
}


int
StreamBufferWrapper::sync()
{
    // just flush the put area
    if ( ! writeData() )
    {
        return traits_type::eof();
    }

    setp( M_outbuf, M_outbuf + M_bufsize );
    return 0;
}


StreamBufferWrapper::int_type
StreamBufferWrapper::underflow()
{
    // this method is supposed to read some bytes from the I/O device

    // if the buffer was not already allocated nor set by user,
    // do it just now
    if ( gptr() == NULL )
    {
        M_inbuf = new char_type[M_bufsize];
    }

    int readn = M_bufsize * sizeof( char_type );
    readn = M_mediator.recvFromClients(M_inbuf, readn);

    int totalbytes = readn;
    setg( M_inbuf, M_inbuf,
          M_inbuf + totalbytes / sizeof( char_type ) );

    return sgetc();
}
