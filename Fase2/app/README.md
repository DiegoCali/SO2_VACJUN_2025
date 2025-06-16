# Documentación de Aplicaciones

## Descripción General

Estas dos aplicaciones permiten cifrar (`encrypt.c`) y descifrar (`decrypt.c`) archivos utilizando llamadas al sistema personalizadas. Cada aplicación acepta argumentos de línea de comandos para especificar el archivo de entrada, el archivo de salida, el número de hilos y la clave de cifrado/descifrado.

> **Nota:** Las llamadas al sistema (`syscall`) están comentadas y reemplazadas por impresiones simuladas para propósitos de prueba. Los números de syscall usados son `998` para cifrado y `999` para descifrado.

## Uso

### Comando General

```bash
./<programa> -p <archivo_entrada> -o <archivo_salida> -j <cantidad_hilos> -k <clave>
```

### Argumentos

| Opción | Descripción                  | Obligatorio |
| ------ | ---------------------------- | ----------- |
| `-p`   | Ruta del archivo de entrada  | Sí          |
| `-o`   | Ruta del archivo de salida   | Sí          |
| `-j`   | Cantidad de hilos a utilizar | Sí          |
| `-k`   | Clave de cifrado/descifrado  | Sí          |

### Ejemplos

#### Cifrado

```bash
./encrypt -p datos.txt -o datos.enc -j 4 -k mi_clave
```

#### Descifrado

```bash
./decrypt -p datos.enc -o datos_descifrados.txt -j 4 -k mi_clave
```

## Consideraciones

* Las syscalls `__NR_encrypt` y `__NR_decrypt` deben estar implementadas en el núcleo del sistema operativo para que funcionen realmente.