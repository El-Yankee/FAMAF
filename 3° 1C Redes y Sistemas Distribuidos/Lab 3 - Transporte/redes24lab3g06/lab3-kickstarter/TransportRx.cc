#ifndef TRANSPORT_RX
#define TRANSPORT_RX

#include <string.h>
#include <omnetpp.h>


using namespace omnetpp;


class TransportRx : public cSimpleModule {
private:
    //Buffer
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;

    //Feedback
    cQueue bufferFeedback;
    cMessage *endFeedbackEvent;
    simtime_t ServiceFeedbackTime;
    bool controlFeedback = false;

    //Variables de medicion
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    int packetsDropeados;
public:
    TransportRx();
    virtual ~TransportRx();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
    endFeedbackEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
    cancelAndDelete(endFeedbackEvent);
}

void TransportRx::initialize() {
    //Buffer
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");

    //Feedback
    bufferFeedback.setName("bufferFeedback");
    endFeedbackEvent = new cMessage("endFeedback");

    //Variables
    packetDropVector.setName("PaquetesDorpeados");
    bufferSizeVector.setName("TamañoDeBuffer");
}

void TransportRx::finish() {
}

void TransportRx::handleMessage(cMessage *msg) {

    // si msg es un mensaje endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toApp");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
        }
    } else if (msg == endFeedbackEvent) {
        // si msg es un mensaje endFeedbackEvent
        // if packet in buffer, send next one
        if (!bufferFeedback.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) bufferFeedback.pop();
            // send packet
            send(pkt, "toOut$o");
            // start new service
            ServiceFeedbackTime = pkt->getDuration();
            scheduleAt(simTime() + ServiceFeedbackTime, endFeedbackEvent);
        }
    }else { 
        // if msg is a data packet
        //check buffer limit
        if (buffer.getLength() >= par("bufferSize").intValue()) {
            //drop the packet
            delete msg;
            this->bubble("packet dropped");
            //Registramos que se perdio un paquete
            packetsDropeados++;
            packetDropVector.record(packetsDropeados);
        } else {
            // Agregamos casos para el canal de feedback
            // enqueue the message
            if (msg->getKind() == 2 || msg->getKind() == 4) {
                bufferFeedback.insert(msg);
                // if the server is idle
                if (!endFeedbackEvent->isScheduled()) {
                    // start the service
                    scheduleAt(simTime() + 0, endFeedbackEvent);
                }
            } else {

                // Configuramos el umbral de las queue (80% y 30%)
                float controlBufferMax = 0.8 * par("bufferSize").intValue();
                float controlBufferMin = 0.3 * par("bufferSize").intValue();

                if (buffer.getLength() > controlBufferMax && !controlFeedback) {
                    //Si se esta llenando el buffer mando una señal 
                    // Creamos mensaje de feedback con su tipo
                    cPacket *feedbackPkt;
                    feedbackPkt = new cPacket("feedbackPkt");
                    feedbackPkt->setByteLength(20);
                    feedbackPkt->setKind(2);
                    // Enviamos al canal de feedback
                    send(feedbackPkt, "toOut$o");
                    // Confirmo que mande un feedback
                    controlFeedback = true;
                } else if (buffer.getLength() < controlBufferMin && controlFeedback) {
                    //Si entran pocos paquetes al buffer mando una señal
                    // Creamos mensaje de feedback con su tipo
                    cPacket *feedbackPkt;
                    feedbackPkt = new cPacket("feedbackPkt");
                    feedbackPkt->setByteLength(20);
                    feedbackPkt->setKind(4);
                    // Enviamos al canal de feedback
                    send(feedbackPkt, "toOut$o");
                    // Confirmo que mande un feedback
                    controlFeedback = false;
                }
                
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

#endif /*TRANSPORT_RX*/
