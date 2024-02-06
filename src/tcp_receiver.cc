#include "tcp_receiver.hh"

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
  }

  if (message.FIN)
  {
    FIN_recieved = true;
  }
  
  // // Conversation can be started
  if (ISN.has_value()) 
  {
      uint64_t abs_seqno = message.seqno.unwrap(ISN.value(), writer().bytes_pushed());

      if (message.SYN && !message.FIN) 
      {
        reassembler_.insert(abs_seqno, message.payload, message.FIN);
      }

      else if (!message.SYN && !message.FIN) 
      {
        reassembler_.insert(abs_seqno + 1, message.payload, message.FIN);
      }

      else if (message.FIN) 
      {
        // stream should be closed if all bytes have been passed into the stream 
        // Bytes still might be left even if a FIN flag is passed
        reassembler_.insert(abs_seqno + 1, message.payload, message.FIN);
      }
      
  }

  // if (message.FIN) {
  //   //this segment is the end of the byte stream
  //   // close conversation once FIN flag is received
  //   uint64_t abs_seqno = message.seqno.unwrap(message.seqno, writer().bytes_pushed());
  //   uint64_t stream_index = abs_seqno - 1;
  //   reassembler_.insert(stream_index, message.payload, message.FIN);
  // }

}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message;
  if (ISN.has_value()) {

    // Check if FIN flag has been received and byte stream is fully popped and closed
    if (FIN_recieved && reader().is_finished())
    {
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 2, ISN.value() );
    }
    // FIN flag received, but byte stream not yet finished
    else if (FIN_recieved && !reader().is_finished())
    {
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 1, ISN.value() );
    }
    // Only SYN flag has been received
    else
    {
      message.ackno = Wrap32::wrap(writer().bytes_pushed() + 1, ISN.value() );
    }
    
    
   

    // uint32_t offset = 1;
    // message.ackno = message.ackno.value() + offset;
  }
  // account for max window size
  message.window_size = writer().available_capacity();
  if (writer().has_error()) {
    message.RST = true;
  }
  return message;
}
