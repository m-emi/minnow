#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  buf_ += data;
  return;
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
  // Your code here.
  return {};
}

bool Reader::is_finished() const
{
  // Your code here.
  return {};
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return {};
}

string_view Reader::peek() const
{
  
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  (void)len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return {};
}
