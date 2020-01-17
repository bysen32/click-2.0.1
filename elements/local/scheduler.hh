#ifndef CLICK_SCHEDULER_HH
#define CLICK_SCHEDULER_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS


class Scheduler: public Element { public:

	Scheduler();
	 ~Scheduler();

	const char *class_name() const { return "Scheduler"; }
	const char *port_count() const	{ return "8/1"; }
	const char *processing() const	{ return PULL; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	Packet * pull(int port);


protected:
	int _curInput;
};

CLICK_ENDDECLS
#endif

