#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "pushlabel.hh"
#include "lookuplib.hh"
#include <clicknet/label.h>
CLICK_DECLS

PushLabel::PushLabel()
{
}

PushLabel::~PushLabel()
{
}

int
PushLabel::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
PushLabel::initialize(ErrorHandler *)
{
	return 0;
}

void
PushLabel::push(int, Packet *p)
{
//	click_chatter("PushLabel PUSH...");
	WritablePacket *q = p->uniqueify();
	click_psp *psp = reinterpret_cast<click_psp *>(q->data());
	int ttl = psp->psp_ttl;
	int tos = psp->psp_resv;
	q->push(sizeof(click_label));
	click_label *label = reinterpret_cast<click_label *>(q->data());
	label->label_qos = tos;
	label->label_ttl = ttl;
	label->label_id = LABEL_IN_NULL;
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PushLabel)

