

#include <click/config.h>
#include "lookuppsproute.hh"
#include <click/ipaddress.hh>
#include <click/straccum.hh>
#include <click/error.hh>
CLICK_DECLS

LookupPSPRoute::LookupPSPRoute()
    : _zero_route(-1)
{
}

LookupPSPRoute::~LookupPSPRoute()
{
}

int
LookupPSPRoute::initialize(ErrorHandler *)
{
    _last_addr = PSPAddress();
    return 0;
}


int
LookupPSPRoute::add_route(const PSPRoute &r, bool allow_replace, PSPRoute* replaced, ErrorHandler *)
{
	// overwrite any existing route
	int found = _t.size();

	for (int i = 0; i < _t.size(); i++)
    		if (_t[i].addr == r.addr) {
	    		if (replaced)
				*replaced = _t[i];
	    		if (!allow_replace)
				return -EEXIST;
	    		_t[i].port = r.port;
	    
	   	 	//return 0;
		}

//	click_chatter("LookupPSPRoute : add route address = 0x%016x	 port = %d\n", r.addr.addr(), r.port);
	_t.push_back(r);
   
	return 0;
}

int
LookupPSPRoute::remove_route(const PSPRoute& route, PSPRoute* old_route, ErrorHandler *errh)
{
    for (int i = 0; i < _t.size(); i++)
	if (route.match(_t[i])) {
	    if (old_route)
		*old_route = _t[i];
	    _t[i].kill();

	  
	    return 0;
	}
    return -ENOENT;
}

int
LookupPSPRoute::lookup_entry(PSPAddress a) const
{
	for (int i = 0; i < _t.size(); i++)
	{
		if (_t[i].contains(a)) {
//			click_chatter("ricky here look entry");
			return i;
		}
	}
	return -1;
}

int
LookupPSPRoute::lookup_route(PSPAddress a) const
{
    int ei = lookup_entry(a);
    if (ei >= 0) {
       click_chatter("ricky here look route");
	return _t[ei].port;
	
    } else
	return -1;
}


void
LookupPSPRoute::push(int, Packet *p)
{
	

//    click_chatter("LookupPSPRoute PUSH...");
    unsigned bytes = 64;
    StringAccum sa( 2 // label:
		   + 6		// (processor)
		   + 28		// timestamp:
		   + 9		// length |
		   + 84      // packet info
		   + 17 // (h[headroom] t[tailroom])
		   + 8 // annotations |
		   + 3 * bytes);
    if (sa.out_of_memory()) {
		click_chatter("no memory for PSPPrint");
		
    }

    int len;
    
    const click_psp *psp = reinterpret_cast<const click_psp *>(p->data());
    const click_udp *udp = reinterpret_cast<const click_udp *>(psp + 1);
    unsigned char *p_pspaddr = 0;


	// print psp seq(8B) , src addr(32B), des addr, udp srcport(6B) , desport
	p_pspaddr = (unsigned char *)&(psp->psp_src);
	len = sprintf(sa.reserve(32), " src %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);
	p_pspaddr = (unsigned char *)&(psp->psp_dst);
	len = sprintf(sa.reserve(32), " des %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);

  if (bytes) {
		sa << " | ";
                int _contents =1;
		char *buf = sa.data() + sa.length();
		const unsigned char *data = p->data();
		if (_contents == 1) {
		    for (int i = 0; i < bytes; i++, data++) {
			if (i && (i % 4) == 0)
			    *buf++ = ' ';
			sprintf(buf, "%02x", *data & 0xff);
			buf += 2;
		    }
		} else if (_contents == 2) {
		    for (int i = 0; i < bytes; i++, data++) {
			if ((i % 8) == 0)
			    *buf++ = ' ';
			if (*data < 32 || *data > 130)
			    *buf++ = '.';
			else
			    *buf++ = *data;
		    }
		}
		sa.adjust_length(buf - (sa.data() + sa.length()));
    }
   
   
   click_chatter("%s", sa.c_str());

//#define EXCHANGE(a,b,t) { t = a; a = b; b = t; }
    //p_pspaddr = (unsigned char *)&(psp->psp_dst);


    PSPAddress a(psp->psp_dst);
	
    int ei = -1;
    if ((ei = lookup_entry(a)) >= 0) {
         click_chatter("Find a Route for a PSP Packet"); 
	_last_addr = a;
	_last_entry = ei;
    } else {
	static int complained = 0;
	if (++complained <= 5)
	    click_chatter("LookupPSPRout: no route"); 
	p->kill();
	return;
    }

    const PSPRoute &e = _t[ei];
//   click_chatter("the output port is %d", e.port);

    

   
    output(e.port).push(p);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(PSPRouteTable)
EXPORT_ELEMENT(LookupPSPRoute)
