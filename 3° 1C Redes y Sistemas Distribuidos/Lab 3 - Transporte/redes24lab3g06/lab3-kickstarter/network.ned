simple Generator
{
    parameters:
        int packetByteSize;
        volatile double generationInterval; // sec
        @display("i=block/source");
    gates:
        output out;
}

simple Queue
{
    parameters:
        int bufferSize;
        volatile double serviceTime; // sec
        @display("i=block/queue;q=buffer");
    gates:
        input in;
        output out;
}

simple Sink
{
    parameters:
        @display("i=block/sink");
    gates:
        input in;
}

simple TransportTx
{
    parameters:
        int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        input toApp;
        inout toOut;
}

simple TransportRx
{
    parameters:
        int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        output toApp;
        inout toOut;
}

module NodeTx
{
    parameters:
        @display("i=block/routing");
    gates:
        output out;
        input in;
    submodules:
        gen: Generator {
            parameters:
                @display("p=75,50");
        }
        traTx: TransportTx {
            parameters:
                @display("p=75,190");
        }
    connections:
        in --> traTx.toOut$i;
        gen.out --> traTx.toApp;
        traTx.toOut$o --> out;
}

module NodeRx
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
    submodules:
        sink: Sink {
            parameters:
                @display("p=75,50");
        }
        traRx: TransportRx {
            parameters:
                @display("p=75,190");
        }
    connections:
        in --> traRx.toOut$i;
        traRx.toOut$o --> out;
        //case study 1:
        traRx.toApp --> {  datarate = 0.5Mbps; } --> sink.in;
        //case study 2:
        //traRx.toApp --> { datarate = 1Mbps; } --> sink.in;        
}

network Network
{
    @display("bgl=2");
    submodules:
        nodeTx: NodeTx {
            @display("p=30,30");
        }
        queue0: Queue {
            @display("p=130,30");
        }
        queue1: Queue {
            @display("p=130,80");
        }
        nodeRx: NodeRx {
            @display("p=230,30");
        }
    connections:
        nodeTx.out --> {  datarate = 1Mbps; delay = 100us; } --> queue0.in;
        
        //case study 1:
        queue0.out --> {  datarate = 1Mbps; delay = 100us; } --> nodeRx.in;
        //case study 2:
        //queue0.out --> { datarate = 0.5Mbps; delay = 100us; } --> nodeRx.in;
        
        //Conexion canal auxiliar
        nodeRx.out --> {  datarate = 1Mbps; delay = 100us; } --> queue1.in;
        queue1.out --> {  datarate = 1Mbps; delay = 100us; } --> nodeTx.in;
                   
}
