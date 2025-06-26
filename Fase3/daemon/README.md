# Daemon

## Pasos para ejecutar el daemon:
Instalar las dependencias necesarias:
```bash
sudo apt install libjson-c-dev 
```
ejecutar el siguiente comando en la terminal:
```bash
make
```
Luego en la carpeta `/dist` ejecutar el siguiente comando:
```bash
./main
```

## API:

### `GET /api/mem_stats`
Retorna:
```
{
  "memory": {
    "total_memory": <int>,
    "used_memory": <int>,
    "free_memory": <int>,
    "cached_memory": <int>
  }, 
  "swap": {
    "total_swap": <int>,
    "used_swap": <int>,
    "free_swap": <int>
  }
}
```

### `GET /api/processes`
Retorna:
```
{
  "processes": [
    {"pid": <int>, "name": "<name>", "mem_percent": <float>},
    {"pid": <int>, "name": "<name>", "mem_percent": <float>}
  ]
}
```

### `GET /api/antivirus_stats`
Retorna:
```
{
  "processes_monitored": <int>,
  "files_scanned": <int>,
  "quarantined_files": <int>
}
```

### `GET /api/quarantine_files`
Retorna:
```
{
  "files": [
    {"name": "<filename>"},
    {"name": "<filename>"}
  ]
}
```

### `GET /api/pages`
Retorna:
```
{
  "active_pages": <int>,
  "active_pages_mem": <int>,
  "inactive_pages": <int>,
  "inactive_pages_mem": <int>,
}
```


### `POST /api/page_faults`
Body:
```
{
  "pid": <int>
}
```
Retorna:
```
{
  "pid": <pid>,
  "minor_faults": <int>,
  "major_faults": <int>
}
```

### `POST /api/scan_file`
Body:
```
{
  "file_path": "<ruta>",
  "hash": "<hash>"
}
```
Retorna:
```
{
  "status": <int>,
  "hash": "<hash>",
  "file_path": "<ruta>"
}
```

### `POST /api/quarantine_file`
Body:
```
{
  "file_path": "<ruta>"
}
```
Retorna:
```
{
  "status": <int>,
  "file_path": "<ruta>"
}
```

### `POST /api/restore_file`
Body:
```
{
  "filename": "<name>"
}
```
Retorna:
```
{
  "status": <int>,
  "filename": "<name>"
}
```

### `POST /api/delete_file`
Body:
```
{
  "filename": "<name>"
}
```
Retorna:
```
{
  "status": <int>,
  "filename": "<name>"
}
```

### `POST /api/add_signature`
Body:
```
{
  "file_path": "<ruta>",
  "hash": "<hash>"
}
```
Retorna:
```
{
  "status": <int>,
  "file_path": "<ruta>"
}
```


### Errores comunes (código 400)
- Si falta un campo requerido en el body, retorna:
```
{
  "error": "Key '<campo>' not found in POST data"
}
```
- Si el endpoint no existe, retorna:
```
{
  "error": "Not Found"
}
```
- Si el método HTTP no está permitido, retorna:
```
{
  "error": "Method Not Allowed"
}
```

>[!NOTE]
> En la carpeta `testing` se encuentra un archivo json con la configuración de las pruebas a la api en postman.