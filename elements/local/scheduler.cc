#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "scheduler.hh"

CLICK_DECLS

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}

int
Scheduler::configure(Vector<String> &conf, ErrorHandler *errh)
{
	return 0;
}

int
Scheduler::initialize(ErrorHandler *)
{
	_curInput = 0;
	return 0;
}

Packet *
Scheduler::pull(int)
{
	Packet *p = input(7).pull();
	if (!p)
	{
		p = input(_curInput).pull();
		_curInput = ( _curInput + 1 ) % 7;
	}
	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Scheduler)

