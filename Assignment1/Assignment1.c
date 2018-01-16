#include<linux/init.h>
#include<linux/module.h>

int first_module_init(void)
{

   printk(KERN_ALERT"Hey World'What's up?\n\n");
   return 0;

}

int first_module_exit(void)
{
   printk(KERN_ALERT"Leaving this world \n \n");
   return 0;
}

module_init(first_module_init);
module_exit(first_module_exit);



