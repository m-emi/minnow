#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  (void)is_last_substring;
  // Red region after first unassembled index and before first unacceptable index
  unassembled_bytes_.resize(output_.writer().available_capacity());
  bitmap_.resize(output_.writer().available_capacity());

  const uint64_t first_unassembled_index = output_.writer().bytes_pushed();
  // Discard bytes after this index
  const uint64_t first_unacceptable_index = first_unassembled_index + 
                  output_.writer().available_capacity();


  // Find first part of data to add to unassembled_bytes. May not always be 
  // data.begin() because part of data might be before first_unassembled_index
  const uint64_t sub_data_start = max(first_unassembled_index, first_index);
  // Find last part of data to add to unassembled_bytes.
  // May not always be data.end() because it can be cut off by
  // first unacceptable index
  const uint64_t sub_data_end = min(first_unacceptable_index, data.size());

  // copy sub_data into apppropriate part of unassembled_bytes
  copy(data.begin() + sub_data_start - first_index, 
       data.begin() + sub_data_end - first_index,
       unassembled_bytes_.begin() + sub_data_start - first_unassembled_index);

  // Push bytes into the bytestream
  const int64_t index = 0;
  while (bitmap_[index]) {
    output_.writer().push(unassembled_bytes_.substr(0,1));
    unassembled_bytes_.erase(0, 1);
    bitmap_.erase(bitmap_.begin());
  }



  









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
