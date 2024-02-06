#include "tcp_receiver.hh"
#include <iostream> 
#include <algorithm>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if (message.RST) 
  {
    reader().set_error();
  }
  // Unwrap gets you absolute sequence number
  // Find stream index.
  // Should be absolute seqno - 1
  if (message.SYN) 
  {
    // this segment is beginning of the byte stream
    // don't start conversation until SYN flag is receieved 
    ISN = message.seqno; 
    cerr << ISN.has_value();
  }
  if (message.FIN)
  {
    // last segment of the bytestream has been sent
    // Bytestream may still be open however
    FIN_recieved = true;
  }
  
  // // Conversation can be started
  if (ISN.has_value()) 
  {
      uint64_t stream_index = message.seqno.unwrap(ISN.value(), writer().bytes_pushed()) - 1;

      if (message.SYN && !message.FIN) 
      {
        reassembler_.insert(stream_index, message.payload, message.FIN);
      }

      else if (message.SYN && message.FIN) 
      {
        reassembler_.insert(stream_index + 1, message.payload, message.FIN);
      }

      // else if (message.FIN) 
      // {
      //   // stream should be closed if all bytes have been passed into the stream 
      //   // Bytes still might be left even if a FIN flag is passed
      //   reassembler_.insert(abs_seqno + 1, message.payload, message.FIN);
      // } 
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message;

  if (ISN.has_value()) 
  {
    // Check if FIN flag has been received and byte stream is fully popped and closed
    if (FIN_recieved && reader().is_finished())
    {
      cerr << "case 1";
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 2, ISN.value() );
    }
    // FIN flag received, but byte stream not yet finished
    else if (FIN_recieved && !reader().is_finished())
    {
      cerr << "case 2";
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 1, ISN.value() );
    }
    // Only SYN flag has been received
    else
    {
      cerr << "case 3";
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 1, ISN.value() );
    }
  }

  //account for max window size
  uint64_t max_window_size = UINT16_MAX;
  message.window_size = min(writer().available_capacity(), max_window_size);

  if (writer().has_error()) {
    message.RST = true;
  }
  return message;
}
