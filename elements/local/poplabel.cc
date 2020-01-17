#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "poplabel.hh"
#include <clicknet/label.h>
CLICK_DECLS

PopLabel::PopLabel()
{
}

PopLabel::~PopLabel()
{
}

int
PopLabel::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
PopLabel::initialize(ErrorHandler *)
{
	return 0;
}

void
PopLabel::push(int, Packet *p)
{
	click_chatter("PopLabel PUSH...");
	WritablePacket *q = p->uniqueify();
	click_label *label = reinterpret_cast<click_label *>(q->data());
	click_psp *psp = reinterpret_cast<click_psp *>(label+1);
	int qos = label->label_qos;
	int ttl = label->label_ttl;
	psp->psp_ttl = ttl;
	q->pull(sizeof(click_label));
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PopLabel)

