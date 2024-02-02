#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return zero_point + n;
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  const Wrap32 wrapped_checkpoint = wrap(checkpoint, zero_point);
  const int64_t signed_wrapped_checkpoint = wrapped_checkpoint.raw_value_;
  int64_t diff = wrapped_checkpoint.raw_value_ - checkpoint;


  const int64_t high_diff = wrapped_checkpoint.raw_value_ - checkpoint;
  const int64_t low_diff = checkpoint - wrapped_checkpoint.raw_value_;





  if (diff == 0) {
    return checkpoint;
  }
  if (diff < 0) {
    return checkpoint + 2**(32)
  }

 
  return {};
}
