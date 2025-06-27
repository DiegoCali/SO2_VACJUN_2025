#include <linux/syscalls_usac.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sysinfo.h>
#include <linux/uaccess.h>
#include <linux/rcupdate.h>
#include <linux/vmstat.h>

// ----------- SYSCALL: sys_scan_processes -----------
// Devuelve la lista de todos los procesos con: name, pid, mem_percentage
SYSCALL_DEFINE2(scan_processes, struct proc __user *, user_buf, size_t, max_count)
{
    struct task_struct *task;
    struct proc *kbuf; //ya tenemos definida la estructura proc en syscalls_usac.h
    int proc_count = 0, i = 0;
    struct sysinfo si;
    unsigned long total_mem;

    // 1. Contar procesos actuales
    rcu_read_lock();
    for_each_process(task)
        proc_count++;
    rcu_read_unlock();

    // 2. Si solo quieren saber cuántos procesos hay
    if (!user_buf || max_count == 0)
        return proc_count;

    // 3. Reservar buffer temporal en kernel
    int to_copy = (proc_count < max_count) ? proc_count : max_count;
    kbuf = kmalloc_array(to_copy, sizeof(struct proc), GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;

    // 4. Leer memoria total del sistema
    si_meminfo(&si);
    total_mem = si.totalram << (PAGE_SHIFT - 10);

    // 5. Llenar buffer con los procesos
    i = 0;
    rcu_read_lock();
    for_each_process(task) {
        if (i >= to_copy)
            break;
        strncpy(kbuf[i].name, task->comm, TASK_COMM_LEN);
        kbuf[i].name[TASK_COMM_LEN - 1] = '\0';
        kbuf[i].pid = task->pid;
        if (task->mm) {
            unsigned long rss_kb = get_mm_rss(task->mm) * (PAGE_SIZE / 1024);
            kbuf[i].mem_percentage = total_mem ? (rss_kb * 100) / total_mem : 0;
        } else {
            kbuf[i].mem_percentage = 0;
        }
        i++;
    }
    rcu_read_unlock();

    // 6. Copiar a user-space
    if (copy_to_user(user_buf, kbuf, to_copy * sizeof(struct proc))) {
        kfree(kbuf);
        return -EFAULT;
    }
    kfree(kbuf);

    // 7. Siempre devolver el número total de procesos, no solo los copiados
    return proc_count;
}


// ----------- SYSCALL: sys_get_page_faults -----------
// Devuelve los fallos de página menores y mayores para el proceso con el pid dado

SYSCALL_DEFINE2(get_page_faults, pid_t, pid, struct page_faults __user *, user_faults)
{
    struct task_struct *task;
    struct page_faults pf = {0};

    // Encontrar el proceso con el PID dado
    rcu_read_lock();
    task = find_task_by_vpid(pid);
    if (!task) {
        rcu_read_unlock();
        return -ESRCH; // No existe ese proceso
    }

    pf.minor_faults = task->min_flt;
    pf.major_faults = task->maj_flt;
    rcu_read_unlock();

    // Copiar la información a user-space
    if (copy_to_user(user_faults, &pf, sizeof(struct page_faults)))
        return -EFAULT;

    return 0; // Éxito
}

// ----------- SYSCALL: sys_get_memory_usage -----------
// Devuelve info de memoria (total, libre, usada, caché)

SYSCALL_DEFINE1(get_memory_usage, struct mem_info __user *, user_mem)
{
    struct sysinfo si;
    struct mem_info info;

    si_meminfo(&si);

    info.total_memory = si.totalram << (PAGE_SHIFT - 10);   // En KB
    info.free_memory  = si.freeram << (PAGE_SHIFT - 10);    // En KB
    info.cached_memory = si.bufferram << (PAGE_SHIFT - 10); // En KB
    info.used_memory  = info.total_memory - info.free_memory; // En KB

    info.total_swap    = si.totalswap << (PAGE_SHIFT - 10);   // Swap total en KB
    info.free_swap     = si.freeswap << (PAGE_SHIFT - 10);    // Swap libre en KB
    info.used_swap     = info.total_swap - info.free_swap;     // Swap usada en KB

    if (copy_to_user(user_mem, &info, sizeof(struct mem_info)))
        return -EFAULT;

    return 0;
}


// ----------- SYSCALL: sys_get_pages -----------
// Devuelve el número de páginas activas e inactivas

SYSCALL_DEFINE1(get_pages, struct memory_pages_info __user *, user_pages)
{
    struct memory_pages_info info;
    unsigned long act = 0, inact = 0;

    // Obtener recuentos de páginas activas/inactivas usando el API del kernel
    act = global_node_page_state(NR_ACTIVE_FILE) + global_node_page_state(NR_ACTIVE_ANON);
    inact = global_node_page_state(NR_INACTIVE_FILE) + global_node_page_state(NR_INACTIVE_ANON);

    info.active_pages = act;
    info.inactive_pages = inact;
    info.active_pages_mem = act * (PAGE_SIZE / 1024);     // KB
    info.inactive_pages_mem = inact * (PAGE_SIZE / 1024); // KB

    if (copy_to_user(user_pages, &info, sizeof(struct memory_pages_info)))
        return -EFAULT;

    return 0;
}
