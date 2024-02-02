#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return zero_point + n;
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  const Wrap32 wrapped_checkpoint = wrap(checkpoint, zero_point);
  uint64_t high_diff = wrapped_checkpoint.raw_value_ - checkpoint;
  uint64_t low_diff = checkpoint - wrapped_checkpoint.raw_value_;
  
  // Underflow has occured. 
  if (low_diff > checkpoint || ) {
    return checkpoint + high_diff;
  }
  if (low_diff > high_diff) {
      return checkpoint + high_diff;
    }
  // raw value is 
  return checkpoint - low_diff;





}
