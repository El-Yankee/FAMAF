#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    //Variable analisis tamaño del buffer
    cOutVector bufferSizeVector;
    //Variable analisis paquetes perdidos
    cOutVector packetDropVector;
    // Variables para el feedback
    int packetsDropeados;
    bool controlFeedback = false;
public:
    Queue();
    virtual ~Queue();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize() {
    //Le asignamos un nombre a la variable de analisis
    packetDropVector.setName("PaquetesDropeados");
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
}

void Queue::finish() {
}

void Queue::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "out");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
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
            float controlBufferMax = 0.8 * par("bufferSize").intValue();
            float controlBufferMin = 0.3 * par("bufferSize").intValue();

            //Si se esta llenando el buffer mando una señal
            if (buffer.getLength() > controlBufferMax && !controlFeedback) {
                cPacket *feedbackPkt;
                feedbackPkt = new cPacket("feedbackPkt");
                feedbackPkt->setByteLength(20);
                feedbackPkt->setKind(2);
                buffer.insertBefore(buffer.front(), feedbackPkt);
                controlFeedback = true;
            }

            //Si entran pocos paquetes al buffer mando una señal
            if (buffer.getLength() < controlBufferMin && controlFeedback) {
                cPacket *feedbackPkt;
                feedbackPkt = new cPacket("feedbackPkt");
                feedbackPkt->setByteLength(20);
                feedbackPkt->setKind(4);
                buffer.insertBefore(buffer.front(), feedbackPkt);
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

#endif /* QUEUE */
