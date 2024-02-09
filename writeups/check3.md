Checkpoint 3 Writeup
====================

My name: [Emilio Melgoza]

My SUNet ID: [emilio18]

I collaborated with: [patwei]

I would like to thank/reward these classmates for their help: [patwei]

This checkpoint took me about [6] hours to do. I [did] attend the lab session.

Program Structure and Design of the TCPSender [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]: 

I implemented a queue that keeps track of all the outstanding messages. Messages 
are able to be created and transmitted while there is space available in the 
window. Whenever there is an ackno received that is greater than the previous 
highest ackno, we then know that previous absolute sequence numbers have been
received. At that point, the TCPSenderMessage elements from the queue are popped.
At first, I thought of implementing a priority queue that is sorted from min to
max. The value that would be compared would have been the ackno, but I realized 
that if I store a TCPSenderMessage instead, I could just access the message 
sequence number from there.



Implementation Challenges:
[]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
