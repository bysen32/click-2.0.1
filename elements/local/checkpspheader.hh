#ifndef CLICK_CHECKPSPHEADER_HH
#define CLICK_CHECKPSPHEADER_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS

#define MTU_SIZE 1500

class CheckPSPHeader: public Element { public:

	CheckPSPHeader();
	 ~CheckPSPHeader();

	const char *class_name() const { return "CheckPSPHeader"; }
	const char *port_count() const	{ return "1/2"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:

};

CLICK_ENDDECLS
#endif

