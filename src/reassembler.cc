#include "reassembler.hh"
#include <algorithm>
#include <numeric>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{

  // Red region after first unassembled index and before first unacceptable index
  unassembled_bytes_.resize( writer().available_capacity() );
  bitmap_.resize( writer().available_capacity() );

  if ( is_last_substring ) {
    total_size_.emplace( first_index + data.size() );
  }

  const uint64_t first_unassembled_index = writer().bytes_pushed();
  // Discard bytes after this index
  const uint64_t first_unacceptable_index = first_unassembled_index + writer().available_capacity();

  // Find first part of data to add to unassembled_bytes. May not always be
  // data.begin() because part of data might be before first_unassembled_index
  const uint64_t sub_data_start = max( first_unassembled_index, first_index );
  // Find last part of data to add to unassembled_bytes.
  // May not always be data.end() because it can be cut off by
  // first unacceptable index
  const uint64_t sub_data_end = min( first_unacceptable_index, first_index + data.size() );

  // copy sub_data into apppropriate part of unassembled_bytes
  if ( sub_data_start < sub_data_end ) {
    copy( data.begin() + sub_data_start - first_index,
          data.begin() + sub_data_end - first_index,
          unassembled_bytes_.begin() + sub_data_start - first_unassembled_index );

    fill( bitmap_.begin() + sub_data_start - first_unassembled_index,
          bitmap_.begin() + sub_data_end - first_unassembled_index,
          true );
  }

  // Push bytes into the bytestream
  const uint64_t count = ranges::find( bitmap_, false ) - bitmap_.begin();

  // uint64_t count = 0;
  // while (bitmap_[0]) {
  //   count++;
  // }
  writer().push( unassembled_bytes_.substr( 0, count ) );
  unassembled_bytes_.erase( 0, count );
  bitmap_.erase( bitmap_.begin(), bitmap_.begin() + count );
  // while (bitmap_[0]) {
  //   output_.writer().push(unassembled_bytes_.substr(0,1));
  //   unassembled_bytes_.erase(0, 1);
  //   bitmap_.erase(bitmap_.begin());
  // }

  if ( total_size_.has_value() and total_size_.value() == writer().bytes_pushed() ) {
    writer().close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // uint64_t count = 0;
  // for (uint64_t i = 0; i < bitmap_.size(); i++) {
  //   if (bitmap_[i]) {
  //     count++;
  //   }
  // }
  // return count;
  return accumulate( bitmap_.begin(), bitmap_.end(), 0 );
}