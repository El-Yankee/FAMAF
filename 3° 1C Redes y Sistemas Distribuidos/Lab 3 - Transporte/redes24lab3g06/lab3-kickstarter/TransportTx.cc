#ifndef TRANSPORT_TX
#define TRANSPORT_TX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class TransportTx : public cSimpleModule {
private:
    //Buffer
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;

    //Feedback
    float tiempoDeEspera;
    float limites;

    //Variables de medición
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    int packetsDropeados;
public:
    TransportTx();
    virtual ~TransportTx();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx() {
    endServiceEvent = NULL;
}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
}

void TransportTx::initialize() {
    //Buffer
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");

    //Feedback
    tiempoDeEspera = 1.0;

    //Variables
    packetDropVector.setName("PaquetesDorpeados");
    bufferSizeVector.setName("TamañoDeBuffer");
}

void TransportTx::finish() {
}

void TransportTx::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toOut$o");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime*tiempoDeEspera, endServiceEvent);
        }
    } else {
        // if msg is a data packet
        //check buffer limit
        if (buffer.getLength() >= par("bufferSize").intValue()) {
            //drop the packet
            delete msg;
            this->bubble("packet dropped");
            packetsDropeados++;
            packetDropVector.record(packetsDropeados);
        } else {
            // Agregamos casos para el canal de feedback
            if (msg->getKind() == 2) {
                tiempoDeEspera = tiempoDeEspera * 4;

            } else if (msg->getKind() == 4) {
                tiempoDeEspera = tiempoDeEspera / 4;

            } else {
                // enqueue the packet
                buffer.insert(msg);
                bufferSizeVector.record(buffer.getLength());
                // if the server is idle
                if (!endServiceEvent->isScheduled()) {
                    // start the service
                    scheduleAt(simTime() + 0, endServiceEvent);
                }
            }
        }
    }
}

#endif /*TRANSPORT_TX*/
