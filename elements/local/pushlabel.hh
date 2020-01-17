#ifndef CLICK_PUSHLABEL_HH
#define CLICK_PUSHLABEL_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS


class PushLabel: public Element { public:

	PushLabel();
	 ~PushLabel();

	const char *class_name() const { return "PushLabel"; }
	const char *port_count() const	{ return "1/1"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:

};

CLICK_ENDDECLS
#endif

