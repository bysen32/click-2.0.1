#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include "lookuplib.hh"
#include <clicknet/label.h>
#include <click/packet_anno.hh>
CLICK_DECLS

LookupLIB::LookupLIB()
{
}

LookupLIB::~LookupLIB()
{
}

int
LookupLIB::configure(Vector<String> &conf, ErrorHandler *errh)
{
//	click_chatter("LookupLIB: configure the label information base ...");

	String s;
	LabelInfor li;

	for(int i = 0; i < conf.size(); i++) {
		s = conf[i];
		if (!parseInt(cp_shift_spacevec(s), li.labIn))
			{
//			click_chatter("parseInt:%d", li.labIn);
			return false;
			}
		if (!parseInt(cp_shift_spacevec(s), li.intfIn))
			{
//			click_chatter("parseInt:%d", li.intfIn);
			return false;
			}
		if (!parseAddr(cp_shift_spacevec(s), li.addrPrefix))
			{
//			click_chatter("parseAddr:0x%016x", li.addrPrefix);
			return false;
			}
		if (!parseAddr(cp_shift_spacevec(s), li.nextPsp))
			{
//			click_chatter("parseAddr:0x%016x", li.nextPsp);
			return false;
			}
		if (!parseInt(cp_shift_spacevec(s), li.labOut))
			{
//			click_chatter("parseInt:%d", li.labOut);
			return false;
			}
		if (!parseInt(cp_shift_spacevec(s), li.intfOut))
			{
//			click_chatter("parseInt:%d", li.intfOut);
			return false;
			}
//		click_chatter("label infor: labIn=%d labOut=%d intfIn=%d intOut=%d", li.labIn, li.labOut, li.intfIn, li.intfOut);
		add_label_infor(li, errh);
	}
	
	return 0;
}

int
LookupLIB::initialize(ErrorHandler *)
{
	return 0;
}

int
LookupLIB::lookup_entry(int labelIn) const
{
	for (int i = 0; i < _t.size(); i++) {
		if (labelIn == _t[i].labIn) {
			return i;
		}
	}
	click_chatter("LookupLIB: lookup entry finished ...");
	return -1;
}

void
LookupLIB::push(int, Packet *p)
{
//	click_chatter("LookupLIB PUSH...");
	
	const click_label *label = reinterpret_cast<const click_label *>(p->data());
	const click_psp *psp = reinterpret_cast<const click_psp *>(label+1);
	
	SET_QOS_ANNO(p, label->label_qos);
	int port;
	click_chatter("packet seq %d", psp->psp_id);
	int i = lookup_entry(label->label_id);
	if (-1 != i) {
		port = _t[i].intfOut;
		SET_LABEL_OUT_ANNO(p, _t[i].labOut);
		output(port).push(p);
	}
	else
		click_chatter("LookupLIB: label not found ...");
}

int
LookupLIB::add_label_infor(const LabelInfor& li, ErrorHandler *errh)
{
	_t.push_back(li);
	return 0;
}

const char *
LookupLIB::basic_parse(const char *s, const char *end,
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
LookupLIB::parseAddr(const String &str, uint64_t &result)
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
LookupLIB::parseInt(const String &str, int &port)
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

CLICK_ENDDECLS
EXPORT_ELEMENT(LookupLIB)

