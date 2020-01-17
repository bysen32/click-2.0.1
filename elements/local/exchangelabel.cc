#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "exchangelabel.hh"
#include <clicknet/label.h>
#include <click/packet_anno.hh>
CLICK_DECLS

ExchangeLabel::ExchangeLabel()
{
}

ExchangeLabel::~ExchangeLabel()
{
}

int
ExchangeLabel::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
ExchangeLabel::initialize(ErrorHandler *)
{
	return 0;
}

void
ExchangeLabel::push(int, Packet *p)
{
//	click_chatter("ExchangeLabel PUSH...");
	WritablePacket *q = p->uniqueify();
	click_label *label = reinterpret_cast<click_label *>(q->data());
	label->label_id = LABEL_OUT_ANNO(p);
//	click_chatter("exchanged label id = %d", label->label_id);
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ExchangeLabel)

