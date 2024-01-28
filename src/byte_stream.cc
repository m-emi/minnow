#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  if ( data.size() > available_capacity() ) {
    data.resize( available_capacity() );
  }
  buf_ += data;
  bytes_pushed_ += data.size();
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buf_.size();
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  if ( buf_.empty() && closed_ ) {
    return true;
  }
  return false;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}

string_view Reader::peek() const
{
  return buf_;
}

void Reader::pop( uint64_t len )
{
  if ( len > buf_.size() ) {
    len = buf_.size();
  }
  buf_.erase( 0, len );
  bytes_popped_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  return buf_.size();
}
