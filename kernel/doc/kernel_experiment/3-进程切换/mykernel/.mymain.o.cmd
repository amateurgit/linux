cmd_mykernel/mymain.o := gcc -Wp,-MD,mykernel/.mymain.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/4.8/include -I/usr/local/src/linux-3.9.4/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/usr/local/src/linux-3.9.4/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/usr/local/src/linux-3.9.4/include/uapi -Iinclude/generated/uapi -include /usr/local/src/linux-3.9.4/include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m32 -msoft-float -mregparm=3 -freg-struct-return -fno-pic -mpreferred-stack-boundary=2 -march=i686 -Wa,-mtune=generic32 -ffreestanding -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mymain)"  -D"KBUILD_MODNAME=KBUILD_STR(mymain)" -c -o mykernel/mymain.o mykernel/mymain.c

source_mykernel/mymain.o := mykernel/mymain.c

deps_mykernel/mymain.o := \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/types.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/posix_types_32.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/posix_types.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /usr/lib/gcc/x86_64-linux-gnu/4.8/include/stdarg.h \
  include/uapi/linux/string.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/string.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/string_32.h \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/kmemcheck.h) \
  include/linux/ctype.h \
  include/linux/tty.h \
    $(wildcard include/config/tty.h) \
    $(wildcard include/config/audit.h) \
  include/linux/fs.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/linkage.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/alignment/16.h) \
  include/linux/stringify.h \
  include/linux/wait.h \
    $(wildcard include/config/lockdep.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /usr/local/src/linux-3.9.4/include/uapi/linux/const.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  include/linux/typecheck.h \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/bitops.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/x86/cmov.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/alternative.h \
    $(wildcard include/config/paravirt.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/asm.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/cpufeature.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/p6/nop.h) \
    $(wildcard include/config/matom.h) \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/modules.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/errno.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/errno.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/errno-base.h \
  include/uapi/linux/kernel.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/sysinfo.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/ia32/emulation.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/page.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/page_types.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/page_32_types.h \
    $(wildcard include/config/highmem4g.h) \
    $(wildcard include/config/highmem64g.h) \
    $(wildcard include/config/page/offset.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/page_32.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/x86/3dnow.h) \
  include/linux/range.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/getorder.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/processor.h \
    $(wildcard include/config/x86/vsmp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
    $(wildcard include/config/cpu/sup/amd.h) \
    $(wildcard include/config/xen.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/processor-flags.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/vm86.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/ptrace.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/segment.h \
    $(wildcard include/config/x86/32/lazy/gs.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/ptrace.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/ptrace-abi.h \
  include/asm-generic/ptrace.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/vm86.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/math_emu.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/sigcontext.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/sigcontext.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/current.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
  include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/pgtable_32_types.h \
    $(wildcard include/config/highmem.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/pgtable-2level_types.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable-nopmd.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/msr.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/msr.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/msr-index.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/ioctl.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/cpumask.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/desc_defs.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/special_insns.h \
  include/linux/personality.h \
  include/uapi/linux/personality.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
    $(wildcard include/config/x86/internode/cache/shift.h) \
  include/linux/math64.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/div64.h \
  include/linux/err.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/irqflags.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/atomic.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/cmpxchg.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/cmpxchg_32.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/atomic64_32.h \
  include/asm-generic/atomic-long.h \
  include/linux/bottom_half.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/barrier.h \
    $(wildcard include/config/x86/ppro/fence.h) \
    $(wildcard include/config/x86/oostore.h) \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/uapi/linux/wait.h \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
  include/linux/seqlock.h \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutiny.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/path.h \
  include/linux/stat.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/stat.h \
  include/uapi/linux/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/uapi/linux/time.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/linux/radix-tree.h \
  include/linux/rbtree.h \
  include/linux/pid.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/semaphore.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/fiemap.h \
  include/linux/shrinker.h \
  include/linux/migrate_mode.h \
  include/linux/percpu-rwsem.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/rwsem.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/pfn.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/uapi/linux/fs.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/limits.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/fcntl.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/fcntl.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/major.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/termios.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/termios.h \
  include/asm-generic/termios.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/uaccess.h \
    $(wildcard include/config/x86/intel/usercopy.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/smap.h \
    $(wildcard include/config/x86/smap.h) \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/uaccess_32.h \
    $(wildcard include/config/debug/strict/user/copy/checks.h) \
  include/uapi/asm-generic/termios.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/termbits.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/termbits.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/ioctls.h \
  /usr/local/src/linux-3.9.4/include/uapi/asm-generic/ioctls.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/param.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/timex.h \
  /usr/local/src/linux-3.9.4/arch/x86/include/asm/tsc.h \
    $(wildcard include/config/x86/tsc.h) \
  include/linux/tty_driver.h \
    $(wildcard include/config/console/poll.h) \
  include/linux/export.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  include/linux/cdev.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/tty_ldisc.h \
  /usr/local/src/linux-3.9.4/include/uapi/linux/tty_flags.h \
  include/uapi/linux/tty.h \
  include/linux/vmalloc.h \
    $(wildcard include/config/mmu.h) \
  mykernel/mypcb.h \

mykernel/mymain.o: $(deps_mykernel/mymain.o)

$(deps_mykernel/mymain.o):
