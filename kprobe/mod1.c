#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>

int Pre_Handler(struct kprobe *p, struct pt_regs *regs) {
	struct task_struct *t = (struct task_struct*)(regs->ax);
	printk("Pre_handler task info: %d(%s) is ptracing by %d(%s)\n",
		t->pid, t->comm, current->pid, current->comm);
	return 0;
}
void Post_Handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
	struct task_struct *t = (struct task_struct*)(regs->ax);
	printk("Post_handler task info: %d(%s) is ptracing by %d(%s)\n",
		t->pid, t->comm, current->pid, current->comm);
}

static struct kprobe kp;

static int __init myinit(void) {
	printk("module inserted\n");
	kp.pre_handler = Pre_Handler;
	kp.post_handler = Post_Handler;
	kp.symbol_name = "ptrace_attach";
	//kp.addr = (kprobe_opcode_t*)0xffffffff810a0310;
	register_kprobe(&kp);
	printk("hook at %p\n", kp.addr);
	return 0;
}

static int __exit myexit(void) {
	unregister_kprobe(&kp);
	printk("module removed\n");
	return 0;
}

module_init(myinit);
mudole_exit(myexit);
MODULE_AUTHOR("YAMA");
MODULE_DESCRIPTION("KPROBE MODULE");
MODULE_LICENSE("GPL");
