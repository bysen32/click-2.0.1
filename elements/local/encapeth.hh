#ifndef CLICK_ENCAPETH_HH
#define CLICK_ENCAPETH_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
#include <click/gaprate.hh>
#include <click/packet.hh>
#include <clicknet/ether.h>
CLICK_DECLS


class EncapEth: public Element { public:

	EncapEth();
	 ~EncapEth();

	const char *class_name() const { return "EncapEth"; }
	const char *port_count() const	{ return "1/1"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:
	click_ether _ethh;
};

CLICK_ENDDECLS
#endif

