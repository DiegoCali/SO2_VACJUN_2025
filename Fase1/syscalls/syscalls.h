// ...

// Hasta el final de todo el archivo...

asmlinkage long sys_get_time_sec(void);
asmlinkage long sys_get_logs(char __user * user_buffer, size_t len);
asmlinkage long sys_panic_signal(const char __user *msg);