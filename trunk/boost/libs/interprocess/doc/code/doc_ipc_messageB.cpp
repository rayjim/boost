   #include <boost/interprocess/detail/config_begin.hpp>
   #include <boost/interprocess/detail/workaround.hpp>

   #include <boost/interprocess/managed_shared_memory.hpp>
   #include <cstddef>

   int main ()
   {
      using namespace boost::interprocess;

      //A shared memory front-end that is able to 
      //allocate raw memory buffers from a shared memory segment
      //Connect to the shared memory segment and initialize needed resources
      managed_shared_memory segment(open_only, "MySharedMemory");  //segment name

      //An handle from the base address can identify any byte of the shared 
      //memory segment even if it is mapped in different base addresses
      managed_shared_memory::handle_t handle = 0;

      //Wait handle msg from the other process and put it in
      //"handle" local variable
      //Get buffer local address from handle
      void *msg = segment.get_address_from_handle(handle);
      (void)msg;
      //Do anything with msg
      //. . .
      //Send ack to sender process
      return 0;
   }

   #include <boost/interprocess/detail/config_end.hpp>
