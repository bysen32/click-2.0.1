#ifndef CLICK_DECPSPTTL_HH
#define CLICK_DECPSPTTL_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS


class DecPspTTL: public Element { public:

	DecPspTTL();
	 ~DecPspTTL();

	const char *class_name() const { return "DecPspTTL"; }
	const char *port_count() const	{ return "1/2"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:

};

CLICK_ENDDECLS
#endif

