MooWoo
======

A distributed programming framework : Master-Worker Pattern

								master0
								master0_
								master0_
								...
								/   |   \
							   /    |    \
							  /     |     \
							 /      |      \
							/       |       \
						worker1   worker2   worker3
						worker1_  worker2_  worker3_
						worker1_  worker2_  worker3_
						...


Distributed Management Pattern:
*	Rank Style:

		Master 	- Rank 0
		Worker1 - Rank 1
		...

False Tolerance Pattern:
*	Many Workers
*	Zookeeper For Master

Error Handling Pattern: [ Requester will deal with them. ]
*	Timeout
*	Buffer
*	Idempotent: Retry

Concurrent Pattern:
*	Multi-Thread
*   IO Multiplex

Comunication Pattern:
*	Get [long-time connection etc.]



Dependency Tools & Libraries:
*	Poco C++  
*	Zookeeper
*	MumanPlatform


