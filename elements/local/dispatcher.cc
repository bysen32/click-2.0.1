#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "dispatcher.hh"
#include <click/packet_anno.hh>
#include <clicknet/label.h>
#include <clicknet/ether.h>
#include <clicknet/psp.h>

CLICK_DECLS

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}

int
Dispatcher::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
Dispatcher::initialize(ErrorHandler *)
{
	return 0;
}

void
Dispatcher::push(int, Packet *p)
{
//	click_chatter("Dispatcher PUSH...");

/*	const click_ether *eth = reinterpret_cast<const click_ether *>(p->data());
	const click_label *label;
	const click_psp *psp;
	if (eth->ether_type == 0x0601)
	{
		label = reinterpret_cast<const click_label *>(eth+1);
		psp = reinterpret_cast<const click_psp *>(label+1);
	}
	else
	{
		psp = reinterpret_cast<const click_psp *>(eth+1);
	}
	click_chatter("dispatcher packet seq %d", psp->psp_id);
*/
	int qos = QOS_ANNO(p);
	output(qos).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Dispatcher)

