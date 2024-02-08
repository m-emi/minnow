#include "tcp_sender.hh"
#include "tcp_config.hh"
#include <iostream> 

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  return seqnos_in_flight_;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return {};
}

void TCPSender::push( const TransmitFunction& transmit )
{
  // TCPSender asked to fill the window
  // reads from the stream and sends as many TCPSenderMessages as possible as long as there are new bytes to be read and space in the window

  // make sure each TCPSenderMessage fits fully inside receiver's window
  // make each individual message as big as possible, but no bigger than MAX_PAYLOAD_SIZE

  // use TCPSenderMEssage::sequence_length() to count total number of sequence numbers occupied by segment
    // SYN and FIN also occupy space in window


  // Use read helper function from bytestream, it peeks and pops
  // checkpoint should be bytes_popped

  // why create a message if you are not going to send it...
  // only create it if you can send it.

  while (window_size_ > sequence_numbers_in_flight()) {
    TCPSenderMessage msg;
    // track if SYN has sent
    if (!SYN_sent_) 
    {
      msg.SYN = true;
      msg.seqno = isn_;
    }


    seqnos_in_flight_ += msg.sequence_length();
    transmit(msg);
  }




  

  
  // while (window_start > 0)
  // {
  //   transmit(sender_msg_queue_.front());
  //   sender_msg_queue.pop();
  // }
  // (void)transmit;
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  TCPSenderMessage empty_sender_msg;
  empty_sender_msg.seqno = Wrap32::wrap(reader().bytes_popped() + 1, isn_);
  return empty_sender_msg;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  if (msg.ackno.has_value()) 
  {
    window_size_ = msg.window_size - seqnos_in_flight_;
   // uint64_t abs_seqno = msg.ackno.value().unwrap(isn_, curr_abs_seqno_);
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  (void)ms_since_last_tick;
  (void)transmit;
  (void)initial_RTO_ms_;
}
