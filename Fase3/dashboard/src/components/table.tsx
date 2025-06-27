import React, { useState } from 'react';

type Column<T> = {
    key: keyof T;
    label: string;
    render?: (value: T[keyof T], row: T) => React.ReactNode;
};

type TableProps<T> = {
    columns: Column<T>[];
    data: T[];
    title?: string;
    pageSize?: number;
};

export function Table<T>({
    columns,
    data,
    title,
    pageSize = 10,
}: TableProps<T>) {
    const [currentPage, setCurrentPage] = useState(1);
    const totalPages = Math.ceil(data.length / pageSize);

    const paginatedData = data.slice(
        (currentPage - 1) * pageSize,
        currentPage * pageSize
    );

    return (
        <div className="bg-gray-800 rounded-2xl shadow p-6 w-full overflow-auto">
            {title && <h2 className="text-xl font-bold mb-4 text-gray-100">{title}</h2>}

            <table className="min-w-full text-sm text-left">
                <thead className="bg-gray-700 text-gray-200 uppercase text-xs">
                    <tr>
                        {columns.map(col => (
                            <th key={col.key as string} className="py-3 px-4 border-b border-gray-600">
                                {col.label}
                            </th>
                        ))}
                    </tr>
                </thead>
                <tbody>
                    {paginatedData.map((row, idx) => (
                        <tr
                            key={idx}
                            className="hover:bg-gray-700 even:bg-gray-800 odd:bg-gray-900 text-gray-100 border-b border-gray-700"
                        >
                            {columns.map(col => (
                                <td
                                    key={col.key as string}
                                    className="py-2 px-4"
                                >
                                    {col.render
                                        ? col.render(row[col.key], row)
                                        : (row[col.key] as React.ReactNode)}
                                </td>
                            ))}
                        </tr>
                    ))}
                </tbody>
            </table>

            {/* Pagination */}
            {totalPages > 1 && (
                <div className="flex justify-end mt-4 space-x-2">
                    <button
                        onClick={() => setCurrentPage(p => Math.max(p - 1, 1))}
                        className="px-3 py-1 rounded-md border border-gray-600 text-sm text-gray-200 bg-gray-700 hover:bg-gray-600"
                        disabled={currentPage === 1}
                    >
                        Anterior
                    </button>
                    {Array.from({ length: totalPages }, (_, i) => (
                        <button
                            key={i}
                            onClick={() => setCurrentPage(i + 1)}
                            className={`px-3 py-1 rounded-md text-sm ${
                                currentPage === i + 1
                                    ? 'bg-blue-600 text-white'
                                    : 'border border-gray-600 text-gray-200 bg-gray-700 hover:bg-gray-600'
                            }`}
                        >
                            {i + 1}
                        </button>
                    ))}
                    <button
                        onClick={() => setCurrentPage(p => Math.min(p + 1, totalPages))}
                        className="px-3 py-1 rounded-md border border-gray-600 text-sm text-gray-200 bg-gray-700 hover:bg-gray-600"
                        disabled={currentPage === totalPages}
                    >
                        Siguiente
                    </button>
                </div>
            )}
        </div>
    );
}
