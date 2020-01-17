#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "decpspttl.hh"
#include <clicknet/label.h>
CLICK_DECLS

DecPspTTL::DecPspTTL()
{
}

DecPspTTL::~DecPspTTL()
{
}

int
DecPspTTL::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
DecPspTTL::initialize(ErrorHandler *)
{
	return 0;
}

void
DecPspTTL::push(int, Packet *p)
{
	click_chatter("DecPspTTL PUSH...");

	WritablePacket *q = p->uniqueify();
	click_psp *psp = reinterpret_cast<click_psp *>(q->data());
	if (psp->psp_ttl <= 1) {
		click_chatter("DecPspTTL ttl expired");
		output(1).push(p);
	}
	else {
		psp->psp_ttl--;
		output(0).push(p);
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DecPspTTL)

