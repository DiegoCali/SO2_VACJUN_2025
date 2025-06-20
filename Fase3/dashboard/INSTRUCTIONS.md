# Dashboard

El dashboard será nuestra interfaz grafica para el usuario final, sus funcionalidades se podrían dividir en 2 partes:
1. **Visualización de datos**: El usuario podrá ver los datos que se han recolectado y procesado por el sistema. Esto incluye gráficos, tablas y otros elementos visuales que representen la información de manera clara y concisa.
2. **Interacción con el sistema**: El usuario podrá interactuar con el sistema para realizar acciones como filtrar datos, buscar información específica, y posiblemente enviar comandos o solicitudes al sistema.

## Visualización de datos:
Para la visualización de datos, se sugiere el uso de las siguientes herramientas y tecnologías:
- **Gráficos**: Utilizar librerías como Chart.js o D3.js para crear gráficos interactivos que representen los datos de procesos, uso de memoria, estadísticas del antivirus, etc.
- **Tablas**: Utilizar librerías como DataTables o Tabulator para mostrar listas de procesos, archivos en cuarentena, etc. Estas tablas pueden incluir funcionalidades de búsqueda y ordenamiento.
- **Indicadores de estado**: Utilizar componentes visuales como tarjetas o badges para mostrar estadísticas clave del sistema, como el número de procesos monitoreados, archivos escaneados, etc.
- **Alertas y notificaciones**: Implementar un sistema de alertas para notificar al usuario sobre eventos importantes, como detección de archivos maliciosos o problemas de rendimiento del sistema además de feedback visual para acciones exitosas o fallidas.

## Interacción con el sistema:
Aquí trabajaremos con la gestión de cuarentena y el monitoreo del sistema. Se sugiere implementar las siguientes funcionalidades:
- **Monitoreo de procesos**: Mostrar una lista de procesos en ejecución con su nombre, PID y porcentaje de memoria utilizada. Permitir al usuario filtrar y buscar procesos específicos.
- **Estadísticas del antivirus**: Mostrar estadísticas del antivirus, como el número de procesos monitoreados, archivos escaneados, etc. Esto puede incluir gráficos y tablas para una mejor visualización.
- **Gestión de cuarentena**: Permitir al usuario ver la lista de archivos en cuarentena, restaurar archivos desde cuarentena, y eliminar archivos de cuarentena. Esto puede incluir botones para realizar estas acciones y confirmaciones visuales.
- **Escaneo de archivos**: Permitir al usuario escanear archivos específicos y mostrar el resultado del escaneo, incluyendo el estado del archivo (malicioso, no malicioso, error) y su hash.

# Documentación:
```markdown
# Dashboard
- <Tecnología utilizada>
- <Puerto de acceso al dashboard>
- <Ejecución del dashboard>

# Funcionalidades
...
```

> [!NOTE]
> Utilizar postman para probar las rutas del dashboard y el daemon. Esto te permitirá verificar que las rutas están funcionando correctamente y que los datos se envían y reciben como se espera sin la necesidad de tener todo el sistema implementado.