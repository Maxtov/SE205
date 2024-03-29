public class BoundedBufferMain {

    public static void main (String[] args) {
        BoundedBuffer buffer;

        // Check the arguments of the command line
        if (args.length != 1){
            System.out.println ("PROGRAM FILENAME");
            System.exit(1);
        }
        Utils.init(args[0]);

        // Create a buffer
        if (Utils.sem_impl == 0)
            buffer = new NatBoundedBuffer(Utils.bufferSize);
        else
            buffer = new SemBoundedBuffer(Utils.bufferSize);

        // Create producers and then consumers
        for(int i=0; i<Utils.nConsumers; i++){
          Consumer consumer = new Consumer(i,buffer);
          consumer.start();
        }

        for(int i=0; i<Utils.nProducers; i++){
          Producer producer = new Producer(i,buffer);
          producer.start();
        }
    }
}
