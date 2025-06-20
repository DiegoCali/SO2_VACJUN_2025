# Syscalls

Podríamos dividir la función de las syscalls en 2 partes:
1. **Monitoreo**: Recolectar información del sistema y de los procesos en ejecución.
2. **Gestión de cuarentena**: Manejar archivos sospechosos, incluyendo escaneo, cuarentena y restauración.

## Monitoreo:
Se sugieren las siguientes syscalls:
- `sys_scan_processes()`: para monitorear procesos en tiempo real. Se sugiere el uso de un struct `proc` con atributos de: `name`, `pid`, `mem_percentage`, el retorno sugerido es: `[proc, ...]`.
- `sys_get_page_faults(pid)`: para obtener estadísticas de fallos de página por proceso. Retorno sugerido: `[int:minor_faults, int:major_faults]`.
- `sys_antivirus_stats()`: para obtener estadísticas del antivirus, como número de procesos monitoreados, archivos escaneados, etc. Se sugiere un contador de procesos, archivos escaneados, etc. Retorno sugerido: `[int:processes_monitored, int:files_scanned, int:quarantine_files]`.
> Las siguientes syscalls no se sugieren en el enunciado pero son **necesarias** para el dashboard:
- `sys_get_memory_usage()`: para obtener el uso de memoria del sistema. Retorno sugerido: `[int:used_memory, int:free_memory, int:cache_memory, int:used_swap]`.
- `sys_get_pages()`: para obtener estadísticas de páginas activas e inactivas. Retorno sugerido: `[int:active_pages, int:inactive_pages]`.

## Gestión de cuarentena:
Se sugieren las siguientes syscalls:
- `sys_scan_file(path)`: para escanear archivos y calcular hashes comparandolos con un archivo `signatures.db`. Retorno sugerido: `[int:status, string:hash]`, donde status puede ser 0 (no malicioso), 1 (malicioso) o -1 (error).
- `sys_quarantine_file(path)`: para mover archivos sospechosos a cuarentena. Retorno sugerido: `[int:status]`, donde status puede ser 0 (éxito), -1 (error).
- `sys_get_quarantine_list()`: para obtener la lista de archivos en cuarentena. Retorno sugerido: `[string:filename, ...]`.
- `sys_restore_file(filename)`: para restaurar archivos desde cuarentena. Retorno sugerido: `[int:status]`, donde status puede ser 0 (éxito), -1 (error).
> Las siguientes syscalls no se sugieren en el enunciado pero facilitarían las pruebas:
- `sys_delete_file(filename)`: para eliminar archivos en cuarentena. Retorno sugerido: `[int:status]`, donde status puede ser 0 (éxito), -1 (error).
- `sys_add_signature(path)`: para agregar una nueva firma al archivo `signatures.db` generando un hash del archivo de entrada. Retorno sugerido: `[int:status]`, donde status puede ser 0 (éxito), -1 (error).

> [!NOTE]
> Las llamadas al sistema deberán de ser desarrolladas en su propio archivo “syscalls_usac.h” y ser compiladas junto al kernel, no se aceptará que estén incluidas en otro archivo como sys.h.

# Documentación:
```markdown
# sys_scan_processes()
Esta syscall devuelve una lista de procesos con su nombre, PID y porcentaje de memoria utilizada.
## Argumentos
- `void`: No recibe argumentos.
## Retorno
- `struct proc*`: Un puntero a un array de estructuras `proc` que contienen la información de los procesos.
```