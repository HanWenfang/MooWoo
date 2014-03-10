MooWoo
======

A distributed programming framework : Master-Worker Pattern


				Master    - Zookeeper
			  /   |    \
			 /          \
			/     |      \
	  Worker1  Worker2  Worker3

Distributed Management Pattern:
	Rank Style:
		Master 	- Rank 0
		Worker1 - Rank 1
		...

False Tolerance Pattern:
	Many Workers
	Zookeeper For Master

Error Handling Pattern: [ Requester will deal with them. ]
	Timeout
	Buffer
	Idempotent: Retry

Concurrent Pattern:
	Single Thread now!!

Comunication Pattern:
	Get [long-time connection etc.]



Dependency Tools & Libraries:
	Poco C++  
	Zookeeper



