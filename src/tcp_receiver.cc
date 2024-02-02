#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{

  // To access reassembler, use reassembler_.
  reassembler_.insert(first_index, message.payload, message.SYN);
  // Unwrap gets you absolute sequence number
  uint64_t abseqno = message.seqno.Unwrap(n, zero_point);


  
  if (message.RST) {
    // use set_error() from bytestream
  }



  if (message.SYN) {
    // this segment is beginning of the byte stream
    // conversation is starting
    // guaranteed to be sent first
    // don't start conversation until SYN flag is receieved 
    // first_unassembled_index is checkpoint, AKA bytes_pushed
    Wrap32 first_seg = message.seqno;
    uint64_t ISN = first_seg.unwrap(first_seg, writer().bytes_pushed())


  }
  if (message.FIN) {
    //this segment is the end of the byte stream
    // close conversation once FIN flag is received
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  return {};
}
