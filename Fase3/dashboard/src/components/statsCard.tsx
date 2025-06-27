import React from 'react';
import {
    BarChart,
    Bar,
    XAxis,
    YAxis,
    Tooltip,
    ResponsiveContainer,
    PieChart,
    Pie,
    Cell,
} from 'recharts';

type StatItem = {
    label: string;
    value: number;
};

type StatsCardProps = {
    title: string;
    stats: StatItem[];
    chartType?: 'bar' | 'pie';
    className?: string;
};

const colors = [
    '#60A5FA',
    '#38BDF8',
    '#818CF8',
    '#A5B4FC',
    '#F472B6',
    '#FBBF24',
    '#34D399',
];

export const StatsCard: React.FC<StatsCardProps> = ({
    title,
    stats,
    chartType = 'bar',
    className = '',
}) => {
    return (
        <div
            className={`bg-gray-800 p-4 rounded-4xl shadow-md w-full max-w-md ${className}`}
            style={{ color: '#FFFFFF' }}
        >
            <h2 className="text-xl font-semibold mb-3">{title}</h2>
            {chartType === 'bar' ? (
                <ResponsiveContainer width="100%" height={200}>
                    <BarChart data={stats}>
                        <XAxis dataKey="label" stroke="#E0E7FF" />
                        <YAxis stroke="#E0E7FF" />
                        <Tooltip />
                        <Bar dataKey="value" fill="#2626D4" />
                    </BarChart>
                </ResponsiveContainer>
            ) : (
                <ResponsiveContainer width="100%" height={200}>
                    <PieChart>
                        <Pie
                            data={stats}
                            dataKey={'value'}
                            nameKey={'label'}
                            cx="50%"
                            cy="50%"
                            outerRadius={50}
                            isAnimationActive={false}
                            label
                        >
                            {stats.map((_, index) => (
                                <Cell
                                    key={`cell-${index}`}
                                    fill={colors[index % colors.length]}
                                />
                            ))}
                        </Pie>
                    </PieChart>
                </ResponsiveContainer>
            )}
            <ul className="mt-4">
                {stats.map(stat => (
                    <li key={stat.label} className="text-sm" style={{ color: '#E0E7FF' }}>
                        {stat.label}:{' '}
                        <span className="font-medium" style={{ color: '#60A5FA' }}>{stat.value}</span>
                    </li>
                ))}
            </ul>
        </div>
    );
};
