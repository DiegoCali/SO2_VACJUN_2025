import React from 'react';

type SimpleStatsCardProps = {
    title?: string;
    data: Record<string, number | string>;
    fontSize?: string; // Ejemplo: 'text-lg', 'text-2xl', etc.
};

const SimpleStatsCard: React.FC<SimpleStatsCardProps> = ({
    title,
    data,
    fontSize = 'text-base',
}) => {
    return (
        <div className="bg-gray-800 p-4 rounded-2xl shadow-md w-full max-w-md">
            {title && (
                <h2 className="text-xl font-semibold mb-4 text-white">{title}</h2>
            )}
            <ul className="divide-y divide-gray-700">
                {Object.entries(data).map(([key, value], _) => (
                    <li
                        key={key}
                        className={`flex justify-between items-center py-3 ${fontSize} text-gray-200`}
                    >
                        <span className="capitalize">{key.replace(/_/g, ' ')}</span>
                        <span className="font-semibold text-white">{value}</span>
                    </li>
                ))}
            </ul>
        </div>
    );
};

export default SimpleStatsCard;
