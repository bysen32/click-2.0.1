// label-route.click

define($MAC0 00:0C:31:00:00:00, $MAC1 00:0C:32:00:00:00, $DESTMAC0 00:0C:32:00:00:00, $DESTMAC1 00:0C:41:00:00:00, $IP0 11.2.0.0.0.0.0.0, $IP1 11.4.0.12.18.0.0.0)

c0 :: Classifier(12/0600, 12/0601, -);

FromDevice(eth0_rename)->Paint(1)
				->c0
				
FromDevice(eth1)->Paint(2)
				->c0
				
out0 :: Queue(200) -> ToDevice(eth0_rename);
disp1 :: Dispatcher;
sche1 :: Scheduler;

sche1[0]-> ToDevice(eth1);
disp1[0] -> Queue(20) -> BandwidthShaper(2048B/s) -> [0]sche1;
disp1[1] -> Queue(20) -> BandwidthShaper(4096B/s) -> [1]sche1;
disp1[2] -> Queue(20) -> BandwidthShaper(8192B/s) -> [2]sche1;
disp1[3] -> Queue(20) -> BandwidthShaper(16384B/s) -> [3]sche1;
disp1[4] -> Queue(20) -> BandwidthShaper(32768B/s) -> [4]sche1;
disp1[5] -> Queue(20) -> BandwidthShaper(65536B/s) -> [5]sche1;
disp1[6] -> Queue(20) -> BandwidthShaper(131072B/s) -> [6]sche1;
disp1[7] -> Queue(100)  -> [7]sche1;

cp0 :: CheckPaint(1);
cp1 :: CheckPaint(2);
cp2 :: CheckPaint(1);
dlt0 :: DecLabelTTL;
dlt1 :: DecLabelTTL;
dpt0 :: DecPspTTL;

cp0[1]->dlt0;
cp0[0]->Discard;
dlt0[0]	->ExchangeLabel
		->EncapEth($DESTMAC0,$DESTMAC1)->out0
dlt0[1]->Discard;

cp1[1]->dlt1;
cp1[0]->Discard;
dlt1[0]	->ExchangeLabel
		->EncapEth($DESTMAC0,$DESTMAC1)->disp1
dlt1[1]->Discard;		

cph0 :: CheckPSPHeader;
cph0[0]->psp :: LookupPSPRoute(
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
cp2[1]->dpt0;
cp2[0]->Discard;
dpt0[0]->EncapEth($DESTMAC0,$DESTMAC1)->out0;
dpt0[1]->Discard;

label[0]->cp0
label[1]->cp1
label[2]->PopLabel
		->cph0;
				
c0[0]->Strip(14)->cph0;
c0[1]->Strip(14)->Print("label")->label;
c0[2]->Discard;
