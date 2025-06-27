import React, { useEffect, useState } from 'react';
import { get } from '../services/api';
import { Table } from '../components/table';
import { StatsCard } from '../components/statsCard';
import { ValueInputBarChart } from '../components/valueInputBarChart';

type Process = {
  pid: number;
  name: string;
  mem_percent: number;
};

type PagesStats = {
  active_pages: number;
  active_pages_mem: number;
  inactive_pages: number;
  inactive_pages_mem: number;
};

const defaultProcess: Process = {
  pid: 0,
  name: '-',
  mem_percent: 0,
};

const MonitoringPage: React.FC = () => {
  const [processes, setProcesses] = useState<Process[]>([defaultProcess]);
  const [pagesStats, setPagesStats] = useState<PagesStats | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  const fetchProcessesAndPages = async () => {
    setLoading(true);
    setError(null);
    try {
      const [processData, pagesData] = await Promise.all([
        get('/processes?pid=0'), // Consulta a pid=0 al cargar
        get('/pages'),
      ]);

      if (processData?.processes && processData.processes.length > 0) {
        setProcesses(processData.processes);
      } else {
        setProcesses([defaultProcess]);
      }

      if (pagesData) {
        setPagesStats(pagesData);
      } else {
        setPagesStats(null);
      }
    } catch (err) {
      setError('Error al obtener datos');
      console.error(err);
      setProcesses([defaultProcess]);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchProcessesAndPages();
    const interval = setInterval(fetchProcessesAndPages, 10000);
    return () => clearInterval(interval);
  }, []);

  const columns = [
    { key: 'pid' as keyof Process, label: 'PID' },
    { key: 'name' as keyof Process, label: 'Nombre' },
    {
      key: 'mem_percent' as keyof Process,
      label: 'Memoria (%)',
      render: (value: string | number, _row: Process) =>
        typeof value === 'number' ? value.toFixed(2) + ' %' : value,
    },
  ];

  const formatActivePagesStats = (): { label: string; value: number }[] => {
    if (!pagesStats) return [];
    return [
      { label: 'Páginas Activas', value: pagesStats.active_pages || 0 },
      {
        label: 'Mem Activa (MB)',
        value: pagesStats.active_pages_mem || 0,
      },
    ];
  };

  const formatInactivePagesStats = (): { label: string; value: number }[] => {
    if (!pagesStats) return [];
    return [
      {
        label: 'Páginas Inactivas',
        value: pagesStats.inactive_pages || 0,
      },
      {
        label: 'Mem Inactiva (MB)',
        value: pagesStats.inactive_pages_mem || 0,
      },
    ];
  };

  return (
    <div className="p-6 space-y-6">
      <div className="flex items-center justify-between mb-4">
        <h1 className="text-2xl font-semibold">Monitoreo de Procesos</h1>
        {loading && (
          <span className="text-sm text-gray-400 animate-pulse">
            Actualizando...
          </span>
        )}
      </div>

      {error && <p className="text-red-500 mb-4">{error}</p>}

      {/* Fila principal con 3 columnas */}
      <div className="flex flex-col md:flex-row md:space-x-8 space-y-6 md:space-y-0 mb-6 min-w-0 items-center">
        {/* Contenedor de las dos gráficas */}
        <div className="self-center flex space-x-6">
          {pagesStats && (
            <StatsCard
              title="Páginas Activas"
              stats={formatActivePagesStats()}
              chartType="pie"
              className="min-w-[200px] max-h-[3500px]"
            />
          )}
          {pagesStats && (
            <StatsCard
              title="Páginas Inactivas"
              stats={formatInactivePagesStats()}
              chartType="pie"
              className="min-w-[200px] max-h-[350px]"
            />
          )}
        </div>

        {/* Formulario + gráfica de page faults */}
        <div className="flex-shrink-0 w-[800px] min-w-[560px] h-[375px]">
          <ValueInputBarChart
            apiUrl="/page-faults"
            inputName="pid"
            inputLabel="Consultar Page Faults"
            title="Page Faults por PID"
          />
        </div>
      </div>

      {/* Tabla con procesos */}
      {processes.length > 0 ? (
        <Table<Process>
          title="Procesos activos"
          columns={columns}
          data={processes}
          pageSize={15}
        />
      ) : (
        !loading && (
          <p className="text-gray-400">No hay procesos disponibles.</p>
        )
      )}
    </div>
  );
};

export default MonitoringPage;
