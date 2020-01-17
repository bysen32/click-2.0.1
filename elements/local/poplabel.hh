#ifndef CLICK_POPLABEL_HH
#define CLICK_POPLABEL_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS


class PopLabel: public Element { public:

	PopLabel();
	 ~PopLabel();

	const char *class_name() const { return "PopLabel"; }
	const char *port_count() const	{ return "1/1"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:

};

CLICK_ENDDECLS
#endif

