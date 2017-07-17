#include <pcap/pcap.h>

#include <stdio.h>

#include <netinet/in.h>

#include <arpa/inet.h>



// ethernet header structure

struct eth_header {

	u_char dest_mac[6];//print

	u_char src_mac[6];//print

	u_short ether_type;

};



// ip header structure

struct ip_header{

	u_char ip_verhl;

	u_char ip_tos;

	u_char ip_tlen[2];

	u_char ip_id[2];

	u_char ip_off[2];

	u_char ip_ttl;

	u_char ip_pro;

	u_char ip_hche[2];

	struct in_addr ip_src, ip_dst;//print

};



// tcp header structure


struct tcp_header{

	u_char src_port[2];//print

	u_char dst_port[2];//print

	u_char seq[4];

	u_char ack[4];

	double nothing;
};



void print_mac (u_char *mac ){

	int i;

	for ( i = 0; i < 6; i++){

		if( i == 5 ){

			printf("%02x\n",mac[i]);

		}

		else

			printf("%02x:",mac[i]);

	}

}



void print_port (u_char *port ){

	int i;
	
	printf("%u:",port[0]);

	printf("%u:\n",port[1]);

	

}



int main(int argc, char *argv[])

{

	pcap_t *handle;			/* Session handle */

	char *dev;			/* The device to sniff on */

	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */

	struct bpf_program fp;		/* The compiled filter */

	char filter_exp[] = "port 80";	/* The filter expression */

	bpf_u_int32 mask;		/* Our netmask */

	bpf_u_int32 net;		/* Our IP */

	struct pcap_pkthdr *header;	/* The header that pcap gives us */

	const u_char *packet;		/* The actual packet */

	/* Get packet detail */

	int check;

	struct eth_header *eth;

	struct ip_header *ip;

	struct tcp_header *tcp;

	u_char *payload;
	
	int i;



		/* Define the device */

	dev = pcap_lookupdev(errbuf);

	if (dev == NULL) {

		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);

		return(2);

	}

		/* Find the properties for the device */

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {

		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);

		net = 0;

		mask = 0;

	}

		/* Open the session in promiscuous mode */

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

	if (handle == NULL) {

		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);

		return(2);

	}

		/* Compile and apply the filter */

	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {

		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));

		return(2);

	}

	if (pcap_setfilter(handle, &fp) == -1) {

		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));

		return(2);

	}



		// detect and show packet detail

	while ( (check = pcap_next_ex(handle, &header, &packet)) >= 0 ){

		if ( check == 0 ) continue;



		printf("          < Packet Start >\n");



			// show ethernet part

		eth = (struct eth_header *)(packet);

		printf("========== ethernet header part ========== \n");

		printf("packet dest mac : ");

		print_mac(eth->dest_mac);

		printf("packet src mac : ");

		print_mac(eth->src_mac);

		printf("ether type : %02x 00\n",eth->ether_type);

		printf("\n");



			// show ip part

		if( eth->ether_type == 8 ){

			ip = (struct ip_header *)(packet + 14);

			printf("========== ip header part ========== \n");

			printf("source ip : %s\n",inet_ntoa(ip->ip_src));

			printf("destination ip : %s\n", inet_ntoa(ip->ip_dst));

			printf("ip protocol : %02x\n",ip->ip_pro);

			printf("\n");

				// show tcp part

			if ( ip->ip_pro == 6 ){

				tcp = (struct tcp_header *)(packet + 14 + sizeof(struct ip_header));

				printf("========== tcp header part ========== \n");

				printf("source port : %d\n",((tcp->src_port[0])*256+(tcp->src_port[1])));

				printf("destination port : %d\n",((tcp->dst_port[0])*256+(tcp->dst_port[1])));

				printf("\n");

			}else{

				fprintf(stderr, "This protocol is not tcp protocol\n");

				return(2);

			}
			
			payload = (u_char *)(packet + 14 +sizeof(struct ip_header) + sizeof(struct tcp_header));
			
			printf("\n\nData Value : ");

			for( i = 0 ; i < 16 ; i++ ){

				printf("%c", *payload);

				payload = payload + 1;

			}
			printf("\n==========================================");
			printf("\n\n\n");



		}else{

			fprintf(stderr, "This ether type is not ip\n");

			return(2);

		}

	}



		/* And close the session */

	pcap_close(handle);

	return(0);

}
