#include "tcp_sender.hh"
#include "tcp_config.hh"
#include "tcp_sender_message.hh"
#include "wrapping_integers.hh"
#include <iostream> 

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  if (!outstanding_queue_.empty())
  {
    return next_seqno_ - outstanding_queue_.front().seqno.unwrap(isn_, next_seqno_);
  }
  return 0;
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

  // There is nothing to push.
  if (reader().bytes_buffered() == 0 && next_seqno_ != 0)
  {
    return;
  }

  //while (window_size_ > sequence_numbers_in_flight()) 
  //{
    TCPSenderMessage msg;
    // track if SYN has sent
    if (next_seqno_ == 0) 
    {
      msg.SYN = true;
      msg.seqno = isn_;
    }
    // SYN already sent
    else
    {
      msg.seqno = Wrap32::wrap(next_seqno_,isn_); // Wrap32
    }
    
    // Get payload 
    uint64_t payload_size = min(TCPConfig::MAX_PAYLOAD_SIZE, window_size_ - sequence_numbers_in_flight() - msg.SYN + 1);
    string payload;
    read(input_.reader(), payload_size, payload);
    msg.payload = payload;


    // remove segment of payload from the bytestream
    //input_.reader().pop(payload_size);

    next_seqno_ += msg.sequence_length();

    // add to queue
    outstanding_queue_.push(msg);
    // transmit
    transmit(msg);
  //}

}

TCPSenderMessage TCPSender::make_empty_message() const
{
  TCPSenderMessage empty_sender_msg;
  empty_sender_msg.seqno = Wrap32::wrap(next_seqno_, isn_);
  return empty_sender_msg;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  if (msg.ackno.has_value()) 
  {
    window_size_ = msg.window_size - sequence_numbers_in_flight();
    uint64_t abs_seqno = msg.ackno.value().unwrap(isn_, next_seqno_);


    // Check of the queue is non empty
    // if (!outstanding_queue_.empty())
    // {
    //   TCPSenderMessage front_msg = outstanding_queue_.front();
    //   uint64_t front_seqno = front_msg.seqno.unwrap(isn_, next_seqno_) + front_msg.sequence_length();

    //   // If your received seqno >= oldest seqno, oldest msg can be popped.
    //   // front 
    //   if ( abs_seqno >= front_seqno )
    //   {
    //     outstanding_queue_.pop();
    //   }
    // }
    while (!outstanding_queue_.empty())
    {
      TCPSenderMessage front_msg = outstanding_queue_.front();
      uint64_t front_seqno = front_msg.seqno.unwrap(isn_, next_seqno_) + front_msg.sequence_length();

      // If your received seqno >= oldest seqno, oldest msg can be popped.
      if ( abs_seqno >= front_seqno)
      {
        outstanding_queue_.pop();
      }
      else 
      {
        break;
      }
    }
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  (void)ms_since_last_tick;
  (void)transmit;
  (void)initial_RTO_ms_;
}
