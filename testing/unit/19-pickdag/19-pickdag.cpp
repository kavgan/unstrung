#include <stdio.h>
#include <stdlib.h>
#include "iface.h"

#include "fakeiface.h"

int main(int argc, char *argv[])
{
        pcap_network_interface *iface = NULL;
        pcap_network_interface *iface2 = NULL;
        struct in6_addr iface_src2;

	rpl_event::event_debug_time = true;

        rpl_debug *deb = new rpl_debug(true, stderr);
        inet_pton(AF_INET6, "fe80::1000:ff:fe64:6423", &iface_src2);

	dag_network *d1 = dag_network::find_or_make_by_string("T1",deb,false);
	d1->set_active();
	d1->set_interval(50);

        /* now finish setting things up with netlink */
        pcap_network_interface::scan_devices(deb);

	/* this has a DAGID: T1 */
        iface = pcap_network_interface::setup_infile_outfile("wlan0", "../INPUTS/dio-19-t1.pcap", "../OUTPUTS/19-pickdag.pcap", deb);
        iface->set_debug(deb);
        iface->set_if_index(1);
        iface->set_if_addr(iface_src2);

        deb->log("Processing input file 1\n");
        iface->process_pcap();

        deb->log("Events created for file 1\n");
	printevents(stderr, iface->things_to_do);

        deb->log("Forcing events created for file 1\n");
	iface->force_next_event();

	/* this has a DAGID: T2 (should not be processed */
        iface = pcap_network_interface::setup_infile_outfile("wlan0", "../INPUTS/dio-19-t2.pcap", "/dev/null", deb);

        deb->log("\nProcessing input file 2\n");
        iface->process_pcap();

        deb->log("Events created for file 2 (should be none)\n");
	printevents(stderr, iface->things_to_do);

        deb->log("Forcing events created for file 2 (should be none)\n");
	iface->force_next_event();


	exit(0);
}


