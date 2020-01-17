// label-route.click

define($MAC0 00:0C:21:00:00:00, $MAC1 00:0C:22:00:00:00, $IP0 11.1.0.0.0.0.0.0, $IP1 11.2.0.0.0.0.0.0)

c0 :: Classifier(12/0600, 12/0601, -);

FromDevice(eth0_rename)->Paint(1)
				->c0
				
FromDevice(eth1)->Paint(2)
				->c0
				
out0 :: Queue(200) -> ToDevice(eth0_rename);
out1 :: Queue(200) -> ToDevice(eth1);

cp0 :: CheckPaint(1);
cp1 :: CheckPaint(2);
cp2 :: CheckPaint(1);
dlt0 :: DecLabelTTL;
dlt1 :: DecLabelTTL;
dpt0 :: DecPspTTL;

cp0[0]->dlt0;
cp0[1]->Discard;
dlt0[0]	->ExchangeLabel
		->PSPMacap($MAC0)->out0
dlt0[1]->Discard;

cp1[0]->dlt1;
cp1[1]->Discard;
dlt1[0]	->ExchangeLabel
		->PSPMacap($MAC1)->out1
dlt1[1]->Discard;		

cph0 :: CheckPSPHeader;
cph0[0]->psp :: LookupPSPRoute(
					$IP0		0,
					$IP1		1
				);
cph0[1]->Discard;
label :: LookupLIB(
	10 	0 	$IP0 	$IP1 	20	1
);

psp[0]	->DropBroadcasts
		->cp2;
psp[1]	->PushLabel
		->label;
cp2[0]->dpt0;
cp2[1]->Discard;
dpt0[0]->PSPMacap($MAC0)->out0;
dpt0[1]->Discard;

label[0]->cp0
label[1]->cp1
label[2]->PopLabel
		->cph0;
				
c0[0]->Strip(14)->cph0;
c0[1]->Strip(14)->label;
c0[2]->Discard;
