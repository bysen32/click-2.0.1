#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "encapeth.hh"
#include <click/etheraddress.hh>
#include <click/packet_anno.hh>
CLICK_DECLS

EncapEth::EncapEth()
{
}

EncapEth::~EncapEth()
{
}

int
EncapEth::configure(Vector<String> &conf, ErrorHandler *errh)
{
	if (Args(conf, this, errh)
		.read_mp_with("SRCETH", EtherAddressArg(), _ethh.ether_shost)
		.read_mp_with("DSTETH", EtherAddressArg(), _ethh.ether_dhost)
		.complete() < 0)
		return -1;	
	_ethh.ether_type = htons(0x0601);
	return 0;
}

int
EncapEth::initialize(ErrorHandler *)
{
	return 0;
}

void
EncapEth::push(int, Packet *p)
{
//	click_chatter("EncapEth PUSH...");

	if (LABEL_OUT_ANNO(p) == 0)
		{
		_ethh.ether_type = htons(0x0600);
		click_chatter("set ether type to psp");
		}
	WritablePacket *q = p->uniqueify();
	q->push(14);
	click_ether *ethh = reinterpret_cast<click_ether *>(q->data());
	memcpy(q->data(), &_ethh, 14);
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(EncapEth)

