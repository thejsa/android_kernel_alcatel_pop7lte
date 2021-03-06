/******************************************************************************/
/*                                                               Date:01/2014 */
/*                             PRESENTATION                                   */
/*                                                                            */
/*      Copyright 2012 TCL Communication Technology Holdings Limited.         */
/*                                                                            */
/* This material is company confidential, cannot be reproduced in any form    */
/* without the written permission of TCL Communication Technology Holdings    */
/* Limited.                                                                   */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/* Author:  Xiaoyun.Wei                                                          */
/* E-Mail:  Xiaoyun.Wei@tcl-mobile.com                                            */
/* Role  :  checkfuseapi                                                        */
/* Reference documents :                                            */
/* -------------------------------------------------------------------------- */
/* Comments:                                                                  */
/* File    : kernel/drivers/char/oemfuse/oemfuse.c                              */
/* Labels  :                                                                  */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
/* Modifications on Features list / Changes Request / Problems Report         */
/* -------------------------------------------------------------------------- */
/* date    | author         | key              | comment (what, where, why)   */
/* --------|----------------|--------------------|--------------------------- */
/* 14/01/21| Xiaoyun.wei    | FR-593076        | Support the check fuse*/
/*---------|----------------|--------------------|--------------------------- */
/******************************************************************************/


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <mach/scm.h>

#include <linux/miscdevice.h>

static int oemfuse_open(struct inode* inode, struct file* filp) {
    return 0;
}

static int oemfuse_release(struct inode* inode, struct file* filp) {
    return 0;
}

static ssize_t oemfuse_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos) {
    u32 tmp=1;
    u32 scmret[32];
    int ret;
    int i=0;
    for( ; i<32; i++ )
        scmret[i] = 0;
    ret = scm_call(254, 2, &tmp,sizeof(tmp),scmret,sizeof(scmret));
    if(ret == 0){
        if(copy_to_user(buf, scmret,count < 128 ? count : 128))
            return -EFAULT;
        else
            return count < 128 ? count : 128;
    }

    return -1;
}

static struct file_operations oemfuse_fops = {
    .owner = THIS_MODULE,
    .open = oemfuse_open,
    .release = oemfuse_release,
    .read = oemfuse_read,
};


static struct miscdevice oemfuse = {
    MISC_DYNAMIC_MINOR,
    "oemfuse",
    &oemfuse_fops
};

static int __init oemfuse_init(void){

    int ret;

    ret = misc_register(&oemfuse);
    if (ret) {
        printk(KERN_ERR "fram: can't misc_register on oemfuse minor=%d\n",
            MISC_DYNAMIC_MINOR);
        return ret;
    }

    printk(KERN_INFO "oemfuse driver init success");
    return ret;
}


static void __exit oemfuse_exit(void) {

    printk(KERN_ALERT"Destroy oemfuse device.\n");
    misc_deregister(&oemfuse);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OEMfuse Driver");

module_init(oemfuse_init);
module_exit(oemfuse_exit);











