#ifndef CLICK_DISPATCHER_HH
#define CLICK_DISPATCHER_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS


class Dispatcher: public Element { public:

	Dispatcher();
	 ~Dispatcher();

	const char *class_name() const { return "Dispatcher"; }
	const char *port_count() const	{ return "1-/8"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:

};

CLICK_ENDDECLS
#endif

