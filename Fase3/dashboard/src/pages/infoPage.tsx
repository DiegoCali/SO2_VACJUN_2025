import React, { useEffect, useState } from 'react';
import { StatsCard } from '../components/statsCard';
import SimpleStatsCard from '../components/simpleStatsCard';
import { get } from '../services/api';

const InfoPage: React.FC = () => {
    const [memStats, setMemStats] = useState<any>(null);
    const [antivirusStats, setAntivirusStats] = useState<any>(null);

    const fetchData = async () => {
        try {
            const memData = await get('/mem_stats');
            const antivirusData = await get('/antivirus_stats');

            if (memData?.memory && memData?.swap) {
                setMemStats(memData);
            }
            if (antivirusData) {
                setAntivirusStats(antivirusData);
            }
        } catch (error) {
            console.error('Error fetching info stats:', error);
        }
    };

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, 10000); // Actualiza cada 10s
        return () => clearInterval(interval);
    }, []);

    const formatMemoryStats = () => {
        const { total_memory, used_memory, free_memory, cached_memory } =
            memStats?.memory || {};
        return memStats?.memory
            ? [
                { label: 'Total', value: total_memory },
                { label: 'Usada', value: used_memory },
                { label: 'Libre', value: free_memory },
                { label: 'Cacheada', value: cached_memory },
            ]
            : [];
    };

    const formatSwapStats = () => {
        const { total_swap, used_swap, free_swap } = memStats?.swap || {};
        return memStats?.swap
            ? [
                { label: 'Total Swap', value: total_swap },
                { label: 'Usada Swap', value: used_swap },
                { label: 'Libre Swap', value: free_swap },
            ]
            : [];
    };

    return (
        <div className="p-6 grid grid-cols-1 md:grid-cols-3 gap-6">
            {memStats?.memory && (
                <StatsCard
                    title="Memoria RAM"
                    stats={formatMemoryStats()}
                    chartType="pie"
                />
            )}
            {memStats?.swap && (
                <StatsCard
                    title="Memoria Swap"
                    stats={formatSwapStats()}
                    chartType="bar"
                />
            )}
            {antivirusStats && (
                <SimpleStatsCard
                    title="Antivirus Stats"
                    data={antivirusStats}
                />
            )}
        </div>
    );
};

export default InfoPage;
