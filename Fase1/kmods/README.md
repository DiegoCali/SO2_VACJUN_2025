# Kernel modifications

## Archivos que se modifican:
Según la carpeta raíz de el kernel extrajimos del .tar.gz, los archivos que se modifican son:

### Dirección: `include/linux/uts.h`
Línea 9:
```c
#define UTS_SYSNAME "Linux"
```

Modificación de línea 9:
```c
#define UTS_SYSNAME "usac-kernel-1128-7639-3856"
```

### Dirección: `init/main.c`
agregar la siguiente al archivo:
Línea 966:
```c
printk(KERN_INFO "Bienvenido a usac-kernel-1128-7639-3856\n");
```

