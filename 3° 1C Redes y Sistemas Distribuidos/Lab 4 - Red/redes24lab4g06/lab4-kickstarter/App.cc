#ifndef APP
#define APP

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>

using namespace omnetpp;

class App: public cSimpleModule {
private:
    cMessage *sendMsgEvent;
    cStdDev delayStats;
    cOutVector delayVector;

    // Variables de control
    bool firstTime;
    bool llegoder;
    int saltosder;
    bool llegoizq;
    int saltosizq;

public:
    App();
    virtual ~App();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(App);

#endif /* APP */

App::App() {
}

App::~App() {
}

void App::initialize() {

    // If interArrivalTime for this node is higher than 0
    // initialize packet generator by scheduling sendMsgEvent
    if (par("interArrivalTime").doubleValue() != 0) {
        sendMsgEvent = new cMessage("sendEvent");
        scheduleAt(par("interArrivalTime"), sendMsgEvent);
    }

    // Inicializa en true porque no sabe que tan lejos esta
    firstTime = true;

    // Initialize statistics
    delayStats.setName("TotalDelay");
    delayVector.setName("Delay");
}

void App::finish() {
    // Record statistics
    recordScalar("Average delay", delayStats.getMean());
    recordScalar("Number of packets", delayStats.getCount());
}

void App::handleMessage(cMessage *msg) {

    Packet *recivo = (Packet *) msg;

    // if msg is a sendMsgEvent, create and send new packet
    // Si tenemos que enviar un paquete y es nuestra primera vez, vemos que tan lejos estamos del destino para ambos lados
    if (msg == sendMsgEvent && firstTime) {

        // Creamos el paquete que ira por la izq
        Packet *pktizq = new Packet("JumpCounterIzq");
        pktizq->setSource(this->getParentModule()->getIndex());
        pktizq->setDestination(par("destination"));
        pktizq->setHopCount(0);
        pktizq->setKind(1);

        // Creamos el paquete que ira por la der
        Packet *pktder = new Packet("JumpCounterDer");
        pktder->setSource(this->getParentModule()->getIndex());
        pktder->setDestination(par("destination"));
        pktder->setHopCount(0);
        pktder->setKind(2);

        // Controlador de que todavia no llego la respuesta
        llegoizq = false;
        saltosizq = 0;

        llegoder = false;
        saltosder = 0;

        // Este chequeo solo se realiza la primera vez
        firstTime = false;

        send(pktder, "toNet$o");
        send(pktizq, "toNet$o");

    } else if (msg == sendMsgEvent && !firstTime && llegoizq && llegoder){

        // Si tenemos que enviar un paquete y no es nuestra primera vez, ya sabemos por que lado ir

        // create new packet
        Packet *pkt = new Packet("packet",this->getParentModule()->getIndex());
        pkt->setByteLength(par("packetByteSize"));
        pkt->setSource(this->getParentModule()->getIndex());
        pkt->setDestination(par("destination"));
        pkt->setHopCount(0);

        // Vemos por que lado es mas corto
        if (saltosder > saltosizq){
            // Se manda por la izq
            pkt->setKind(5);

        } else {
            // Se manda por la der
            pkt->setKind(6);
        }

        // send to net layer
        send(pkt, "toNet$o");

        // compute the new departure time and schedule next sendMsgEvent
        simtime_t departureTime = simTime() + par("interArrivalTime");
        scheduleAt(departureTime, sendMsgEvent);

    } else if (recivo->getKind()==3){

        // Me llega el aviso de que llego el feedback de la izq
        llegoizq = true;
        saltosizq = recivo->getHopCount();

        // Yo no se que lado va a llegar antes, asique cuando llega el ultimo, envio el mensaje
        if (llegoizq && llegoder){
            // create new packet
            Packet *pkt = new Packet("packet",this->getParentModule()->getIndex());
            pkt->setByteLength(par("packetByteSize"));
            pkt->setSource(this->getParentModule()->getIndex());
            pkt->setDestination(par("destination"));
            pkt->setHopCount(0);

            if (saltosder > saltosizq){
                // Se manda por la izq
                pkt->setKind(5);

            } else {
                // Se manda por la der
                pkt->setKind(6);
            }

            // send to net layer
            send(pkt, "toNet$o");

            // compute the new departure time and schedule next sendMsgEvent
            simtime_t departureTime = simTime() + par("interArrivalTime");
            scheduleAt(departureTime, sendMsgEvent);
        }

    } else if (recivo->getKind()==4){

        // Me llega el aviso de que llego el feedback de la der
        llegoder = true;
        saltosder = recivo->getHopCount();

        // Yo no se que lado va a llegar antes, asique cuando llega el ultimo, envio el mensaje
        if (llegoizq && llegoder){
            // create new packet
            Packet *pkt = new Packet("packet",this->getParentModule()->getIndex());
            pkt->setByteLength(par("packetByteSize"));
            pkt->setSource(this->getParentModule()->getIndex());
            pkt->setDestination(par("destination"));
            pkt->setHopCount(0);

            if (saltosder > saltosizq){
                // Se manda por la izq
                pkt->setKind(5);

            } else {

                // Se manda por la der
                pkt->setKind(6);
            }

            // send to net layer
            send(pkt, "toNet$o");

            // compute the new departure time and schedule next sendMsgEvent
            simtime_t departureTime = simTime() + par("interArrivalTime");
            scheduleAt(departureTime, sendMsgEvent);
        }

    }
    // else, msg is a packet from net layer
    else {
        // compute delay and record statistics
        simtime_t delay = simTime() - msg->getCreationTime();
        delayStats.collect(delay);
        delayVector.record(delay);
        // delete msg
        delete (msg);
    }

}
