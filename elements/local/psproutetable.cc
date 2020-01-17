/*
 * psproutetable.{cc,hh} -- a stupid PSP routing table, best for small routing tables
 * Ricky Huang
 *
 */
#include <iostream>
#include <click/config.h>
#include <click/ipaddress.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include <click/router.hh>
#include "psproutetable.hh"

CLICK_DECLS

const char *
basic_parse(const char *s, const char *end,
			  unsigned char value[8], int &nbytes)
{
    memset(value, 0, 8);
    int d = 0;
    while (d < 8 && s != end && (d == 0 || *s == '.')) {
	const char *t = s + !!d;
	if (t == end || !isdigit((unsigned char) *t))
	    break;
	int part = *t - '0';
	for (++t; t != end && isdigit((unsigned char) *t) && part <= 255; ++t)
	    part = part * 10 + *t - '0';
	if (part > 255)
	    break;
	s = t;
	value[d] = part;
	if (++d == 8)
	    break;
    }
    nbytes = d;
    return s;
}

bool
parseAddr(const String &str, uint64_t &result)
{
    unsigned char value[8];
    int nbytes;
    if (basic_parse(str.begin(), str.end(), value, nbytes) == str.end()
	&& nbytes == 8) {
		memcpy(&result, value, 8);
		return true;
    }
    return false;
}

bool
parseInt(const String &str, int &port)
{
	const char *s = str.begin(), *end = str.end();
	const char *t = s;
	if (t == end || !isdigit((unsigned char) *t))
	    return false;
	int part = *t - '0';
	for (++t; t != end && isdigit((unsigned char) *t) && part <= 255; ++t)
	    part = part * 10 + *t - '0';
	port = part;
//	click_chatter("parseInt: %d\n", part);
	return true;
}

bool
PSPRouteTable::cp_psp_route(String s, ErrorHandler *errh)
{
    	uint64_t addr;
    	if (!parseAddr(cp_shift_spacevec(s), addr))
		return false;

    	String word = cp_shift_spacevec(s);
    	if (word == "-")
	/* null gateway; do nothing */;
    	else
	{
		int port;
		if (parseInt(word, port) && !cp_shift_spacevec(s)) // nothing left
		{
			PSPAddress temp(addr);
			PSPRoute r(temp, port);
			add_route(r, false, 0, errh);
			return true;
		}
	}

    	return false;
}

int
PSPRouteTable::configure(Vector<String> &conf, ErrorHandler *errh)
{
//    click_chatter("PSPRouteTable: configure the psp_route_table ...");
    int r = 0, r1, eexist = 0;
    String conf_item;
    PSPRoute route;

    for (int i = 0; i < conf.size(); i++) {
           cp_psp_route(conf[i], errh);
    }


    PSPAddress temp(0x00000000d60cc5ca) ;
	
    PSPRoute route_tmp(temp,2);

    add_route(route_tmp,false, 0, errh);
    
    PSPAddress temp1(0x000000000201810a) ;	   
    PSPRoute route_tmp1(temp1,1);   
    add_route(route_tmp1,false, 0, errh);
    //
    // maybe make a new slot
   //_t.push_back(r);

   /*	
    for (int i = 0; i < conf.size(); i++) {
	 if ((r1 = add_route(route, false, 0, errh)) < 0) {
	    if (r1 == -EEXIST)
		++eexist;
	    else
		r = r1;
	}
    }
    */
 

    if (eexist)
	errh->warning("%d %s replaced by later versions", eexist, eexist > 1 ? "routes" : "route");
    return r;
}


int
PSPRouteTable::add_route(const PSPRoute&, bool, PSPRoute*, ErrorHandler *errh)
{
    // by default, cannot add routes
    return errh->error("cannot add routes to this routing table");
}

int
PSPRouteTable::remove_route(const PSPRoute&, PSPRoute*, ErrorHandler *errh)
{
    // by default, cannot remove routes
    return errh->error("cannot delete routes from this routing table");
}

int
PSPRouteTable::lookup_route(PSPAddress) const
{
    return -1;			// by default, route lookups fail
}

void
PSPRouteTable::push(int, Packet *p)
{
    click_chatter("psproutetable PUSH...");
    const click_ether *peth = reinterpret_cast<const click_ether *>(p->data()) ;
    const click_psp *psp = reinterpret_cast<const click_psp *>(p->data()+14);
    const click_udp *udp = reinterpret_cast<const click_udp *>(psp + 1);
    unsigned char *p_pspaddr = 0;
     click_chatter("psproutetable PUSH...");
    //p_pspaddr = (unsigned char *)&(psp->psp_dst);
    PSPAddress lookuppsp = PSPAddress(psp->psp_dst);
	
    int port = lookup_route(lookuppsp);
    if (port >= 0) {
	assert(port < noutputs());
	output(port).push(p);
    } else {
	static int complained = 0;
	if (++complained <= 5)
	    click_chatter("IPRouteTable: no route for %s", p->dst_ip_anno().unparse().c_str());
	p->kill();
    }
}

int
PSPRouteTable::add_route_handler(const String &conf, Element *e, void *thunk, ErrorHandler *errh)
{
    //PSPRouteTable *table = static_cast<PSPRouteTable *>(e);
   // return table->run_command((thunk ? CMD_SET : CMD_ADD), conf, 0, errh);
   return 0;
}

int
PSPRouteTable::remove_route_handler(const String &conf, Element *e, void *, ErrorHandler *errh)
{
    //IPRouteTable *table = static_cast<IPRouteTable *>(e);
   // return table->run_command(CMD_REMOVE, conf, 0, errh);
   return 0;
}

void
PSPRouteTable::add_handlers()
{
    add_write_handler("add", add_route_handler, 0);

    add_write_handler("remove", remove_route_handler);

}

// must always generate the whole template instance! LookupIPRoute demands it
//template class Vector<PSPTable::Entry>;
CLICK_ENDDECLS
ELEMENT_PROVIDES(PSPRouteTable)

