#include 'system.h'
 /**
  * main file to to retrive all Os system informations
  */
  int main(void) 
  {
        while (1) 
        {
            get_cpu_usage();
            get_memory_usage();
            get_disk_usage();
            get_services_list();

            sleep(5);
        }
        return (0);
  }