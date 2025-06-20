# Daemon

El daemon será el encargado de hacer las llamadas al sistema además de el envío de datos al dashboard. Podríamos dividirlo en 3 partes:
1. **Ejecución de syscalls**: El daemon ejecutará las syscalls y obtendrá el retorno de estas.
2. **Procesamiento de datos**: El daemon procesará los datos obtenidos de las syscalls para generar la información que se enviará al dashboard.
3. **Envío de datos al dashboard**: El daemon enviará los datos procesados al dashboard para su visualización.

## Ejecución de syscalls:
Para la ejecución de syscalls, se sugiere implementar las siguientes funciones:
- `get_mem_stats()`: Esta función obtendrá las estadísticas de memoria del sistema, incluyendo los fallos de página, las páginas activas y la memoria swap.
- `get_processes()`: Esta función obtendrá la lista de procesos en ejecución, incluyendo su nombre, PID y porcentaje de memoria utilizada.
- `get_antivirus_stats()`: Esta función obtendrá las estadísticas del antivirus, incluyendo el número de procesos monitoreados, archivos escaneados, etc.
- `get_quarantine_files()`: Esta función obtendrá la lista de archivos en cuarentena, incluyendo su nombre, hash y estado.
- `scan_file(file_path)`: Esta función escaneará un archivo específico y devolverá el resultado del escaneo, incluyendo el estado del archivo (malicioso, no malicioso, error) y su hash.
- `add_to_quarantine(file_path)`: Esta función añadirá un archivo a la cuarentena y devolverá el estado de la operación.
- `restore_from_quarantine(file_path)`: Esta función restaurará un archivo desde la cuarentena y devolverá el estado de la operación.
- `remove_from_quarantine(file_path)`: Esta función eliminará un archivo de la cuarentena y devolverá el estado de la operación.

## Procesamiento de datos:
Es una parte intermedia entre la ejecución de syscalls y el envío de datos al dashboard. Aquí se procesarán los datos obtenidos de las syscalls para generar la información que se enviará al dashboard.

## Envío de datos al dashboard:
Para el envío de datos al dashboard, se utilizará el protocolo HTTP. El daemon enviará los datos procesados al dashboard a través de peticiones HTTP POST. Se sugiere implementar las siguientes rutas:
- `/api/mem_stats`: Ruta para enviar las estadísticas de memoria del sistema.
- `/api/processes`: Ruta para enviar la lista de procesos en ejecución.
- `/api/antivirus_stats`: Ruta para enviar las estadísticas del antivirus.
- `/api/quarantine_files`: Ruta para enviar la lista de archivos en cuarentena.
- `/api/scan_file`: Ruta para enviar el resultado del escaneo de un archivo.
- `/api/add_to_quarantine`: Ruta para añadir un archivo a la cuarentena.
- `/api/restore_from_quarantine`: Ruta para restaurar un archivo desde la cuarentena.
- `/api/remove_from_quarantine`: Ruta para eliminar un archivo de la cuarentena.

# Documentación:
```markdown
# Daemon
- <Tecnología utilizada>
- <Puerto de acceso al daemon>
- <Ejecución del daemon>
# Rutas
- `/api/mem_stats`: Enviar estadísticas de memoria del sistema.
```

>[!NOTE]
> Utilizar Postman para probar las rutas del daemon. Esto te permitirá verificar que las rutas están funcionando correctamente y que los datos se envían y reciben como se espera sin la necesidad de tener todo el sistema implementado.