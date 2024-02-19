#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return zero_point + n;
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  const uint32_t wrapped_checkpoint = Wrap32::wrap( checkpoint, zero_point ).raw_value_;
  uint32_t high_diff = raw_value_ - wrapped_checkpoint;
  uint32_t low_diff = wrapped_checkpoint - raw_value_;

  // Underflow has occured.
  if ( low_diff <= checkpoint && low_diff < high_diff ) {
    return checkpoint - low_diff;
  } else {
    return checkpoint + high_diff;
  }
}
