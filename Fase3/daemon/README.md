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
  "total": 8192,
  "used": 4096,
  "free": 2048,
  "cached": 1024,
  "swapped": 512
}
```


### `GET /api/processes`
Retorna:
```
{
  "processes": [
    {"pid": 1, "name": "init", "mem_percent": 0.5},
    {"pid": 2, "name": "kthreadd", "mem_percent": 0.1}
  ]
}
```


### `GET /api/antivirus_stats`
Retorna:
```
{
  "processes_monitored": 100,
  "files_scanned": 5,
  "quarantined_files": 2
}
```


### `GET /api/quarantine_files`
Retorna:
```
{
  "files": [
    {"name": "malware.exe"},
    {"name": "virus.txt"}
  ]
}
```


### `GET /api/pages`
Retorna:
```
{
  "active_pages": 1000,
  "inactive_pages": 500
}
```
### `GET /api/page_faults`
Retorna:
```
{
  "page_faults": 12345,
  "major_page_faults": 6789
}
```