#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "declabelttl.hh"
#include <clicknet/label.h>
CLICK_DECLS

DecLabelTTL::DecLabelTTL()
{
}

DecLabelTTL::~DecLabelTTL()
{
}

int
DecLabelTTL::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
DecLabelTTL::initialize(ErrorHandler *)
{
	return 0;
}

void
DecLabelTTL::push(int, Packet *p)
{
//	click_chatter("DecLabelTTL PUSH...");

	const click_label *label = reinterpret_cast<const click_label *>(p->data());
	if (label->label_ttl <= 1)
		output(1).push(p);
	else {
		WritablePacket *q = p->uniqueify();
		click_label *lab = reinterpret_cast<click_label *>(q->data());
		lab->label_ttl --;
		output(0).push(p);
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DecLabelTTL)

