#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "checkpspheader.hh"
#include <clicknet/label.h>
CLICK_DECLS

CheckPSPHeader::CheckPSPHeader()
{
}

CheckPSPHeader::~CheckPSPHeader()
{
}

int
CheckPSPHeader::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
CheckPSPHeader::initialize(ErrorHandler *)
{
	return 0;
}

void
CheckPSPHeader::push(int, Packet *p)
{
//	click_chatter("CheckPSPHeader PUSH...");
	const click_psp *psp = reinterpret_cast<const click_psp *>(p->data());
	if (psp->psp_len/16 > MTU_SIZE) {
		click_chatter("psp packet is too large: %d", psp->psp_len);
		output(1).push(p);
		return;
	}

	output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CheckPSPHeader)

