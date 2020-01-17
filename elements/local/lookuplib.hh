#ifndef CLICK_LOOKUPLIB_HH
#define CLICK_LOOKUPLIB_HH
#include <click/error.hh>
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
CLICK_DECLS

#define LABEL_IN_NULL	0
#define LABEL_OUT_ACT	0

struct LabelInfor {
	int 		labIn;
	int 		intfIn;
	int		labOut;
	int		intfOut;
	uint64_t 	addrPrefix;
	uint64_t	nextPsp;
};

class LookupLIB: public Element { public:

	LookupLIB();
	 ~LookupLIB();

	const char *class_name() const { return "LookupLIB"; }
	const char *port_count() const	{ return "1/-"; }
	const char *processing() const	{ return PUSH; }

	int configure(Vector<String>&, ErrorHandler*);
	int initialize(ErrorHandler *);

	void push(int port, Packet *p);


protected:
	Vector<LabelInfor> _t;

	int add_label_infor(const LabelInfor& li, ErrorHandler *errh);
	int lookup_entry(int labelIn) const;
	const char* basic_parse(const char *s, const char *end,
			  unsigned char value[8], int &nbytes);
	bool parseAddr(const String &str, uint64_t &result);
	bool parseInt(const String &str, int &port);

};

CLICK_ENDDECLS
#endif

