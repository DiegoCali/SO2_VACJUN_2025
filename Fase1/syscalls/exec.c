/* 
	* Se edito con la intencion de notificar al usuario que se ha
	* usado una funcion que tiene que ver con la red por lo que si
	* un proceso que no debe de usarlo usó la misma, podrá identificarlo
	* de manera mas sencilla, la implementacion solo notifica mas no 
	* interviene.
*/


//la siguiente funcion es la que se edita, en la ruta fs
static int do_execveat_common(int fd, struct filename *filename,
			      struct user_arg_ptr argv,
			      struct user_arg_ptr envp,
			      int flags)
{

	 /**
	 * revisa que todo esté bien con el nombre del archivo
	 * y que no se esté intentando ejecutar un comando peligroso.
	 * Si se detecta un comando peligroso, se emite una advertencia
	 * mas no lo detiene
	 * */ 
	
	if (filename && filename->name) {
        const char *fname = filename->name;

        if (strstr(fname, "nmap") || strstr(fname, "ssh") || strstr(fname, "nc") || strstr(fname, "netcat")) {
            printk(KERN_WARNING "Advertencia: intento de uso de herramienta de red por PID: %d\n", current->pid);
        }
    }
	struct linux_binprm *bprm;
	int retval;

	if (IS_ERR(filename))
		return PTR_ERR(filename);
	/*
	 * We move the actual failure in case of RLIMIT_NPROC excess from
	 * set*uid() to execve() because too many poorly written programs
	 * don't check setuid() return code.  Here we additionally recheck
	 * whether NPROC limit is still exceeded.
	 */
	if ((current->flags & PF_NPROC_EXCEEDED) &&
	    is_rlimit_overlimit(current_ucounts(), UCOUNT_RLIMIT_NPROC, rlimit(RLIMIT_NPROC))) {
		retval = -EAGAIN;
		goto out_ret;
	}

	/* We're below the limit (still or again), so we don't want to make
	 * further execve() calls fail. */
	current->flags &= ~PF_NPROC_EXCEEDED;

	bprm = alloc_bprm(fd, filename);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}

	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv: empty string added\n",
			     current->comm, bprm->filename);
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;

	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;

	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;

	/*
	 * When argv is empty, add an empty string ("") as argv[0] to
	 * ensure confused userspace programs that start processing
	 * from argv[1] won't end up walking envp. See also
	 * bprm_stack_limits().
	 */
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
			goto out_free;
		bprm->argc = 1;
	}

	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
}
