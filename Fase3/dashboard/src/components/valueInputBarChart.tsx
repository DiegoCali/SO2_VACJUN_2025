import React, { useState } from 'react';
import { StatsCard } from './statsCard';
import { post } from '../services/api'; // Asumo que tienes un método post similar a get

type Props = {
    apiUrl: string; // URL a la que se hace POST
    inputName: string; // Nombre del campo en el body (ej: "pid")
    title: string; // Título para el componente/form
    inputLabel?: string; // Label para el input (opcional)
};

type ResponseData = {
    [key: string]: number;
};

export const ValueInputBarChart: React.FC<Props> = ({
    apiUrl,
    inputName,
    title,
    inputLabel,
}) => {
    const [inputValue, setInputValue] = useState('');
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);
    const [data, setData] = useState<ResponseData | null>(null);

    const handleSubmit = async (e: React.FormEvent) => {
        e.preventDefault();
        setError(null);
        setData(null);

        // Validar que el valor es un número entero positivo
        const numValue = Number(inputValue);
        if (!Number.isInteger(numValue) || numValue < 0) {
            setError('Por favor ingresa un número entero válido');
            return;
        }

        setLoading(true);
        try {
            const body = { [inputName]: numValue };
            const res = await post(apiUrl, body);
            setData(res);
        } catch (err) {
            setError('Error al obtener los datos');
            console.error(err);
        } finally {
            setLoading(false);
        }
    };

    // Formatear data para StatsCard
    const formatData = (): { label: string; value: number }[] => {
        if (!data) return [];
        return Object.entries(data)
            .filter(([key]) => key !== inputName) // excluye el input enviado, ej "pid"
            .map(([key, value]) => ({
                label: key.replace(/_/g, ' '), // para que "minor_faults" sea "minor faults"
                value,
            }));
    };

    return (
        <div className="bg-gray-950 p-6 rounded-3xl max-w-4xl mx-auto shadow-lg h-full">
            <div className="flex gap-6 items-start">
                <div className="w-1/3 flex flex-col">
                    <h2 className="text-2xl font-semibold mb-4">{title}</h2>
                    <form onSubmit={handleSubmit} className="flex flex-col">
                        <label
                            className="block mb-2 font-medium"
                            htmlFor={inputName}
                        >
                            {inputLabel || inputName}
                        </label>
                        <input
                            type="text"
                            id={inputName}
                            value={inputValue}
                            onChange={e => setInputValue(e.target.value)}
                            className="w-full p-2 rounded-md bg-gray-800 border border-gray-700 text-white"
                            placeholder={`Ingrese ${inputLabel || inputName}`}
                        />
                        <button
                            type="submit"
                            disabled={loading}
                            className="mt-4 px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded-md text-white font-semibold disabled:opacity-50"
                        >
                            {loading ? 'Cargando...' : 'Consultar'}
                        </button>
                        {error && <p className="text-red-500 mt-2">{error}</p>}
                    </form>
                </div>

                <div className="flex-1">
                    {data && (
                        <StatsCard
                            title={`Resultados para ${inputLabel || inputName}: ${inputValue}`}
                            stats={formatData()}
                            chartType="bar"
                        />
                    )}
                </div>
            </div>
        </div>
    );
};
