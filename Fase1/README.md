# Objetivos:
- Aprender a compilar el kernel de Linux.
- Aprender a realizar modificaciones al kernel de Linux.
- Comprender como funcionan las llamadas al sistema de Linux.

# Configuración del entorno:
Se estará utilizando la siguiente version del kernel de Linux: `6.6.44` en una maquina virtual con mint 21.1, con las siguientes características:
- 4 GB de RAM.
- 4 núcleos de CPU.
- 20 GB de disco duro.

# Descargar e instalar el kernel:

Instalar primero las dependencias necesarias para compilar el kernel:
```bash
 sudo apt install build-essential libncurses-dev bison flex libssl-dev libelf-dev fakeroot dwarves
 ```

Descargar el kernel de Linux:
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.44.tar.xz
tar -xf linux-6.6.44.tar.xz
```

Entrar al directorio del kernel:
```bash
cd linux-6.6.44
```

# Configurar el kernel:
Para configurar el kernel, se puede utilizar una configuración predefinida o crear una desde cero. Para usar una configuración predefinida, se puede copiar el archivo de configuración del kernel actual:
```bash
make defconfig
cp /boot/config-$(uname -r) .config
```
O para crear una configuración mínima, se puede usar el comando:
```bash
make localmodconfig
```
Eliminar las llaves privadas de la configuración:
```bash
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""
```

# Compilar el kernel:
Para compilar el kernel, se puede usar el comando:
```bash
fakeroot make -j$(nproc)
```

# Instalar el kernel:
Para instalar el kernel compilado, se puede usar el comando:
```bash
sudo make modules_install
sudo make install
```
>[!NOTE]
> Cuando reiniciar la máquina virtual, se debe seleccionar el nuevo kernel en el menú de GRUB.

# Errores comunes:

## Error por memoria insuficiente:
![error_memoria](/Fase1/imgs/memory_error.jpeg)
Para solucionar este error, se debe aumentar la memoria RAM de la máquina virtual. Se recomienda al menos 4 GB de RAM para compilar el kernel o agregar un archivo de intercambio (swap) para evitar este error.
## Error por configuración incorrecta:
![error_configuracion](/Fase1/imgs/config_error.jpeg)
Si se produce un error de configuración, se debe revisar el archivo `.config` y asegurarse de que todas las opciones necesarias estén habilitadas. También se puede intentar usar una configuración predefinida como `make defconfig` o `make localmodconfig`.

# como se ve una compilación exitosa:
![compilacion_exitosa](/Fase1/imgs/successful_build.jpeg)
