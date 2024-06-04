#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>

using namespace omnetpp;

class Net: public cSimpleModule {
private:
    cOutVector contadorSaltos;

public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Net);

#endif /* NET */

Net::Net() {
}

Net::~Net() {
}

void Net::initialize() {
    contadorSaltos.setName("Saltos");
}

void Net::finish() {

}

void Net::handleMessage(cMessage *msg) {

    // All msg (events) on net are packets
    Packet *pkt = (Packet *) msg;

    // If this node is the final destination, send to App
    if (pkt->getDestination() == this->getParentModule()->getIndex() && (pkt->getKind()==5 || pkt->getKind()==6) ) {
        contadorSaltos.record(pkt->getHopCount());
        send(msg, "toApp$o");

    } else if (pkt->getDestination() == this->getParentModule()->getIndex() && pkt->getKind() !=0 ){

        if (pkt->getKind()==1) {
            pkt->setKind(3);
            pkt->setDestination(pkt->getSource());

            send(pkt, "toLnk$o", 0);


        } else if (pkt->getKind()==2) {
            pkt->setKind(4);
            pkt->setDestination(pkt->getSource());

            send(pkt, "toLnk$o", 1);

        } else if (pkt->getKind()==3) {
            send(pkt, "toApp$o");

        } else if (pkt->getKind()==4) {
            send(pkt, "toApp$o");
        }

    }
    // If not, forward the packet to some else... to who?
    else {
        // Hacemos que los contadores giren en sentido horario y antihorario
        if (pkt->getKind() ==1) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 1);

        } else if (pkt->getKind()== 2) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 0);

        } else if (pkt->getKind() ==3 ) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 0);

        } else if (pkt->getKind() == 4) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 1);
        } else if (pkt->getKind() == 5) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 1);
        } else if (pkt->getKind() == 6) {
            pkt->setHopCount(pkt->getHopCount() + 1);
            send(pkt, "toLnk$o", 0);
        }
    }
}
