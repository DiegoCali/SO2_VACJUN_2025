#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/err.h>
#include <linux/namei.h>          /* getname()/putname() */
#include <linux/syscalls_usac.h>  /* declaración de sys_xor_encrypt y sys_xor_decrypt */

/* Estructura para manejar fragmentos de datos */
typedef struct {
    unsigned char *data;    /* Buffer de datos */
    size_t        data_size;/* Tamaño total del buffer */
    unsigned char *key;     /* Buffer de la clave */
    size_t        key_size; /* Tamaño de la clave */
    size_t        start;    /* Offset inicial (inclusive) */
    size_t        end;      /* Offset final (exclusive) */
} DataFragment;

/* Parámetros de cada hilo */
struct task_parameters {
    DataFragment      fragment;
    struct completion done;
};

/* ------------------------------------------------------------
 * xor_task: ejecuta XOR sobre fragmento [start..end)
 * ------------------------------------------------------------
 */
static int xor_task(void *arg)
{
    struct task_parameters *p = arg;
    DataFragment          *f = &p->fragment;
    size_t                 i;

    for (i = f->start; i < f->end; i++)
        f->data[i] ^= f->key[i % f->key_size];

    complete(&p->done);
    return 0;
}

/* ------------------------------------------------------------
 * process_file: lee input, aplica XOR en paralelo, escribe out
 * ------------------------------------------------------------
 */
static int process_file(const char __user *u_input,
                        const char __user *u_output,
                        const char __user *u_key,
                        int num_threads)
{
    struct filename       *tmp = NULL;
    struct file           *in = NULL, *out = NULL, *kf = NULL;
    unsigned char         *key_buf = NULL, *data_buf = NULL;
    struct task_parameters *params = NULL;
    struct task_struct    **threads = NULL;
    DataFragment          *frags = NULL;
    loff_t                  in_off = 0, out_off = 0, key_off = 0;
    size_t                  in_size, key_size;
    size_t                  frag_sz, rem;
    int                     i, ret = 0;
    ssize_t                 written;

    /* --- Abrir fichero de entrada --- */
    tmp = getname(u_input);
    if (IS_ERR(tmp)) {
        ret = PTR_ERR(tmp);
        tmp = NULL;
        goto cleanup;
    }
    in = filp_open(tmp->name, O_RDONLY, 0);
    putname(tmp);
    if (IS_ERR(in)) {
        ret = PTR_ERR(in);
        in = NULL;
        goto cleanup;
    }

    /* --- Abrir fichero de salida --- */
    tmp = getname(u_output);
    if (IS_ERR(tmp)) {
        ret = PTR_ERR(tmp);
        tmp = NULL;
        goto cleanup;
    }
    out = filp_open(tmp->name, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    putname(tmp);
    if (IS_ERR(out)) {
        ret = PTR_ERR(out);
        out = NULL;
        goto cleanup;
    }

    /* --- Abrir fichero de clave --- */
    tmp = getname(u_key);
    if (IS_ERR(tmp)) {
        ret = PTR_ERR(tmp);
        tmp = NULL;
        goto cleanup;
    }
    kf = filp_open(tmp->name, O_RDONLY, 0);
    putname(tmp);
    if (IS_ERR(kf)) {
        ret = PTR_ERR(kf);
        kf = NULL;
        goto cleanup;
    }

    /* --- Leer clave --- */
    key_size = i_size_read(file_inode(kf));
    if (key_size == 0) {
        ret = -EINVAL;
        goto cleanup;
    }
    key_buf = kmalloc(key_size, GFP_KERNEL);
    if (!key_buf) {
        ret = -ENOMEM;
        goto cleanup;
    }
    ret = kernel_read(kf, key_buf, key_size, &key_off);
    if (ret < 0)
        goto cleanup;

    /* --- Leer datos de entrada --- */
    in_size = i_size_read(file_inode(in));
    if (in_size == 0) {
        ret = -EINVAL;
        goto cleanup;
    }
    data_buf = kmalloc(in_size, GFP_KERNEL);
    if (!data_buf) {
        ret = -ENOMEM;
        goto cleanup;
    }
    ret = kernel_read(in, data_buf, in_size, &in_off);
    if (ret < 0)
        goto cleanup;

    /* --- Preparar hilos --- */
    threads = kmalloc_array(num_threads, sizeof(*threads), GFP_KERNEL);
    params  = kmalloc_array(num_threads, sizeof(*params),  GFP_KERNEL);
    frags   = kmalloc_array(num_threads, sizeof(*frags),   GFP_KERNEL);
    if (!threads || !params || !frags) {
        ret = -ENOMEM;
        goto cleanup;
    }

    frag_sz = in_size / num_threads;
    rem     = in_size % num_threads;

    for (i = 0; i < num_threads; i++) {
        frags[i].data      = data_buf;
        frags[i].data_size = in_size;
        frags[i].key       = key_buf;
        frags[i].key_size  = key_size;
        frags[i].start     = i * frag_sz;
        frags[i].end       = (i == num_threads - 1)
                              ? (i + 1) * frag_sz + rem
                              : (i + 1) * frag_sz;

        params[i].fragment = frags[i];
        init_completion(&params[i].done);

        threads[i] = kthread_run(xor_task, &params[i], "xor_thr_%d", i);
        if (IS_ERR(threads[i])) {
            ret = PTR_ERR(threads[i]);
            threads[i] = NULL;
            goto cleanup;
        }
    }

    /* --- Esperar finalización --- */
    for (i = 0; i < num_threads; i++)
        wait_for_completion(&params[i].done);

    /* --- Escribir salida --- */
    written = kernel_write(out, data_buf, in_size, &out_off);
    if (written < 0) {
        ret = written;
        goto cleanup;
    }
    if (written != in_size) {
        ret = -EIO;
        goto cleanup;
    }

    vfs_fsync(out, 0);

cleanup:
    if (in)  filp_close(in,  NULL);
    if (out) filp_close(out, NULL);
    if (kf)  filp_close(kf,  NULL);

    kfree(key_buf);
    kfree(data_buf);
    kfree(threads);
    kfree(params);
    kfree(frags);

    return ret;
}
/*
*/

SYSCALL_DEFINE4(xor_decrypt,
                const char __user *, file_route_in,
                const char __user *, file_route_out,
                int,                 thread_count,
                const char __user *, key_file_route)
{
    return process_file(file_route_in,
                        file_route_out,
                        key_file_route,
                        thread_count);
}