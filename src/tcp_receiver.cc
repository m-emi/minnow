#include "tcp_receiver.hh"
#include <algorithm>
#include <iostream>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if ( message.RST ) {
    reader().set_error();
  }
  // Unwrap gets you absolute sequence number
  // Find stream index.
  // Should be absolute seqno - 1
  if ( message.SYN && !ISN.has_value() ) {
    // don't start conversation until SYN flag is receieved
    ISN = message.seqno;
  }
  // // Conversation can be started
  if ( ISN.has_value() ) {
    uint64_t stream_index;
    if ( message.SYN ) {
      stream_index = 0;
    }
    if ( !message.SYN ) {
      stream_index = message.seqno.unwrap( ISN.value(), writer().bytes_pushed() ) - 1;
    }
    // uint64_t stream_index = message.seqno.unwrap(ISN.value(), writer().bytes_pushed()) - 1;
    reassembler_.insert( stream_index, message.payload, message.FIN );
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message;
  if ( ISN.has_value() ) {
    if ( writer().is_closed() ) {
      message.ackno = Wrap32::wrap( writer().bytes_pushed() + 2, ISN.value() );
    } else {
      message.ackno = Wrap32::wrap( writer().bytes_pushed() + 1, ISN.value() );
    }
  }
  // account for max window size
  uint64_t max_window_size = UINT16_MAX;
  message.window_size = min( writer().available_capacity(), max_window_size );
  if ( writer().has_error() ) {
    message.RST = true;
  }
  return message;
}
