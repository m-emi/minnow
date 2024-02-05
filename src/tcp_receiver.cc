#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if (message.RST) {
    reader().set_error();
  }
  // Unwrap gets you absolute sequence number
  // Find stream index.
  // Should be absolute seqno - 1

  if (message.SYN) {
    // this segment is beginning of the byte stream
    // don't start conversation until SYN flag is receieved 
    ISN = message.seqno;
  }
  // Conversation can be started
  if (ISN.has_value()) {
      uint64_t abs_seqno = message.seqno.unwrap(ISN.value(), writer().bytes_pushed()) + 1;

      if (message.SYN && !message.FIN) {
        reassembler_.insert(0, message.payload, message.FIN);
      }
      else if (!message.SYN && !message.FIN) 
      {
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

    message.ackno = Wrap32::wrap(writer().bytes_pushed(), ISN.value() + 1);


    uint32_t offset = 1;
    message.ackno = message.ackno.value() + offset;
  }

  message.window_size = writer().available_capacity();
  if (writer().has_error()) {
    message.RST = true;
  }
  return message;
}
