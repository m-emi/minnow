#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  (void)first_index;
  (void)data;
  (void)is_last_substring;


  // Red region after first unassembled index and before first unacceptable index
  unassembled_bytes_.resize(output_.writer().available_capacity());




  const uint64_t first_unassembled_index = output_.writer().bytes_pushed();

  // Discard bytes after this index
  const uint64_t first_unacceptable_index = first_unassembled_index + 
                  output_.writer().available_capacity();



// Push assembled bytes


}

uint64_t Reassembler::bytes_pending() const
{
  uint64_t count = 0;
  for (int i = 0; i < bitmap_.size(); i++) {
    if (bitmap_[i]) {
      count++;
    }
  }
  return count;
}
